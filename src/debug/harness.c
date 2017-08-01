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
    struct sem_t sem;
};

static struct module_t module;

int harness_init(struct harness_t *self_p)
{
    sem_init(&module.sem, 0, 1);

    return (0);
}

int harness_run(struct harness_t *self_p,
                struct harness_testcase_t *testcases_p)
{
    int err;
    struct harness_testcase_t *testcase_p;
    int total, passed, failed, skipped;
    struct mock_entry_t *entry_p;
    size_t sizes[HEAP_FIXED_SIZES_MAX] = {
        8, 16, 32, 32, 32, 32, 32, 32
    };

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

    std_printf(OSTR("================================== TEST BEGIN ==================================\r\n\r\n"));
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

        std_printf(OSTR("enter: %s\r\n"), testcase_p->name_p);

        err = testcase_p->callback(self_p);

        do {
            LIST_SL_REMOVE_HEAD(&module.mock_list, &entry_p);

            if (entry_p != NULL) {
                std_printf(OSTR("Found unread mock id '%s'. Failing test.\r\n"),
                           entry_p->id_p);
                err = -1;
            }
        } while (entry_p != NULL);

        if (err == 0) {
            passed++;
            std_printf(OSTR("exit: %s: PASSED\r\n\r\n"),
                       testcase_p->name_p);
        } else if (err < 0) {
            failed++;
            std_printf(OSTR("exit: %s: FAILED\r\n\r\n"),
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

    std_printf(OSTR("=============================== TEST END (%s) ==============================\r\n\r\n"),
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
    struct mock_entry_t *entry_p;

    /* Allocate memory for the mock entry and data. */
    sem_take(&module.sem, NULL);
    entry_p = heap_alloc(&module.heap.obj, sizeof(*entry_p) + size - 1);
    sem_give(&module.sem, 1);

    if (entry_p == NULL) {
        std_printf(FSTR("error: %s: mock id memory allocation failed\r\n"),
                   id_p);

        return (-ENOMEM);
    }

    /* Initiate the object. */
    entry_p->id_p = id_p;
    memcpy(&entry_p->data.buf[0], buf_p, size);
    entry_p->data.size = size;

    /* Add the entry at the end of the list. */
    LIST_SL_ADD_TAIL(&module.mock_list, entry_p);

    return (size);
}

ssize_t harness_mock_read(const char *id_p,
                          void *buf_p,
                          size_t size)
{
    struct list_sl_iterator_t iterator;
    struct mock_entry_t *entry_p;
    struct mock_entry_t *iterator_entry_p;
    struct mock_entry_t *prev_entry_p;
    ssize_t res;

    res = -1;

    sem_take(&module.sem, NULL);

    LIST_SL_ITERATOR_INIT(&iterator, &module.mock_list);

    while (1) {
        LIST_SL_ITERATOR_NEXT(&iterator, &entry_p);

        if (entry_p == NULL) {
#if CONFIG_HARNESS_MOCK_VERBOSE == 1
            std_printf(FSTR("error: %s: mock id not found\r\n"), id_p);
#endif
            break;
        }

        if (strcmp(entry_p->id_p, id_p) == 0) {
            res = size;
            break;
        }
    }

    if (res == size) {
        /* Remove the element from the list. */
        LIST_SL_REMOVE_ELEM(&module.mock_list,
                            &iterator,
                            entry_p,
                            iterator_entry_p,
                            prev_entry_p);

        /* Copy the value to the output buffer. */
        memcpy(buf_p, &entry_p->data.buf[0], entry_p->data.size);

        /* Free allocated memory. */
        heap_free(&module.heap.obj, entry_p);
    }

    sem_give(&module.sem, 1);

    return (res);
}
