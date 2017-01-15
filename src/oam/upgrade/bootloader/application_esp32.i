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

#include "esp_ota_ops.h"

struct application_t {
    const esp_partition_t *partition_p;
    size_t offset;
};

static struct application_t application;

static const esp_partition_t *get_application_partition(void)
{
    static const esp_partition_t *partition_p;

    partition_p = esp_esp_partition_find_first(ESP_PARTITION_TYPE_APP,
                                               ESP_PARTITION_SUBTYPE_APP_OTA_0,
                                               NULL);

    if (partition_p == NULL) {
        std_printf(FSTR("error: application partition missing\r\n"));
        return (NULL);
    }

    return (partition_p);
}

static int upgrade_bootloader_application_port_erase()
{
    static const esp_partition_t *partition_p;

    partition_p = get_application_partition();

    if (partition_p == NULL) {
        return (-1);
    }

    if (esp_esp_partition_erase_range(partition_p,
                                      0,
                                      partition_p->size) != ESP_OK) {
        return (-1);
    }

    return (0);
}

static int upgrade_bootloader_application_port_write_begin(void)
{
    application.partition_p = get_application_partition();

    if (application.partition_p == NULL) {
        std_printf(FSTR("error: application partition missing\r\n"));
        return (-1);
    }

    application.offset = 0;

    return (0);

}

static int upgrade_bootloader_application_port_write_chunk(const void *buf_p,
                                                           size_t size)
{
    if (esp_esp_partition_write(application.partition_p,
                                application.offset,
                                buf_p,
                                size) != ESP_OK) {
        return (-1);
    }

    application.offset += size;

    return (0);
}

static int upgrade_bootloader_application_port_write_end()
{
    if (esp_esp_ota_set_boot_partition(application.partition_p) != ESP_OK) {
        std_printf(FSTR("error: set boot partition\r\n"));
        return (-1);
    }

    return (0);
}

static int upgrade_bootloader_application_port_is_valid()
{
    return (0);
}

static int upgrade_bootloader_application_port_write_valid_flag()
{
    return (0);
}

static int upgrade_bootloader_application_port_jump()
{
    return (-1);
}

static int upgrade_bootloader_application_port_sha1(uint8_t *dst_p)
{
    static const esp_partition_t *partition_p;
    struct sha1_t sha1;
    uint8_t buf[256];
    size_t size;
    size_t left;

    partition_p = get_application_partition();

    if (partition_p == NULL) {
        std_printf(FSTR("error: application partition missing\r\n"));
        return (-1);
    }

    sha1_init(&sha1);

    left = partition_p->size;

    while (left > 0) {
        if (left < sizeof(buf)) {
            size = left;
        } else {
            size = sizeof(buf);
        }

        if (esp_esp_partition_read(partition_p,
                                   partition_p->size - left,
                                   &buf[0],
                                   size) != ESP_OK) {
            return (-1);
        }

        sha1_update(&sha1, &buf[0], size);

        left -= size;
    }

    sha1_digest(&sha1, dst_p);

    return (0);
}
