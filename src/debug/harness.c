/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2017, Erik Moqvist
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

struct mock_entry_t {
    struct entry_t *next_p;
    const char *id_p;
    struct {
        void *array[2 * CONFIG_HARNESS_BACKTRACE_DEPTH_MAX];
        size_t depth;
    } backtrace;
    struct {
        size_t size;
        uint8_t buf[1];
    } data;
};

struct module_t {
    struct {
        struct heap_t obj;
        uint8_t buf[CONFIG_HARNESS_HEAP_MAX];
    } heap;
    struct {
        struct mock_entry_t *head_p;
        struct mock_entry_t *tail_p;
    } mock_list;
    struct mutex_t mutex;
    struct bus_t bus;
    int current_testcase_failed;
};

static struct module_t module;

static int print_backtrace(void *array[], int depth, const char *name_p)
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

static int print_assert_backtrace(void)
{
    void *array[2 * CONFIG_HARNESS_BACKTRACE_DEPTH_MAX];
    int depth;

    depth = sys_backtrace(array, sizeof(array));

    return (print_backtrace(array, depth, "assert"));
}

static int print_read_backtrace(void)
{
    void *array[2 * CONFIG_HARNESS_BACKTRACE_DEPTH_MAX];
    int depth;

    depth = sys_backtrace(array, sizeof(array));

    return (print_backtrace(array, depth, "read"));
}

static int create_write_backtrace(struct mock_entry_t *entry_p)
{
    entry_p->backtrace.depth = sys_backtrace(entry_p->backtrace.array,
                                             sizeof(entry_p->backtrace.array));

    return (0);
}

static int print_write_backtrace(struct mock_entry_t *entry_p)
{
    return (print_backtrace(entry_p->backtrace.array,
                            entry_p->backtrace.depth,
                            "write"));
}

static struct mock_entry_t *find_mock_entry(const char *id_p)
{
    struct list_sl_iterator_t iterator;
    struct mock_entry_t *entry_p;
    struct mock_entry_t *iterator_entry_p;
    struct mock_entry_t *prev_entry_p;

    mutex_lock(&module.mutex);

    LIST_SL_ITERATOR_INIT(&iterator, &module.mock_list);

    while (1) {
        LIST_SL_ITERATOR_NEXT(&iterator, &entry_p);

        if (entry_p == NULL) {
            break;
        }

        if (strcmp(entry_p->id_p, id_p) == 0) {
            LIST_SL_REMOVE_ELEM(&module.mock_list,
                                &iterator,
                                entry_p,
                                iterator_entry_p,
                                prev_entry_p);
            break;
        }
    }

    mutex_unlock(&module.mutex);

    return (entry_p);
}

int harness_run(struct harness_testcase_t *testcases_p)
{
    int err;
    struct harness_testcase_t *testcase_p;
    int total, passed, failed, skipped;
    struct mock_entry_t *entry_p;
    size_t sizes[HEAP_FIXED_SIZES_MAX] = {
        8, 16, 32, 32, 32, 32, 32, 32
    };

    mutex_init(&module.mutex);

    total = 0;
    passed = 0;
    failed = 0;
    skipped = 0;
    testcase_p = testcases_p;

#if !defined(ARCH_LINUX)
    thrd_sleep_ms(CONFIG_HARNESS_SLEEP_MS);
#endif

    /* Print a header. */
    std_printf(OSTR("\r\n"));

    std_printf(OSTR("================================== TEST BEGIN =========="
                    "========================\r\n\r\n"));
    std_printf(sys_get_info());
    std_printf(OSTR("\r\n"));

    while (testcase_p->callback != NULL) {
        /* Reinitialize the heap before every testcase for minimal
           memory usage. */
        LIST_SL_INIT(&module.mock_list);

        heap_init(&module.heap.obj,
                  &module.heap.buf[0],
                  sizeof(module.heap.buf),
                  &sizes[0]);

        /* Mark current testcase as non-failed before its executed. */
        module.current_testcase_failed = 0;

        std_printf(OSTR("enter: %s\r\n"), testcase_p->name_p);

        err = testcase_p->callback();

        do {
            LIST_SL_REMOVE_HEAD(&module.mock_list, &entry_p);

            if (entry_p != NULL) {
                std_printf(OSTR("Found unread mock id '%s'. Failing test.\r\n"),
                           entry_p->id_p);
                err = -1;
            }
        } while (entry_p != NULL);

        if ((err < 0) || (module.current_testcase_failed != 0)) {
            failed++;
            std_printf(OSTR("exit: %s: FAILED\r\n\r\n"),
                       testcase_p->name_p);
        } else if (err == 0) {
            passed++;
            std_printf(OSTR("exit: %s: PASSED\r\n\r\n"),
                       testcase_p->name_p);
        } else {
            skipped++;
            std_printf(OSTR("exit: %s: SKIPPED\r\n\r\n"),
                       testcase_p->name_p);
        }

        total++;
        testcase_p++;
    }

#if CONFIG_THRD_FS_COMMANDS == 1
    char buf[18];

    std_strcpy(buf, FSTR("/kernel/thrd/list"));
    fs_call(buf, NULL, sys_get_stdout(), NULL);

    std_printf(OSTR("\r\n"));
#endif

    std_printf(OSTR("harness report: total(%d), passed(%d), "
                    "failed(%d), skipped(%d)\r\n\r\n"),
               total, passed, failed, skipped);

    std_printf(OSTR("=============================== TEST END (%s) =========="
                    "====================\r\n\r\n"),
               ((passed + skipped) == total ? "PASSED" : "FAILED"));

    sys_stop(failed);

    return (0);
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

        std_printf(FSTR("%c"), c);

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

    struct mock_entry_t *entry_p;

    /* Allocate memory for the mock entry and data. */
    mutex_lock(&module.mutex);
    entry_p = heap_alloc(&module.heap.obj, sizeof(*entry_p) + size - 1);
    mutex_unlock(&module.mutex);

    if (entry_p == NULL) {
        std_printf(FSTR("error: %s: mock id memory allocation failed\r\n"),
                   id_p);

        return (-ENOMEM);
    }

    /* Initiate the object. */
    entry_p->id_p = id_p;

    if (buf_p != NULL) {
        memcpy(&entry_p->data.buf[0], buf_p, size);
    }

    entry_p->data.size = size;
    create_write_backtrace(entry_p);

    /* Add the entry at the end of the list. */
    mutex_lock(&module.mutex);
    LIST_SL_ADD_TAIL(&module.mock_list, entry_p);
    mutex_unlock(&module.mutex);

    return (size);
}

ssize_t harness_mock_read(const char *id_p,
                          void *buf_p,
                          ssize_t size)
{
    ssize_t res;

    res = harness_mock_try_read(id_p, buf_p, size);

    /* Mark testcase as failed if the mock id was not found. */
    if (res == -1) {
        std_printf(FSTR("error: %s: mock id not found\r\n"), id_p);
        print_read_backtrace();
        module.current_testcase_failed = 1;
    }

    return (res);
}

ssize_t harness_mock_try_read(const char *id_p,
                              void *buf_p,
                              ssize_t size)
{
    ASSERTN(id_p != NULL, EINVAL);

    struct mock_entry_t *entry_p;
    ssize_t res;

    res = -1;
    entry_p = find_mock_entry(id_p);

    if (entry_p != NULL) {
        /* Copy the value to the output buffer. */
        if (size == HARNESS_MOCK_READ_ALL) {
            size = entry_p->data.size;
        }

        if (buf_p != NULL) {
            memcpy(buf_p,
                   &entry_p->data.buf[0],
                   size);
        }

        res = size;

        /* Free allocated memory. */
        mutex_lock(&module.mutex);
        heap_free(&module.heap.obj, entry_p);
        mutex_unlock(&module.mutex);
    }

    return (res);
}

int harness_mock_assert(const char *id_p,
                        const void *buf_p)
{
    ASSERTN(id_p != NULL, EINVAL);

    struct mock_entry_t *entry_p;
    int res;

    res = 0;
    entry_p = find_mock_entry(id_p);

    if (entry_p != NULL) {
        /* Compare the value to the expected value. */
        if (buf_p != NULL) {
            res = memcmp(buf_p, &entry_p->data.buf[0], entry_p->data.size);

            if (res != 0) {
                std_printf(FSTR("harness_mock_assert: %s: data mismatch "),
                           id_p);
                _ASSERTHEX("actual",
                           buf_p,
                           "expected",
                           &entry_p->data.buf[0],
                           entry_p->data.size);
                print_assert_backtrace();
                print_write_backtrace(entry_p);
                res = -1;
            }
        }

        /* Free allocated memory. */
        mutex_lock(&module.mutex);
        heap_free(&module.heap.obj, entry_p);
        mutex_unlock(&module.mutex);
    } else {
        std_printf(FSTR("error: %s: mock id not found\r\n"), id_p);
        print_assert_backtrace();
        res = -1;
    }

    if (res != 0) {
        module.current_testcase_failed = 1;
    }

    return (res);
}

ssize_t harness_mock_write_notify(const char *id_p,
                                  const void *buf_p,
                                  size_t size)
{
    ASSERTN(id_p != NULL, EINVAL);

    uint32_t mask;

    harness_mock_write(id_p, buf_p, size);
    mask = 1;
    bus_write(&module.bus, 0, &mask, sizeof(mask));

    return (size);
}

ssize_t harness_mock_read_wait(const char *id_p,
                               void *buf_p,
                               ssize_t size,
                               struct time_t *timeout_p)
{
    ASSERTN(id_p != NULL, EINVAL);

    struct event_t event;
    struct bus_listener_t listener;
    ssize_t res;
    uint32_t mask;

    event_init(&event);
    bus_listener_init(&listener, 0, &event);
    bus_attach(&module.bus, &listener);

    while (1) {
        res = harness_mock_try_read(id_p, buf_p, size);

        if (res != -1) {
            bus_detatch(&module.bus, &listener);
            break;
        }

        mask = 1;
        event_read(&event, &mask, sizeof(mask));
    }

    return (size);
}
