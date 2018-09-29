/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2018, Erik Moqvist
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * This file is part of the Simba project.
 */

#include "simba.h"

#if CONFIG_HARNESS_DEBUG == 1
#    define DPRINT(fmt, ...) std_printf(OSTR(fmt), ##__VA_ARGS__)
#else
#    define DPRINT(fmt, ...)
#endif

/* A rough estimate of the average mock entry size, including heap
   allocation overhead. */
#define MOCK_ENTRY_SIZE (sizeof(struct mock_entry_t) + 8 * sizeof(void *))

#define HEAP_SIZE (MOCK_ENTRY_SIZE * CONFIG_HARNESS_MOCK_ENTRIES_MAX)

struct mock_entry_t {
    struct list_elem_t base;
    struct entry_t *next_p;
    const char *id_p;
#if CONFIG_HARNESS_WRITE_BACKTRACE_DEPTH_MAX > 0
    struct {
        void *array[CONFIG_HARNESS_WRITE_BACKTRACE_DEPTH_MAX];
        int8_t depth;
    } backtrace;
#endif
    struct {
        size_t size;
        uint8_t buf[1];
    } data;
};

struct mock_entry_cb_t {
    struct list_elem_t base;
    struct mock_entry_t *entry_p;
    harness_mock_cb_t fn;
    char arg[1];
};

struct module_t {
    struct {
        struct heap_t obj;
        uint8_t buf[HEAP_SIZE];
    } heap;
    struct {
        struct list_t list;
        struct list_t cb_list;
    } mock;
    struct mutex_t mutex;
    struct bus_t bus;
    int total;
    int passed;
    int failed;
    int skipped;
    int current_testcase_result;
};

static struct module_t module;

static int print_backtrace_array(void *array[], int depth, const char *name_p)
{
    int i;

    std_printf(OSTR("\r\n"
                    "Mock %s backtrace (most recent call first):\r\n"),
               name_p);

    for (i = 0; i < depth; i++) {
#if defined(ARCH_LINUX)
        fprintf(stderr, ": %p\r\n", array[2 * i]);
#else
        std_printf(OSTR(": 0x%08x\r\n"), array[2 * i]);
#endif
    }

    return (0);
}

static int print_backtrace(const char *name_p)
{
    void *array[2 * (CONFIG_HARNESS_BACKTRACE_DEPTH_MAX + 8)];
    int depth;

    depth = sys_backtrace(array, sizeof(array));

    return (print_backtrace_array(array, depth, name_p));
}

static int print_assert_backtrace(void)
{
    return (print_backtrace("assert"));
}

static int print_read_backtrace(void)
{
    return (print_backtrace("read"));
}

static int print_write_backtrace(void)
{
    return (print_backtrace("write"));
}

#if CONFIG_HARNESS_WRITE_BACKTRACE_DEPTH_MAX > 0

static int mock_entry_create_write_backtrace(struct mock_entry_t *entry_p)
{
    void *array[2 * CONFIG_HARNESS_WRITE_BACKTRACE_DEPTH_MAX];
    int depth;
    int i;

    depth = sys_backtrace(array, sizeof(array));

    for (i = 0; i < depth; i++) {
        entry_p->backtrace.array[i] = array[2 * i];
    }

    entry_p->backtrace.depth = (int8_t)depth;

    return (0);
}

static int mock_entry_print_write_backtrace(struct mock_entry_t *entry_p)
{
    void *array[2 * CONFIG_HARNESS_BACKTRACE_DEPTH_MAX];
    int depth;
    int i;

    depth = (int)entry_p->backtrace.depth;

    for (i = 0; i < depth; i++) {
        array[2 * i] = entry_p->backtrace.array[i];
    }

    return (print_backtrace_array(&array[0], depth, "write"));
}

#else

static int mock_entry_create_write_backtrace(struct mock_entry_t *entry_p)
{
    return (0);
}

static int mock_entry_print_write_backtrace(struct mock_entry_t *entry_p)
{
    return (0);
}

#endif

static struct mock_entry_cb_t *alloc_mock_entry_cb(size_t size)
{
    struct mock_entry_cb_t *entry_cb_p;

    mutex_lock(&module.mutex);
    entry_cb_p = heap_alloc(&module.heap.obj,
                            sizeof(*entry_cb_p) + size - 1);
    mutex_unlock(&module.mutex);

    return (entry_cb_p);
}

static int free_mock_entry_cb_no_lock(struct mock_entry_cb_t *entry_cb_p)
{
    heap_free(&module.heap.obj, entry_cb_p);

    return (0);
}

static struct mock_entry_cb_t *find_mock_entry_cb(struct mock_entry_t *entry_p)
{
    struct list_iter_t iter;
    struct mock_entry_cb_t *entry_cb_p;

    list_iter_init(&iter, &module.mock.cb_list);

    while (1) {
        entry_cb_p = (struct mock_entry_cb_t *)list_iter_next(&iter);

        if (entry_cb_p == NULL) {
            break;
        }

        if (entry_cb_p->entry_p == entry_p) {
            list_remove(&module.mock.cb_list, entry_cb_p);
            break;
        }
    }

    return (entry_cb_p);
}

static struct mock_entry_t *alloc_mock_entry_no_lock(const char *id_p,
                                                     size_t size)
{
    struct mock_entry_t *entry_p;

    DPRINT("Allocating mock entry for id '%s'.\r\n", id_p);

    entry_p = heap_alloc(&module.heap.obj,
                         sizeof(*entry_p) + size - 1);

    if (entry_p != NULL) {
        entry_p->id_p = id_p;
    }

    return (entry_p);
}

static struct mock_entry_t *alloc_mock_entry(const char *id_p,
                                             size_t size)
{
    struct mock_entry_t *entry_p;

    mutex_lock(&module.mutex);
    entry_p = alloc_mock_entry_no_lock(id_p, size);
    mutex_unlock(&module.mutex);

    return (entry_p);
}

static int free_mock_entry(struct mock_entry_t *entry_p)
{
    DPRINT("Freeing mock entry with id '%s'.\r\n", entry_p->id_p);

    mutex_lock(&module.mutex);
    heap_free(&module.heap.obj, entry_p);
    mutex_unlock(&module.mutex);

    return (0);
}

static struct mock_entry_t *copy_mock_entry_no_lock(struct mock_entry_t *entry_p)
{
    struct mock_entry_t *copy_p;

    copy_p = alloc_mock_entry_no_lock(entry_p->id_p,
                                      entry_p->data.size);
    memcpy(copy_p, entry_p, sizeof(*entry_p) + entry_p->data.size - 1);

    return (copy_p);
}

static struct mock_entry_t *find_mock_entry(const char *id_p)
{
    struct list_iter_t iter;
    struct mock_entry_t *entry_p;
    struct mock_entry_t *new_entry_p;
    struct mock_entry_cb_t *entry_cb_p;
    int res;

    mutex_lock(&module.mutex);

    list_iter_init(&iter, &module.mock.list);

    while (1) {
        entry_p = (struct mock_entry_t *)list_iter_next(&iter);

        if (entry_p == NULL) {
            break;
        }

        if (strcmp(entry_p->id_p, id_p) == 0) {
            entry_cb_p = find_mock_entry_cb(entry_p);

            if (entry_cb_p == NULL) {
                list_remove(&module.mock.list, entry_p);
            } else {
                /* Make a copy of the mock entry since the mock
                   callback may modify it. */
                new_entry_p = copy_mock_entry_no_lock(entry_p);

                res = entry_cb_p->fn(&entry_cb_p->arg[0],
                                     &new_entry_p->data.buf[0],
                                     &new_entry_p->data.size);

                if (res == 1) {
                    list_remove(&module.mock.list, entry_p);
                    free_mock_entry_cb_no_lock(entry_cb_p);
                } else {
                    list_add_tail(&module.mock.cb_list, entry_cb_p);
                }

                entry_p = new_entry_p;
            }

            break;
        }
    }

    mutex_unlock(&module.mutex);

    return (entry_p);
}

static int read_mock_entry(struct mock_entry_t *entry_p,
                           const char *id_p,
                           void *buf_p,
                           size_t size,
                           const char *function_p)
{
    ssize_t res;

    res = -1;

    if (size == entry_p->data.size) {
        if (size > 0) {
            if (buf_p != NULL) {
                memcpy(buf_p,
                       &entry_p->data.buf[0],
                       size);
                res = size;
            } else {
                std_printf(OSTR("\r\n%s(): Got NULL pointer with size greater "
                                "than zero(0) for mock id '%s'."),
                           function_p,
                           id_p);
            }
        } else {
            res = 0;
        }
    } else {
        std_printf(OSTR("\r\n%s(): Trying to read exactly %d bytes(s) from "
                        "mock entry with id '%s' but got %d"),
                   function_p,
                   size,
                   id_p,
                   entry_p->data.size);
    }

    if (res < 0) {
        std_printf(OSTR(" ::\r\n"
                        "Mock entry data:\r\n"));
        std_hexdump(sys_get_stdout(),
                    &entry_p->data.buf[0],
                    entry_p->data.size);
        print_read_backtrace();
        mock_entry_print_write_backtrace(entry_p);
        harness_set_testcase_result(-1);
    }

    free_mock_entry(entry_p);

    return (res);
}

static int mwrite_cb(void *arg_p, void *buf_p, size_t *size_p)
{
    int *length_p;

    length_p = arg_p;

    (*length_p)--;

    return (*length_p == 0);
}

static ssize_t create_mock_entry(const char *id_p,
                                 const void *buf_p,
                                 size_t size,
                                 struct mock_entry_t **entry_pp)
{
    ASSERTN(id_p != NULL, EINVAL);

    struct mock_entry_t *entry_p;

    if ((buf_p == NULL) && (size > 0)) {
        std_printf(OSTR("create_mock_entry(): Got NULL pointer with size "
                        "greater than zero(0) for mock id '%s'\r\n"),
                   id_p);
        print_write_backtrace();
        harness_set_testcase_result(-1);

        return (-EINVAL);
    }

    entry_p = alloc_mock_entry(id_p, size);

    if (entry_p == NULL) {
        std_printf(
            OSTR("Mock entry memory allocation failed for id '%s'\r\n"),
            id_p);
        print_write_backtrace();
        harness_set_testcase_result(-1);

        return (-ENOMEM);
    }

    /* Initiate the object. */
    if (size > 0) {
        memcpy(&entry_p->data.buf[0], buf_p, size);
    }

    entry_p->data.size = size;
    mock_entry_create_write_backtrace(entry_p);

    *entry_pp = entry_p;

    return (size);
}

static int number_of_testcases(struct harness_testcase_t *testcase_p)
{
    int number_of_testcases;

    number_of_testcases = 0;

    while (testcase_p->callback != NULL) {
        number_of_testcases++;
        testcase_p++;
    }

    return (number_of_testcases);
}

static int print_report_and_stop(void)
{
    int total;
    int passed;
    int failed;
    int skipped;

    total = module.total;
    passed = module.passed;
    failed = module.failed;
    skipped = module.skipped;
    skipped += (total - passed - failed - skipped);

    std_printf(OSTR("\r\n"
                    "harness report: total(%d), passed(%d), "
                    "failed(%d), skipped(%d)\r\n\r\n"),
               total,
               passed,
               failed,
               skipped);

    std_printf(OSTR("=============================== TEST END (%s) =========="
                    "====================\r\n\r\n"),
               ((passed + skipped) == total ? "PASSED" : "FAILED"));

    sys_stop(module.failed);

    return (0);
}

int harness_run(struct harness_testcase_t *testcases_p)
{
    int err;
    struct harness_testcase_t *testcase_p;
    struct mock_entry_t *entry_p;
    size_t sizes[HEAP_FIXED_SIZES_MAX] = {
        8, 16, 32, 32, 32, 32, 32, 32
    };

    mutex_init(&module.mutex);

    module.total = number_of_testcases(testcases_p);
    module.passed = 0;
    module.failed = 0;
    module.skipped = 0;
    testcase_p = testcases_p;

    thrd_sleep_ms(CONFIG_HARNESS_SLEEP_MS);

    /* Print a header. */
    std_printf(OSTR("\r\n"));

    std_printf(OSTR("================================== TEST BEGIN =========="
                    "========================\r\n\r\n"));
    std_printf(sys_get_info());
    std_printf(OSTR("\r\n"));
    std_printf(OSTR("mock heap size: %u bytes\r\n"), sizeof(module.heap.buf));

    while (testcase_p->callback != NULL) {
        /* Reinitialize the heap before every testcase for minimal
           memory usage. */
        list_init(&module.mock.list);
        list_init(&module.mock.cb_list);

        heap_init(&module.heap.obj,
                  &module.heap.buf[0],
                  sizeof(module.heap.buf),
                  &sizes[0]);

        /* Mark current testcase as passed before its executed. */
        harness_set_testcase_result(0);

        std_printf(OSTR("\r\nenter: %s\r\n"), testcase_p->name_p);

        err = testcase_p->callback();

        do {
            entry_p = (struct mock_entry_t *)list_remove_head(&module.mock.list);

            if (entry_p != NULL) {
                std_printf(OSTR("Found unread mock id '%s'. Failing test.\r\n"),
                           entry_p->id_p);
                err = -1;
            }
        } while (entry_p != NULL);

        if ((err < 0) || (harness_get_testcase_result() == -1)) {
            module.failed++;
            std_printf(OSTR("exit: %s: FAILED\r\n"),
                       testcase_p->name_p);
#if CONFIG_HARNESS_EARLY_EXIT == 1
            print_report_and_stop();
#endif
        } else if ((err == 0) && (harness_get_testcase_result() == 0)) {
            module.passed++;
            std_printf(OSTR("exit: %s: PASSED\r\n"),
                       testcase_p->name_p);
        } else {
            module.skipped++;
            std_printf(OSTR("exit: %s: SKIPPED\r\n"),
                       testcase_p->name_p);
        }

        testcase_p++;
    }

#if CONFIG_THRD_FS_COMMANDS == 1
    char buf[18];

    std_printf(OSTR("\r\n"));
    std_strcpy(buf, OSTR("/kernel/thrd/list"));
    fs_call(buf, NULL, sys_get_stdout(), NULL);
#endif

    return (print_report_and_stop());
}

int harness_expect(void *chan_p,
                   const char *pattern_p,
                   const struct time_t *timeout_p)
{
    char c;
    size_t length;
    size_t pattern_length;
    static char buf[CONFIG_HARNESS_EXPECT_BUFFER_SIZE];
    static struct time_t timeout = {
        .seconds = 1,
        .nanoseconds = 0
    };

    length = 0;
    pattern_length = strlen(pattern_p);

    if (timeout_p == NULL) {
        timeout_p = &timeout;
    }

    while (length < sizeof(buf) - 1) {
        if (chan_poll(chan_p, timeout_p) == NULL) {
            return (-ETIMEDOUT);
        }

        chan_read(chan_p, &c, sizeof(c));

        std_printf(OSTR("%c"), c);

        buf[length++] = c;
        buf[length] = '\0';

        /* Compare to pattern. */
        if (length >= pattern_length) {
            if (strcmp(&buf[length - pattern_length], pattern_p) == 0) {
                return (length);
            }
        }
    }

    return (-1);
}

ssize_t harness_mock_write(const char *id_p,
                           const void *buf_p,
                           size_t size)
{
    ASSERTN(id_p != NULL, EINVAL);

    ssize_t res;
    struct mock_entry_t *entry_p;

    res = create_mock_entry(id_p, buf_p, size, &entry_p);

    if (res != size) {
        return (res);
    }

    /* Add the entry at the end of the list. */
    mutex_lock(&module.mutex);
    list_add_tail(&module.mock.list, entry_p);
    mutex_unlock(&module.mutex);

    return (res);
}

ssize_t harness_mock_mwrite(const char *id_p,
                            const void *buf_p,
                            size_t size,
                            int length)
{
    return (harness_mock_cwrite(id_p,
                                buf_p,
                                size,
                                mwrite_cb,
                                &length,
                                sizeof(length)));
}

ssize_t harness_mock_cwrite(const char *id_p,
                            const void *buf_p,
                            size_t size,
                            harness_mock_cb_t cb,
                            void *arg_p,
                            size_t arg_size)
{
    ssize_t res;
    struct mock_entry_t *entry_p;
    struct mock_entry_cb_t *entry_cb_p;

    res = create_mock_entry(id_p,
                            buf_p,
                            size,
                            &entry_p);

    if (res != size) {
        return (res);
    }

    /* Allocate a callback entry. */
    entry_cb_p = alloc_mock_entry_cb(arg_size);

    if (entry_cb_p == NULL) {
        std_printf(
            OSTR("harness_mock_cwrite(): Mock entry callback memory allocation "
                 "failed for id '%s'\r\n"),
            id_p);
        print_write_backtrace();
        harness_set_testcase_result(-1);
        free_mock_entry(entry_p);

        return (-ENOMEM);
    }

    /* Initiate the callback entry. */
    entry_cb_p->entry_p = entry_p;
    entry_cb_p->fn = cb;

    if (arg_p != NULL) {
        memcpy(&entry_cb_p->arg[0], arg_p, arg_size);
    }

    mutex_lock(&module.mutex);
    list_add_tail(&module.mock.list, entry_p);
    list_add_tail(&module.mock.cb_list, entry_cb_p);
    mutex_unlock(&module.mutex);

    return (size);
}

ssize_t harness_mock_read(const char *id_p,
                          void *buf_p,
                          size_t size)
{
    struct mock_entry_t *entry_p;
    ssize_t res;

    res = -1;
    entry_p = find_mock_entry(id_p);

    if (entry_p != NULL) {
        res = read_mock_entry(entry_p, id_p, buf_p, size, "harness_mock_read");
    } else {
        std_printf(OSTR("\r\nharness_mock_read(): Mock id '%s' not found.\r\n"),
                   id_p);
        print_read_backtrace();
        harness_set_testcase_result(-1);
    }

    return (res);
}

ssize_t harness_mock_try_read(const char *id_p,
                              void *buf_p,
                              size_t size)
{
    struct mock_entry_t *entry_p;
    ssize_t res;

    res = -ENOENT;
    entry_p = find_mock_entry(id_p);

    if (entry_p != NULL) {
        res = read_mock_entry(entry_p,
                              id_p,
                              buf_p,
                              size,
                              "harness_mock_try_read");
    }

    return (res);
}

int harness_mock_assert(const char *id_p,
                        const void *buf_p,
                        size_t size)
{
    ASSERTN(id_p != NULL, EINVAL);

    struct mock_entry_t *entry_p;
    int res;

    res = -1;
    entry_p = find_mock_entry(id_p);

    if (entry_p != NULL) {
        if (size == entry_p->data.size) {
            if (size > 0) {
                if (buf_p != NULL) {
                    res = memcmp(buf_p, &entry_p->data.buf[0], entry_p->data.size);

                    if (res != 0) {
                        std_printf(OSTR("\r\nharness_mock_assert(): Data "
                                        "mismatch for mock id '%s' "),
                                   id_p);
                        res = -1;
                    }
                } else {
                    std_printf(OSTR("\r\nharness_mock_assert(): Got NULL pointer "
                                    "with size greater than zero(0) for mock id "
                                    "'%s' "),
                               id_p);
                }
            } else {
                res = 0;
            }
        } else {
            std_printf(OSTR("\r\nharness_mock_assert(): Trying to read exactly "
                            "%d bytes(s) from mock id '%s' but got %d"),
                       size,
                       id_p,
                       entry_p->data.size);
        }

        if (res < 0) {
            if ((buf_p != NULL) && (size > 0)) {
                _ASSERTHEX("actual",
                           buf_p,
                           "expected",
                           &entry_p->data.buf[0],
                           size,
                           entry_p->data.size);
            } else {
                std_printf(OSTR("::\r\nMock entry data:\r\n"));
                std_hexdump(sys_get_stdout(),
                            &entry_p->data.buf[0],
                            entry_p->data.size);
            }

            print_assert_backtrace();
            mock_entry_print_write_backtrace(entry_p);
            harness_set_testcase_result(-1);
        }

        free_mock_entry(entry_p);
    } else {
        std_printf(OSTR("\r\nharness_mock_assert(): %s: mock id not found\r\n"),
                   id_p);
        print_assert_backtrace();
        harness_set_testcase_result(-1);
    }

    return (res);
}

ssize_t harness_mock_write_notify(const char *id_p,
                                  const void *buf_p,
                                  size_t size)
{
    ASSERTN(id_p != NULL, EINVAL);

    uint32_t mask;
    ssize_t res;

    res = harness_mock_write(id_p, buf_p, size);

    if (res != size) {
        return (res);
    }

    mask = 1;
    bus_write(&module.bus, 0, &mask, sizeof(mask));

    return (size);
}

ssize_t harness_mock_read_wait(const char *id_p,
                               void *buf_p,
                               size_t size,
                               struct time_t *timeout_p)
{
    ASSERTN(id_p != NULL, EINVAL);

    struct event_t event;
    struct bus_listener_t listener;
    ssize_t res;
    uint32_t mask;
    struct mock_entry_t *entry_p;

    res = 0;

    event_init(&event);
    bus_listener_init(&listener, 0, &event);
    bus_attach(&module.bus, &listener);

    while (1) {
        entry_p = find_mock_entry(id_p);

        if (entry_p != NULL) {
            res = read_mock_entry(entry_p,
                                  id_p,
                                  buf_p,
                                  size,
                                  "harness_mock_read_wait");
            bus_detach(&module.bus, &listener);
            break;
        }

        mask = 1;
        event_read(&event, &mask, sizeof(mask));
    }

    return (res);
}

int harness_set_testcase_result(int result)
{
    module.current_testcase_result = result;

#if CONFIG_HARNESS_EARLY_EXIT == 1
    if (result == -1) {
        module.failed++;
        print_report_and_stop();
    }
#endif

    return (0);
}

int harness_get_testcase_result(void)
{
    return (module.current_testcase_result);
}
