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

/* Memory ranges. */
#define APPLICATION_ADDRESS                      0x00000000
#define APPLICATION_SIZE                         0x20000000

static uint8_t inbuf[128];
static uint8_t outbuf[128];
static char buf[128];

static int test_send_file_kermit(struct harness_t *self_p)
{
    struct queue_t qin;
    struct queue_t qout;

    static char input[] =
        "kermit -ir\r\n"
        "\x01""9 S~/ @-#Y3~^>J)0___F\"U1@?\r"
        "\x01.!FVERSION.TXTL\r"
        "\x01*\"D0.3.0#JO\r"
        "\x01##ZB\r"
        "\x01#$B+\r";

    static char output[] =
        "\x01""0 Y~!  -#N1N\" !~(\r"
        "\x01#!Y?\r"
        "\x01#\"Y@\r"
        "\x01##YA\r"
        "\x01#$YB\r"
        "File transfer completed successfully.\r\n";

    queue_init(&qin, inbuf, sizeof(inbuf));
    queue_init(&qout, outbuf, sizeof(outbuf));

    upgrade_kermit_init(&qin, &qout);

    queue_write(&qin, input, sizeof(input) - 1);
    BTASSERT(upgrade_kermit_load_file() == 0);
    queue_read(&qout, buf, sizeof(output) - 1);
    BTASSERT(memcmp(output, buf, sizeof(output) - 1) == 0);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_send_file_kermit, "test_send_file_kermit" },
        { NULL, NULL }
    };

    sys_start();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
