/**
 * @section License
 *
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org/>
 *
 * This file is part of the Simba project.
 */

#include "simba.h"

static struct queue_t queue[2];

static THRD_STACK(t0_stack, 512);

static void *t0_main(void *arg_p)
{
    int b, c[4];

    thrd_set_name("t0");

    /* Test: test_read_write. */
    b = 1;
    BTASSERTN(chan_write(&queue[0], &b, sizeof(b)) == sizeof(b), NULL);
    BTASSERTN(b == 1, NULL);
    b = 2;
    BTASSERTN(chan_write(&queue[0], &b, sizeof(b)) == sizeof(b), NULL);
    BTASSERTN(b == 2, NULL);
    b = 3;
    BTASSERTN(chan_write(&queue[0], &b, sizeof(b)) == sizeof(b), NULL);
    BTASSERTN(b == 3, NULL);
    c[0] = 4;
    c[1] = 5;
    c[2] = 6;
    c[3] = 7;
    BTASSERTN(chan_write(&queue[0], c, sizeof(c)) == sizeof(c), NULL);
    BTASSERTN(c[0] == 4, NULL);
    BTASSERTN(c[1] == 5, NULL);
    BTASSERTN(c[2] == 6, NULL);
    BTASSERTN(c[3] == 7, NULL);
    BTASSERTN(chan_write(&queue[0], c, sizeof(c)) == sizeof(c), NULL);
    BTASSERTN(c[0] == 4, NULL);
    BTASSERTN(c[1] == 5, NULL);
    BTASSERTN(c[2] == 6, NULL);
    BTASSERTN(c[3] == 7, NULL);

    thrd_sleep_us(50000);

    /* Test: test_poll. */
    c[0] = 8;
    c[1] = 9;
    c[2] = 10;
    c[3] = 11;
    BTASSERTN(chan_write(&queue[1], c, sizeof(c)) == sizeof(c), NULL);

    /* Write to chan that was polled but not read.*/
    b = 12;
    BTASSERTN(chan_write(&queue[0], &b, sizeof(b)) == sizeof(b), NULL);

    /* Test: test_size. */
    b = 0;
    BTASSERTN(chan_read(&queue[0], &b, sizeof(b)) == sizeof(b), NULL);
    BTASSERTN(b == 1, NULL);

    /* Test: test_stopped. */

    /* Read one of the two integers the other thread writes. Tests
       resuming the writer when a queue is stopped. */
    BTASSERTN(chan_read(&queue[0], &b, sizeof(b)) == sizeof(b), NULL);
    BTASSERTN(b == 2, NULL);
    BTASSERTN(chan_size(&queue[0]) == sizeof(b), NULL);
    BTASSERTN(queue_stop(&queue[0]) == 1, NULL);

    /* Tests resuming a reader thread when stopping a queue. */
    b = 13;
    BTASSERTN(chan_write(&queue[1], &b, sizeof(b)) == sizeof(b), NULL);
    BTASSERTN(queue_stop(&queue[1]) == 1, NULL);

    thrd_suspend(NULL);

    return (0);
}

static int test_init(struct harness_t *harness)
{
    struct thrd_t *t0;

    BTASSERT(queue_init(&queue[0], NULL, 0) == 0);
    BTASSERT(queue_init(&queue[1], NULL, 0) == 0);

    BTASSERT((t0 = thrd_spawn(t0_main,
                              NULL,
                              1,
                              t0_stack,
                              sizeof(t0_stack))) != NULL);

    return (0);
}

static int test_read_write(struct harness_t *harness)
{
    int b, c[4];

    b = 0;
    BTASSERT(chan_read(&queue[0], &b, sizeof(b)) == sizeof(b));
    BTASSERT(b == 1);
    BTASSERT(chan_read(&queue[0], &b, sizeof(b)) == sizeof(b));
    BTASSERT(b == 2);
    BTASSERT(chan_read(&queue[0], &b, sizeof(b)) == sizeof(b));
    BTASSERT(b == 3);
    BTASSERT(chan_read(&queue[0], c, sizeof(c)) == sizeof(c));
    BTASSERT(c[0] == 4);
    BTASSERT(c[1] == 5);
    BTASSERT(c[2] == 6);
    BTASSERT(c[3] == 7);
    BTASSERT(chan_read(&queue[0], c, sizeof(c)) == sizeof(c));
    BTASSERT(c[0] == 4);
    BTASSERT(c[1] == 5);
    BTASSERT(c[2] == 6);
    BTASSERT(c[3] == 7);

    return (0);
}

static int test_poll(struct harness_t *harness_p)
{
    int b;
    struct chan_list_t list;
    char workspace[64];

    /* Use a list with one chan.*/
    BTASSERT(chan_list_init(&list, workspace, sizeof(workspace)) == 0);

    BTASSERT(chan_list_add(&list, &queue[0]) == 0);
    BTASSERT(chan_list_add(&list, &queue[1]) == 0);

    BTASSERT(chan_list_poll(&list, NULL) == &queue[1]);
    BTASSERT(chan_read(&queue[1], &b, sizeof(b)) == sizeof(b));
    BTASSERT(b == 8);

    BTASSERT(chan_list_poll(&list, NULL) == &queue[1]);
    BTASSERT(chan_read(&queue[1], &b, sizeof(b)) == sizeof(b));
    BTASSERT(b == 9);

    BTASSERT(chan_list_poll(&list, NULL) == &queue[1]);
    BTASSERT(chan_read(&queue[1], &b, sizeof(b)) == sizeof(b));
    BTASSERT(b == 10);

    BTASSERT(chan_list_poll(&list, NULL) == &queue[1]);
    BTASSERT(chan_read(&queue[1], &b, sizeof(b)) == sizeof(b));
    BTASSERT(b == 11);

    BTASSERT(chan_list_destroy(&list) == 0);

    /* Read from inactive polled chan.*/
    BTASSERT(chan_read(&queue[0], &b, sizeof(b)) == sizeof(b));
    BTASSERT(b == 12);

    return (0);
}

static int test_size(struct harness_t *harness_p)
{
    int b;
    struct queue_t foo;
    uint8_t buf[16];

    BTASSERT(queue_init(&foo, buf, sizeof(buf)) == 0);

    BTASSERT(queue_size(&foo) == 0);
    BTASSERT(queue_unused_size(&foo) == 15);

    b = 6;
    BTASSERT(queue_write(&foo, &b, sizeof(b)));

    BTASSERT(queue_size(&foo) == sizeof(b));
    BTASSERT(queue_unused_size(&foo) == (15 - sizeof(b)));

    while (queue_unused_size(&queue[0]) != sizeof(int)) {
        thrd_sleep_us(100);
    }

    b = 1;
    BTASSERT(chan_write(&queue[0], &b, sizeof(b)) == sizeof(b));
    BTASSERT(b == 1);

    return (0);
}

static int test_stopped(struct harness_t *harness_p)
{
    int a[2];

    /* Write two integers, the other thread will read one and then
       stop the queue to test the behaviour of stopping a thread with
       active writer. */
    a[0] = 2;
    a[1] = 1;
    BTASSERT(queue_write(&queue[0], &a[0], sizeof(a)) == sizeof(int));

    /* The read function returns sizeof(int) because at least
       sizeof(int) bytes were written to the queue before it was
       stopped by the other thread. */
    BTASSERT(queue_read(&queue[1], a, sizeof(a)) == sizeof(int));
    BTASSERT(a[0] == 13);

    /* The read function returns 0 because the queue is stopped and no
       data is available. */
    BTASSERT(queue_read(&queue[1], a, sizeof(a)) == 0);

    /* Write to a stopped queue. */
    BTASSERT(chan_write(&queue[1], &a[0], sizeof(a)) == -1);

    BTASSERT(queue_start(&queue[1]) == 0)

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_init, "test_init" },
        { test_read_write, "test_read_write" },
        { test_poll, "test_poll" },
        { test_size, "test_size" },
        { test_stopped, "test_stopped" },
        { NULL, NULL }
    };

    sys_start();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
