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

#define IP 192.168.0.7

static struct tftp_server_t server;
static THRD_STACK(tftp_server_stack, 2048);

int main()
{
    struct inet_addr_t addr;

    sys_start();

    inet_aton(STRINGIFY(IP), &addr.ip);
    addr.port = 69;

    /* Create the TFTP server. */
    if (tftp_server_init(&server,
                         &addr,
                         5000,
                         "tftp_server",
                         tftp_server_stack,
                         sizeof(tftp_server_stack)) != 0) {
        std_printf(FSTR("tftp_server_init() failed\r\n"));
        return (-1);
    }

    if (tftp_server_start(&server) != 0) {
        std_printf(FSTR("tftp_server_start() failed\r\n"));
        return (-1);
    }

    std_printf(FSTR("TFTP server running.\r\n"));

    thrd_suspend(NULL);

    return (0);
}
