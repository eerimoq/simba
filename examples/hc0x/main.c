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

/**
 * Setup a shell on the UART towards the PC
 */

#include "simba.h"

#define EOT '\x04'

static struct fs_command_t cmd_at;

static struct uart_soft_driver_t uart;

static char qinbuf[32];

static struct shell_t shell;

static int cmd_at_cb(int argc,
                     const char *argv[],
                     void *out_p,
                     void *in_p,
                     void *arg_p,
                     void *call_arg_p)
{
    struct chan_list_t list;
    void *chan_p;
    char c;
    char buf[32];

    std_fprintf(out_p, OSTR("type ctrl-d to exit\r\n"));

    /* Wait for data from PC and HC-0X. */
    chan_list_init(&list, buf, sizeof(buf));
    chan_list_add(&list, &uart.chin);
    chan_list_add(&list, in_p);

    /* Pass data between PC and bluetooth device. */
    while (1) {
        chan_p = chan_list_poll(&list, NULL);

        if (chan_p == in_p) {
            chan_read(chan_p, &c, sizeof(c));

            /* break if EOT is found, otherwise write to HC-0X. */
            if (c == EOT) {
                break;
            }

            chan_write(&uart.chout, &c, sizeof(c));
        } else if (chan_p == &uart.chin) {
            /* Write all output from HC-0X to the PC. */
            chan_read(chan_p, &c, sizeof(c));
            chan_write(out_p, &c, sizeof(c));
        } else {
            std_printf(OSTR("bad input channel 0x%02x\r\n"),  (int)chan_p);
        }
    }

    chan_list_destroy(&list);

    return (0);
}

int main()
{
    sys_start();

    uart_soft_init(&uart,
                   &pin_d4_dev,
                   &pin_d3_dev,
                   &exti_d3_dev,
                   9600,
                   qinbuf,
                   sizeof(qinbuf));

    fs_command_init(&cmd_at,
                    CSTR("/at"),
                    cmd_at_cb,
                    NULL);
    fs_command_register(&cmd_at);

    std_printf(FSTR("Welcome to HC-0X configuration tool!\r\n"
                    "\r\n"
                    "SETUP: Connect pin34 to VCC so the device enters AT mode.\r\n"
                    "\r\n"
                    "Type 'at' to start communicating with the device.\r\n"));

    shell_init(&shell,
               sys_get_stdin(),
               sys_get_stdout(),
               NULL,
               NULL,
               NULL,
               NULL);
    shell_main(&shell);

    return (0);
}
