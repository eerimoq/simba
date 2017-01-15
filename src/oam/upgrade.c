/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2016, Erik Moqvist
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

struct module_t {
    int initialized;
};

static struct module_t module;

int upgrade_module_init()
{
    /* Return immediately if the module is already initialized. */
    if (module.initialized == 1) {
        return (0);
    }

    module.initialized = 1;

#if CONFIG_UPGRADE_APPLICATION == 1
    upgrade_application_bootloader_module_init();
#endif

#if CONFIG_UPGRADE_BOOTLOADER == 1
    upgrade_bootloader_application_module_init();
    upgrade_bootloader_kermit_module_init();
#endif

    return (0);
}

int upgrade_bootloader_start()
{
#if CONFIG_UPGRADE_HTTP_SERVER == 1
    upgrade_bootloader_http_init();
    upgrade_bootloader_http_start();
#endif

#if CONFIG_UPGRADE_TFTP_SERVER == 1
    upgrade_bootloader_tftp_init();
    upgrade_bootloader_tftp_start();
#endif

    upgrade_bootloader_kermit_init(sys_get_stdin(), sys_get_stdout());

    return (0);
}
