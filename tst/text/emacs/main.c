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

static struct queue_t emacs_input;
static struct queue_t emacs_output;
static char input_buffer[2048];
static char output_buffer[2048];

static void print_ascii(char *ascii_p, int length)
{
    int i;

    for (i = 0; i < 16 - length; i++) {
        std_printf(FSTR("   "));
    }

    std_printf(FSTR("'"));

    for (i = 0; i < length; i++) {
        std_printf(FSTR("%c"), isprint((int)ascii_p[i]) ? ascii_p[i] : '.');
    }

    std_printf(FSTR("'"));
}

/**
 * Compare the emacs output to the expected output.
 */
static int check_emacs_output(const char *expected_output_p)
{
    char c;
    char buf[3];
    int pos;
    int failed_pos = -1;
    char ascii[16];

    std_printf(FSTR("emacs output size: %d\r\n"), queue_size(&emacs_output));

    pos = 0;

    while (queue_size(&emacs_output) > 0) {
        queue_read(&emacs_output, &c, sizeof(c));
        std_sprintf(&buf[0], FSTR("%02x"), c);

        ascii[pos % 16] = c;

        if ((pos % 16) == 0) {
            std_printf(FSTR("%4d: "), pos);
        }

        std_printf(FSTR("%s "), &buf[0]);

        if ((pos % 16) == 15) {
            print_ascii(&ascii[0], 16);
            std_printf(FSTR("\r\n"));
        }

        if (strncmp(&buf[0], &expected_output_p[2 * pos], 2) != 0) {
            if (failed_pos == -1) {
                failed_pos = pos;
            }
        }

        pos++;
    }

    print_ascii(&ascii[0], pos % 16);
    std_printf(FSTR("\r\n"));

    if (failed_pos == -1) {
        return (0);
    } else {
        std_printf(FSTR("failed position: %d\r\n"), failed_pos);

        return (-1);
    }
}

static int test_setup(struct harness_t *harness_p)
{
    queue_init(&emacs_input, input_buffer, sizeof(input_buffer));
    queue_init(&emacs_output, output_buffer, sizeof(output_buffer));

    return (0);
}

static int test_start_quit(struct harness_t *harness_p)
{
    static const char expected_output[] =
        "0d1b5b4b0d0a1b5b4b0d0a1b5b4b0d0a"
        "1b5b4b0d0a1b5b4b0d0a1b5b4b0d0a1b"
        "5b4b0d0a1b5b4b0d0a1b5b4b0d0a1b5b"
        "4b0d0a1b5b4b0d0a1b5b4b0d0a1b5b4b"
        "0d0a1b5b4b0d0a1b5b4b0d0a1b5b4b0d"
        "0a1b5b4b0d0a1b5b4b0d0a1b5b4b0d0a"
        "1b5b4b0d0a1b5b4b0d0a1b5b4b0d0a1b"
        "5b4b0d0a1b5b4b1b5b3233411b371b5b"
        "3f32356c1b381b5b3232423d3d3d2041"
        "74746f3a203d3d202a73637261746368"
        "2a3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d"
        "3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d"
        "3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d"
        "3d3d3d3d3d3d3d3d3d3d3d1b5b4b1b38"
        "1b5b3f3235681b5b3f32356c1b381b5b"
        "3f3235681b5b3f32356c1b381b5b3233"
        "421b5b3f3235681b381b5b4b0d0a1b5b"
        "4b0d0a1b5b4b0d0a1b5b4b0d0a1b5b4b"
        "0d0a1b5b4b0d0a1b5b4b0d0a1b5b4b0d"
        "0a1b5b4b0d0a1b5b4b0d0a1b5b4b0d0a"
        "1b5b4b0d0a1b5b4b0d0a1b5b4b0d0a1b"
        "5b4b0d0a1b5b4b0d0a1b5b4b0d0a1b5b"
        "4b0d0a1b5b4b0d0a1b5b4b0d0a1b5b4b"
        "0d0a1b5b4b0d0a1b5b4b0d0a1b5b4b1b"
        "38";

    BTASSERT(queue_write(&emacs_input, "\x18\x03", 2) == 2);
    BTASSERT(emacs(NULL, &emacs_input, &emacs_output) == 0);
    BTASSERT(check_emacs_output(&expected_output[0]) == 0);

    return (0);
}

static int test_start_space_quit(struct harness_t *harness_p)
{
    static const char expected_output[] =
        "0d1b5b4b0d0a1b5b4b0d0a1b5b4b0d0a"
        "1b5b4b0d0a1b5b4b0d0a1b5b4b0d0a1b"
        "5b4b0d0a1b5b4b0d0a1b5b4b0d0a1b5b"
        "4b0d0a1b5b4b0d0a1b5b4b0d0a1b5b4b"
        "0d0a1b5b4b0d0a1b5b4b0d0a1b5b4b0d"
        "0a1b5b4b0d0a1b5b4b0d0a1b5b4b0d0a"
        "1b5b4b0d0a1b5b4b0d0a1b5b4b0d0a1b"
        "5b4b0d0a1b5b4b1b5b3233411b371b5b"
        "3f32356c1b381b5b3232423d3d3d2041"
        "74746f3a203d3d202a73637261746368"
        "2a3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d"
        "3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d"
        "3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d"
        "3d3d3d3d3d3d3d3d3d3d3d1b5b4b1b38"
        "1b5b3f3235681b5b3f32356c1b381b5b"
        "3f3235681b5b3f32356c1b38201b5b4b"
        "1b381b5b3232423d3d2a204174746f3a"
        "203d3d202a736372617463682a3d3d3d"
        "3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d"
        "3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d"
        "3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d"
        "3d3d3d3d3d3d3d1b5b4b1b381b5b3143"
        "1b5b3f3235681b5b3f32356c1b381b5b"
        "31431b5b3f3235681b5b3f32356c1b38"
        "1b5b3233424d6f646966696564206275"
        "66666572732065786973743b20726561"
        "6c6c7920657869742028792f6e29203f"
        "206e081b5b4b1b381b5b3233421b5b34"
        "36431b5b3f3235681b5b3f32356c1b38"
        "1b5b3233421b5b3f3235681b381b5b4b"
        "0d0a1b5b4b0d0a1b5b4b0d0a1b5b4b0d"
        "0a1b5b4b0d0a1b5b4b0d0a1b5b4b0d0a"
        "1b5b4b0d0a1b5b4b0d0a1b5b4b0d0a1b"
        "5b4b0d0a1b5b4b0d0a1b5b4b0d0a1b5b"
        "4b0d0a1b5b4b0d0a1b5b4b0d0a1b5b4b"
        "0d0a1b5b4b0d0a1b5b4b0d0a1b5b4b0d"
        "0a1b5b4b0d0a1b5b4b0d0a1b5b4b0d0a"
        "1b5b4b1b38";

    BTASSERT(queue_write(&emacs_input, " \x18\x03y", 4) == 4);
    BTASSERT(emacs(NULL, &emacs_input, &emacs_output) == 0);
    BTASSERT(check_emacs_output(&expected_output[0]) == 0);

    return (0);
}

static int test_start_multiline_quit(struct harness_t *harness_p)
{
    static const char expected_output[] =
        "0d1b5b4b0d0a1b5b4b0d0a1b5b4b0d0a"
        "1b5b4b0d0a1b5b4b0d0a1b5b4b0d0a1b"
        "5b4b0d0a1b5b4b0d0a1b5b4b0d0a1b5b"
        "4b0d0a1b5b4b0d0a1b5b4b0d0a1b5b4b"
        "0d0a1b5b4b0d0a1b5b4b0d0a1b5b4b0d"
        "0a1b5b4b0d0a1b5b4b0d0a1b5b4b0d0a"
        "1b5b4b0d0a1b5b4b0d0a1b5b4b0d0a1b"
        "5b4b0d0a1b5b4b1b5b3233411b371b5b"
        "3f32356c1b381b5b3232423d3d3d2041"
        "74746f3a203d3d202a73637261746368"
        "2a3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d"
        "3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d"
        "3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d"
        "3d3d3d3d3d3d3d3d3d3d3d1b5b4b1b38"
        "1b5b3f3235681b5b3f32356c1b381b5b"
        "3f3235681b5b3f32356c1b38661b5b4b"
        "1b381b5b3232423d3d2a204174746f3a"
        "203d3d202a736372617463682a3d3d3d"
        "3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d"
        "3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d"
        "3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d"
        "3d3d3d3d3d3d3d1b5b4b1b381b5b3143"
        "1b5b3f3235681b5b3f32356c1b381b5b"
        "31431b5b3f3235681b5b3f32356c1b38"
        "666f1b5b4b1b381b5b32431b5b3f3235"
        "681b5b3f32356c1b381b5b32431b5b3f"
        "3235681b5b3f32356c1b38666f6f1b5b"
        "4b1b381b5b33431b5b3f3235681b5b3f"
        "32356c1b381b5b33431b5b3f3235681b"
        "5b3f32356c1b38666f6f1b5b4b1b381b"
        "5b31421b5b3f3235681b5b3f32356c1b"
        "381b5b31421b5b3f3235681b5b3f3235"
        "6c1b38666f6f1b5b4b1b381b5b31421b"
        "5b4b1b381b5b32421b5b3f3235681b5b"
        "3f32356c1b381b5b32421b5b3f323568"
        "1b5b3f32356c1b38666f6f1b5b4b1b38"
        "1b5b31421b5b4b1b381b5b3242621b5b"
        "4b1b381b5b32421b5b31431b5b3f3235"
        "681b5b3f32356c1b381b5b32421b5b31"
        "431b5b3f3235681b5b3f32356c1b3866"
        "6f6f1b5b4b1b381b5b31421b5b4b1b38"
        "1b5b324262611b5b4b1b381b5b32421b"
        "5b32431b5b3f3235681b5b3f32356c1b"
        "381b5b32421b5b32431b5b3f3235681b"
        "5b3f32356c1b38666f6f1b5b4b1b381b"
        "5b31421b5b4b1b381b5b32426261721b"
        "5b4b1b381b5b32421b5b33431b5b3f32"
        "35681b5b3f32356c1b381b5b32421b5b"
        "33431b5b3f3235681b5b3f32356c1b38"
        "666f6f1b5b4b1b381b5b31421b5b4b1b"
        "381b5b32426261721b5b4b1b381b5b33"
        "421b5b3f3235681b5b3f32356c1b381b"
        "5b33421b5b3f3235681b5b3f32356c1b"
        "38666f6f1b5b4b1b381b5b31421b5b4b"
        "1b381b5b32426261721b5b4b1b381b5b"
        "33421b5b4b1b381b5b34421b5b3f3235"
        "681b5b3f32356c1b381b5b34421b5b3f"
        "3235681b5b3f32356c1b381b5b323342"
        "4d6f6469666965642062756666657273"
        "2065786973743b207265616c6c792065"
        "7869742028792f6e29203f206e081b5b"
        "4b1b381b5b3233421b5b3436431b5b3f"
        "3235681b5b3f32356c1b381b5b323342"
        "1b5b3f3235681b381b5b4b0d0a1b5b4b"
        "0d0a1b5b4b0d0a1b5b4b0d0a1b5b4b0d"
        "0a1b5b4b0d0a1b5b4b0d0a1b5b4b0d0a"
        "1b5b4b0d0a1b5b4b0d0a1b5b4b0d0a1b"
        "5b4b0d0a1b5b4b0d0a1b5b4b0d0a1b5b"
        "4b0d0a1b5b4b0d0a1b5b4b0d0a1b5b4b"
        "0d0a1b5b4b0d0a1b5b4b0d0a1b5b4b0d"
        "0a1b5b4b0d0a1b5b4b0d0a1b5b4b1b38";

    BTASSERT(queue_write(&emacs_input, "foo\r\nbar\r\n\x18\x03y", 13) == 13);
    BTASSERT(emacs(NULL, &emacs_input, &emacs_output) == 0);
    BTASSERT(check_emacs_output(&expected_output[0]) == 0);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_setup, "test_setup" },
        { test_start_quit, "test_start_quit" },
        { test_start_space_quit, "test_start_space_quit" },
        { test_start_multiline_quit, "test_start_multiline_quit" },
        { NULL, NULL }
    };

    sys_start();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
