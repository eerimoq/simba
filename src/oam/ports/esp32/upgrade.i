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

#include "esp_ota_ops.h"

#define STAY_MAGIC "stay"

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

static int application_sha1(uint8_t *dst_p, size_t size)
{
    static const esp_partition_t *partition_p;
    struct sha1_t sha1;
    uint8_t buf[256];
    size_t left;
    size_t chunk_size;

    partition_p = get_application_partition();

    if (partition_p == NULL) {
        std_printf(FSTR("error: application partition missing\r\n"));
        return (-1);
    }

    sha1_init(&sha1);

    left = size;

    while (left > 0) {
        if (left < sizeof(buf)) {
            chunk_size = left;
        } else {
            chunk_size = sizeof(buf);
        }

        if (esp_esp_partition_read(partition_p,
                                   size - left,
                                   &buf[0],
                                   chunk_size) != ESP_OK) {
            return (-1);
        }

        sha1_update(&sha1, &buf[0], chunk_size);

        left -= chunk_size;
    }

    sha1_digest(&sha1, dst_p);

    return (0);
}

static int upgrade_port_bootloader_enter()
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

    if (upgrade_bootloader_stay_set() != 0) {
        return (-1);
    }

#if !defined(UPGRADE_TEST)
    sys_reboot();
#endif

    /* Will never be here. System has been rebooted. */

    return (-1);
}

static int upgrade_port_bootloader_stay_set()
{
    const esp_partition_t *partition_p;

    partition_p = esp_esp_partition_find_first(ESP_PARTITION_TYPE_APP,
                                               ESP_PARTITION_SUBTYPE_APP_FACTORY,
                                               NULL);

    if (partition_p == NULL) {
        return (-1);
    }

    if (esp_esp_partition_write(partition_p,
                                partition_p->size - SPI_FLASH_SEC_SIZE,
                                STAY_MAGIC,
                                4) != ESP_OK) {
        return (-1);
    }

    return (0);
}

static int upgrade_port_bootloader_stay_clear()
{
    const esp_partition_t *partition_p;

    partition_p = esp_esp_partition_find_first(ESP_PARTITION_TYPE_APP,
                                               ESP_PARTITION_SUBTYPE_APP_FACTORY,
                                               NULL);

    if (partition_p == NULL) {
        return (-1);
    }

    if (esp_esp_partition_erase_range(partition_p,
                                      partition_p->size - SPI_FLASH_SEC_SIZE,
                                      SPI_FLASH_SEC_SIZE) != ESP_OK) {
        return (-1);
    }

    return (0);
}

static int upgrade_port_bootloader_stay_get()
{
    const esp_partition_t *partition_p;
    char buf[4];

    partition_p = esp_esp_partition_find_first(ESP_PARTITION_TYPE_APP,
                                               ESP_PARTITION_SUBTYPE_APP_FACTORY,
                                               NULL);

    if (partition_p == NULL) {
        return (-1);
    }

    if (esp_esp_partition_read(partition_p,
                               partition_p->size - SPI_FLASH_SEC_SIZE,
                               buf,
                               sizeof(buf)) != ESP_OK) {
        return (-1);
    }

    return (memcmp(&buf[0], STAY_MAGIC, sizeof(buf)) == 0);
}

static int upgrade_port_application_enter()
{
    const esp_partition_t *partition_p;

    if (upgrade_application_is_valid(1) != 1) {
        return (-1);
    }

    partition_p = esp_esp_partition_find_first(ESP_PARTITION_TYPE_APP,
                                               ESP_PARTITION_SUBTYPE_APP_OTA_0,
                                               NULL);

    if (partition_p == NULL) {
        return (-1);
    }

    if (esp_esp_ota_set_boot_partition(partition_p) != ESP_OK) {
        return (-1);
    }

    if (upgrade_bootloader_stay_clear() != 0) {
        return (-1);
    }

#if !defined(UPGRADE_TEST)
    sys_reboot();
#endif

    /* Will never be here. System has been rebooted. */

    return (-1);
}

static int upgrade_port_application_erase()
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

static int upgrade_port_application_is_valid(int quick)
{
    static const esp_partition_t *partition_p;
    uint8_t expected_sha1[20];
    uint8_t sha1[20];
    size_t size;

    partition_p = get_application_partition();

    if (partition_p == NULL) {
        return (-1);
    }

    if (esp_esp_partition_read(partition_p,
                               partition_p->size - sizeof(size),
                               &size,
                               sizeof(size)) != ESP_OK) {
        return (-1);
    }

    if (size > partition_p->size) {
        return (0);
    }

    if (esp_esp_partition_read(partition_p,
                               (partition_p->size
                                - sizeof(expected_sha1)
                                - sizeof(size)),
                               &expected_sha1,
                               sizeof(expected_sha1)) != ESP_OK) {
        return (-1);
    }

    if (quick == 1) {
        memset(&sha1[0], 0xff, sizeof(sha1));

        return (memcmp(&sha1[0], &expected_sha1[0], sizeof(sha1)) != 0);
    } else {
        if (application_sha1(&sha1[0], size) != 0) {
            return (-1);
        }

        return (memcmp(&sha1[0], &expected_sha1[0], sizeof(sha1)) == 0);
    }
}

static int upgrade_port_binary_upload_begin()
{
    application.partition_p = get_application_partition();

    if (application.partition_p == NULL) {
        std_printf(FSTR("error: application partition missing\r\n"));
        return (-1);
    }

    application.offset = 0;

    return (0);
}

static int upgrade_port_binary_upload(const void *buf_p,
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

static int upgrade_port_binary_upload_end()
{
    if (esp_esp_partition_write(application.partition_p,
                                (application.partition_p->size
                                 - sizeof(module.header.size)),
                                &module.header.size,
                                sizeof(module.header.size)) != ESP_OK) {
        return (-1);
    }

    if (esp_esp_partition_write(application.partition_p,
                                (application.partition_p->size
                                 - sizeof(module.header.size)
                                 - sizeof(module.header.sha1)),
                                &module.header.sha1,
                                sizeof(module.header.sha1)) != ESP_OK) {
        return (-1);
    }

    if (upgrade_application_is_valid(0) != 1) {
        return (-1);
    }

    if (esp_esp_ota_set_boot_partition(application.partition_p) != ESP_OK) {
        std_printf(FSTR("error: set boot partition\r\n"));
        return (-1);
    }

    return (0);
}
