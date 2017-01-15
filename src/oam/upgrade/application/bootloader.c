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

static struct fs_command_t cmd_bootloader_enter;

/**
 * File system command to enter the bootloader from the application.
 */
static int cmd_bootloader_enter_cb(int argc,
                                   const char *argv[],
                                   void *out_p,
                                   void *in_p,
                                   void *arg_p,
                                   void *call_arg_p)
{
    return (upgrade_application_bootloader_enter());
}

int upgrade_application_bootloader_module_init()
{
    fs_command_init(&cmd_bootloader_enter,
                    FSTR("/oam/upgrade/bootloader/enter"),
                    cmd_bootloader_enter_cb,
                    NULL);
    fs_command_register(&cmd_bootloader_enter);

    return (0);
}

#if defined(ARCH_ESP32)

#include "esp_ota_ops.h"

int upgrade_application_bootloader_enter()
{
    const esp_partition_t *partition_p;
    
    partition_p = esp_esp_partition_find_first(ESP_PARTITION_TYPE_APP,
                                               ESP_PARTITION_SUBTYPE_APP_FACTORY,
                                               NULL);

    if (partition_p == NULL) {
        return (-1);
    }

    if (esp_esp_ota_set_boot_partition(partition_p) != ESP_OK) {
        return (-1);
    }

    sys_reboot();

    /* Will never be here. System has been rebooted. */
    
    return (-1);
}

#else

int upgrade_application_bootloader_enter()
{
    return (-1);
}

#endif
