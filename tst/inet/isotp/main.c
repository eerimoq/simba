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

static int test_input_single_frame(struct harness_t *harness_p)
{
    struct isotp_t isotp;
    uint8_t buf[8];
    uint8_t frame[8];

    BTASSERT(isotp_init(&isotp, &buf[0], sizeof(buf), 0) == 0);

    /* Input a good frame. */
    frame[0] = (0 << 4) | 3;
    frame[1] = 'f';
    frame[2] = 'o';
    frame[3] = 'o';

    BTASSERT(isotp_input(&isotp, &frame[0], 4) == 3);
    BTASSERT(memcmp(&buf[0], "foo", 3) == 0);

    return (0);
}

static int test_input_multi_frame(struct harness_t *harness_p)
{
    struct isotp_t isotp;
    uint8_t buf[32];
    uint8_t frame[8];
    size_t size;

    BTASSERT(isotp_init(&isotp, &buf[0], sizeof(buf), 0) == 0);

    /* Input the first frame. */
    frame[0] = (1 << 4) | 0;
    frame[1] = 19;
    frame[2] = '1';
    frame[3] = '2';
    frame[4] = '3';
    frame[5] = '4';
    frame[6] = '5';
    frame[7] = '6';

    BTASSERT(isotp_input(&isotp, &frame[0], 8) == 0);

    /* Verify that the flow control frame is created. */
    BTASSERT(isotp_output(&isotp, &frame[0], &size) == 0);

    BTASSERT(size == 3);
    BTASSERT(frame[0] == (3 << 4));
    BTASSERT(frame[1] == 0);
    BTASSERT(frame[2] == 0);

    /* No data to output. */
    BTASSERT(isotp_output(&isotp, &frame[0], &size) == 0);
    BTASSERT(size == 0);

    /* Input two consecutive frames. */
    frame[0] = (2 << 4) | 1;
    frame[1] = '7';
    frame[2] = '8';
    frame[3] = '9';
    frame[4] = '0';
    frame[5] = 'a';
    frame[6] = 'b';
    frame[7] = 'c';

    BTASSERT(isotp_input(&isotp, &frame[0], 8) == 0);

    /* No data to output. */
    BTASSERT(isotp_output(&isotp, &frame[0], &size) == 0);
    BTASSERT(size == 0);

    frame[0] = (2 << 4) | 2;
    frame[1] = 'd';
    frame[2] = 'e';
    frame[3] = 'f';
    frame[4] = 'g';
    frame[5] = 'h';
    frame[6] = 'i';

    /* Message completly received. */
    BTASSERT(isotp_input(&isotp, &frame[0], 7) == 19);

    BTASSERT(memcmp(&buf[0], "1234567890abcdefghi", 19) == 0);

    return (0);
}

static int test_output_single_frame(struct harness_t *harness_p)
{
    struct isotp_t isotp;
    uint8_t message[8];
    uint8_t frame[8];
    size_t size;

    /* Message to transmit. */
    message[0] = 'f';
    message[1] = 'o';
    message[2] = 'o';

    BTASSERT(isotp_init(&isotp, &message[0], 3, 0) == 0);
    BTASSERT(isotp_output(&isotp, &frame[0], &size) == 3);

    BTASSERT(frame[0] == ((0 << 4) | 3));
    BTASSERT(frame[1] == 'f');
    BTASSERT(frame[2] == 'o');
    BTASSERT(frame[3] == 'o');

    return (0);
}

static int test_output_multi_frame(struct harness_t *harness_p)
{
    struct isotp_t isotp;
    char message[] = "1234567890abcdefghi";
    uint8_t frame[8];
    size_t size;

    /* Send a message. */
    BTASSERT(isotp_init(&isotp, (uint8_t *)&message[0], 19, 0) == 0);
    BTASSERT(isotp_output(&isotp, &frame[0], &size) == 0);

    /* Verify outputted first frame. */
    BTASSERT(size == 8);
    BTASSERT(frame[0] == ((1 << 4) | 0));
    BTASSERT(frame[1] == 19);
    BTASSERT(frame[2] == '1');
    BTASSERT(frame[3] == '2');
    BTASSERT(frame[4] == '3');
    BTASSERT(frame[5] == '4');
    BTASSERT(frame[6] == '5');
    BTASSERT(frame[7] == '6');

    /* Input a flow control frame. */
    frame[0] = (3 << 4);
    frame[1] = 0;
    frame[2] = 0;

    BTASSERT(isotp_input(&isotp, &frame[0], 3) == 0);
    BTASSERT(isotp_output(&isotp, &frame[0], &size) == 0);

    /* Verify the outputted consecutive frame. */
    BTASSERT(frame[0] == ((2 << 4) | 1));
    BTASSERT(frame[1] == '7');
    BTASSERT(frame[2] == '8');
    BTASSERT(frame[3] == '9');
    BTASSERT(frame[4] == '0');
    BTASSERT(frame[5] == 'a');
    BTASSERT(frame[6] == 'b');
    BTASSERT(frame[7] == 'c');

    /* Input another flow control frame. */
    frame[0] = (3 << 4);
    frame[1] = 0;
    frame[2] = 0;

    BTASSERT(isotp_input(&isotp, &frame[0], 3) == 0);

    /* Message complete. */
    BTASSERT(isotp_output(&isotp, &frame[0], &size) == 19);

    /* Verify the outputted final consecutive frame. */
    BTASSERT(frame[0] == ((2 << 4) | 2));
    BTASSERT(frame[1] == 'd');
    BTASSERT(frame[2] == 'e');
    BTASSERT(frame[3] == 'f');
    BTASSERT(frame[4] == 'g');
    BTASSERT(frame[5] == 'h');
    BTASSERT(frame[6] == 'i');

    return (0);
}

static int test_input_single_frame_excessive_data(struct harness_t *harness_p)
{
    struct isotp_t isotp;
    uint8_t buf[8];
    uint8_t frame[8];

    BTASSERT(isotp_init(&isotp, &buf[0], sizeof(buf), 0) == 0);

    /* Input a good frame. */
    frame[0] = (0 << 4) | 3;
    frame[1] = 'f';
    frame[2] = 'o';
    frame[3] = 'o';

    /* Input 8 bytes, that is 5 excessive bytes. */
    BTASSERT(isotp_input(&isotp, &frame[0], 8) == 3);
    BTASSERT(memcmp(&buf[0], "foo", 3) == 0);

    return (0);
}

static int test_input_multi_frame_excessive_data(struct harness_t *harness_p)
{
    struct isotp_t isotp;
    uint8_t buf[32];
    uint8_t frame[8];
    size_t size;

    BTASSERT(isotp_init(&isotp, &buf[0], sizeof(buf), 0) == 0);

    /* Input the first frame. */
    frame[0] = (1 << 4) | 0;
    frame[1] = 19;
    frame[2] = '1';
    frame[3] = '2';
    frame[4] = '3';
    frame[5] = '4';
    frame[6] = '5';
    frame[7] = '6';

    BTASSERT(isotp_input(&isotp, &frame[0], 8) == 0);

    /* Verify that the flow control frame was created. */
    BTASSERT(isotp_output(&isotp, &frame[0], &size) == 0);
    BTASSERT(size == 3);
    BTASSERT(frame[0] == (3 << 4));
    BTASSERT(frame[1] == 0);
    BTASSERT(frame[2] == 0);

    /* No data to output. */
    BTASSERT(isotp_output(&isotp, &frame[0], &size) == 0);
    BTASSERT(size == 0);

    /* Input two consecutive frames. */
    frame[0] = (2 << 4) | 1;
    frame[1] = '7';
    frame[2] = '8';
    frame[3] = '9';
    frame[4] = '0';
    frame[5] = 'a';
    frame[6] = 'b';
    frame[7] = 'c';

    BTASSERT(isotp_input(&isotp, &frame[0], 8) == 0);

    frame[0] = (2 << 4) | 2;
    frame[1] = 'd';
    frame[2] = 'e';
    frame[3] = 'f';
    frame[4] = 'g';
    frame[5] = 'h';
    frame[6] = 'i';

    /* Message completly received, with one excessive byte. */
    BTASSERT(isotp_input(&isotp, &frame[0], 8) == 19);

    BTASSERT(memcmp(&buf[0], "1234567890abcdefghi", 19) == 0);

    return (0);
}

static int test_input_single_frame_too_long(struct harness_t *harness_p)
{
    struct isotp_t isotp;
    uint8_t buf[8];
    uint8_t frame[8];

    BTASSERT(isotp_init(&isotp, &buf[0], sizeof(buf), 0) == 0);

    /* Length field has too high value. */
    frame[0] = (0 << 4) | 8;

    BTASSERT(isotp_input(&isotp, &frame[0], 4) == -1);

    return (0);
}

static int test_input_single_frame_no_data(struct harness_t *harness_p)
{
    struct isotp_t isotp;
    uint8_t buf[32];
    uint8_t frame[8];

    BTASSERT(isotp_init(&isotp, &buf[0], sizeof(buf), 0) == 0);

    /* Input a single frame without data. */
    frame[0] = (0 << 4) | 0;

    BTASSERT(isotp_input(&isotp, &frame[0], 1) == -1);

    return (0);
}

static int test_input_first_frame_no_data(struct harness_t *harness_p)
{
    struct isotp_t isotp;
    uint8_t buf[32];
    uint8_t frame[8];

    BTASSERT(isotp_init(&isotp, &buf[0], sizeof(buf), 0) == 0);

    /* Input a first frame without data. */
    frame[0] = (1 << 4) | 0;
    frame[1] = 0;

    BTASSERT(isotp_input(&isotp, &frame[0], 2) == -1);

    return (0);
}

static int test_input_unexpected_consecutive_frame(struct harness_t *harness_p)
{
    struct isotp_t isotp;
    uint8_t buf[32];
    uint8_t frame[8];

    BTASSERT(isotp_init(&isotp, &buf[0], sizeof(buf), 0) == 0);

    /* Input a consecutive frame. */
    frame[0] = (2 << 4) | 1;

    BTASSERT(isotp_input(&isotp, &frame[0], 1) == -1);

    return (0);
}

static int test_input_unexpected_flow_control_frame(struct harness_t *harness_p)
{
    struct isotp_t isotp;
    uint8_t buf[32];
    uint8_t frame[8];

    BTASSERT(isotp_init(&isotp, &buf[0], sizeof(buf), 0) == 0);

    /* Input a flow control frame. */
    frame[0] = (3 << 4) | 0;

    BTASSERT(isotp_input(&isotp, &frame[0], 1) == -1);

    return (0);
}

static int test_input_bad_multi_frame_consecutive(struct harness_t *harness_p)
{
    struct isotp_t isotp;
    uint8_t buf[32];
    uint8_t frame[8];
    size_t size;

    BTASSERT(isotp_init(&isotp, &buf[0], sizeof(buf), 0) == 0);

    /* Input the first frame. */
    frame[0] = (1 << 4) | 0;
    frame[1] = 19;
    frame[2] = '1';
    frame[3] = '2';
    frame[4] = '3';
    frame[5] = '4';
    frame[6] = '5';
    frame[7] = '6';

    BTASSERT(isotp_input(&isotp, &frame[0], 8) == 0);

    /* Verify that the flow control frame is created. */
    BTASSERT(isotp_output(&isotp, &frame[0], &size) == 0);

    BTASSERT(size == 3);
    BTASSERT(frame[0] == (3 << 4));
    BTASSERT(frame[1] == 0);
    BTASSERT(frame[2] == 0);

    /* No data to output. */
    BTASSERT(isotp_output(&isotp, &frame[0], &size) == 0);
    BTASSERT(size == 0);

    /* Input an unexpected first frame, waiting for a consecutive
       frame. */
    frame[0] = (1 << 4) | 0;
    frame[1] = 19;
    frame[2] = '1';
    frame[3] = '2';
    frame[4] = '3';
    frame[5] = '4';
    frame[6] = '5';
    frame[7] = '6';

    BTASSERT(isotp_input(&isotp, &frame[0], 8) == -1);

    return (0);
}

static int test_output_multi_frame_unexpected_non_flow_control(
    struct harness_t *harness_p)
{
    struct isotp_t isotp;
    char message[] = "1234567890abcdefghi";
    uint8_t frame[8];
    size_t size;

    /* Send a message. */
    BTASSERT(isotp_init(&isotp, (uint8_t *)&message[0], 19, 0) == 0);
    BTASSERT(isotp_output(&isotp, &frame[0], &size) == 0);

    /* Verify outputted first frame. */
    BTASSERT(size == 8);
    BTASSERT(frame[0] == ((1 << 4) | 0));
    BTASSERT(frame[1] == 19);
    BTASSERT(frame[2] == '1');
    BTASSERT(frame[3] == '2');
    BTASSERT(frame[4] == '3');
    BTASSERT(frame[5] == '4');
    BTASSERT(frame[6] == '5');
    BTASSERT(frame[7] == '6');

    /* Input a flow control frame. */
    frame[0] = (3 << 4);
    frame[1] = 0;
    frame[2] = 0;

    BTASSERT(isotp_input(&isotp, &frame[0], 3) == 0);
    BTASSERT(isotp_output(&isotp, &frame[0], &size) == 0);

    /* Verify the outputted consecutive frame. */
    BTASSERT(frame[0] == ((2 << 4) | 1));
    BTASSERT(frame[1] == '7');
    BTASSERT(frame[2] == '8');
    BTASSERT(frame[3] == '9');
    BTASSERT(frame[4] == '0');
    BTASSERT(frame[5] == 'a');
    BTASSERT(frame[6] == 'b');
    BTASSERT(frame[7] == 'c');

    /* Input an unexpected non-flow control frame, in this case a
       single frame . */
    frame[0] = (0 << 4) | 1;
    frame[1] = '1';

    BTASSERT(isotp_input(&isotp, &frame[0], 2) == -1);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t testcases[] = {
        { test_input_single_frame, "test_input_single_frame" },
        { test_input_multi_frame, "test_input_multi_frame" },
        { test_output_single_frame, "test_output_single_frame" },
        { test_output_multi_frame, "test_output_multi_frame" },
        { test_input_single_frame_excessive_data,
          "test_input_single_frame_excessive_data" },
        { test_input_multi_frame_excessive_data,
          "test_input_multi_frame_excessive_data" },
        { test_input_single_frame_too_long,
          "test_input_single_frame_too_long" },
        { test_input_single_frame_no_data,
          "test_input_single_frame_no_data" },
        { test_input_first_frame_no_data,
          "test_input_first_frame_no_data" },
        { test_input_unexpected_consecutive_frame,
          "test_input_unexpected_consecutive_frame" },
        { test_input_unexpected_flow_control_frame,
          "test_input_unexpected_flow_control_frame" },
        { test_input_bad_multi_frame_consecutive,
          "test_input_bad_multi_frame_consecutive" },
        { test_output_multi_frame_unexpected_non_flow_control,
          "test_output_multi_frame_unexpected_non_flow_control" },
        { NULL, NULL }
    };

    sys_start();

    harness_init(&harness);
    harness_run(&harness, testcases);

    return (0);
}
