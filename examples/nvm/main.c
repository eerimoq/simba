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

int main()
{
    long counter;
    ssize_t size;

    sys_start();

    std_printf(sys_get_info());

    log_object_print(NULL,
                     LOG_INFO,
                     OSTR("Reading counter from non-volatile memory...\r\n"));

    size = nvm_read(&counter, 0, sizeof(counter));

    if (size != sizeof(counter)) {
        log_object_print(NULL, LOG_INFO, OSTR("failed.\r\n"));
        return (-1);
    }

    log_object_print(NULL, LOG_INFO, OSTR("done.\r\n"));
    log_object_print(NULL,
                     LOG_INFO,
                     OSTR("Incrementing counter from %ld to %ld.\r\n"),
                     counter,
                     counter + 1);
    counter++;

    log_object_print(NULL,
                     LOG_INFO,
                     OSTR("Writing incremented counter to non-volatile "
                          "memory...\r\n"));

    size = nvm_write(0, &counter, sizeof(counter));

    if (size != sizeof(counter)) {
        log_object_print(NULL, LOG_INFO, OSTR("failed.\r\n"));

        return (-1);
    }

    log_object_print(NULL, LOG_INFO, OSTR("done.\r\n"));
    log_object_print(NULL, LOG_INFO, OSTR("All good!\r\n"));

    thrd_suspend(NULL);

    return (0);
}
