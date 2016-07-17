/**
 * @file ds18b20.c
 * @version 1.1.0
 *
 * @section License
 * Copyright (C) 2014-2016, Erik Moqvist
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * This file is part of the Simba project.
 */

#include "simba.h"

/* DS18B20 one wire family code. */
#define FAMILY_CODE 0x28

/* Commands. */
#define CONVERT_T         0x44
#define READ_SCRATCHPAD   0xbe
#define WRITE_SCRATCHPAD  0x4e
#define COPY_SCRATCHPAD   0x48
#define RECALL_E          0xb8
#define READ_POWER_SUPPLY 0xb4

static struct fs_command_t cmd_list;

struct ds18b20_scratchpad_t {
    int16_t temperature;
    int8_t high_trigger;
    int8_t low_trigger;
    uint8_t configuration;
    uint8_t reserved[3];
    uint8_t crc;
};

static struct ds18b20_driver_t *list_p = NULL;

static int cmd_list_cb(int argc,
                       const char *argv[],
                       chan_t *chout_p,
                       chan_t *chin_p,
                       void *arg_p,
                       void *call_arg_p)
{
    struct ds18b20_driver_t *self_p;
    char buf[10];
    struct owi_device_t *dev_p;
    int i;

    std_fprintf(chout_p, FSTR("              ID      TEMP\r\n"));

    self_p = list_p;

    while (self_p != NULL) {
        owi_search(self_p->owi_p);
        ds18b20_convert(self_p);
        dev_p = self_p->owi_p->devices_p;

        for (i = 0; i < self_p->owi_p->len; i++, dev_p++) {
            if (dev_p->id[0] != FAMILY_CODE) {
                continue;
            }

            std_fprintf(chout_p,
                        FSTR("%02x%02x%02x%02x%02x%02x%02x%02x %9s\r\n"),
                        (unsigned int)dev_p->id[0],
                        (unsigned int)dev_p->id[1],
                        (unsigned int)dev_p->id[2],
                        (unsigned int)dev_p->id[3],
                        (unsigned int)dev_p->id[4],
                        (unsigned int)dev_p->id[5],
                        (unsigned int)dev_p->id[6],
                        (unsigned int)dev_p->id[7],
                        ds18b20_get_temperature_str(self_p, dev_p->id, buf));
        }

        self_p = self_p->next_p;
    }

    return (0);
}

/**
 * Read scratchpad in device.
 * @param[in] drv Driver object to be initialized.
 * @param[out] scratchpad Read scratchpad.
 * @param[in] id Device identity.
 * @return zero(0) or negative error code.
 */
static int ds18b20_read_scratchpad(struct ds18b20_driver_t *self_p,
                                   struct ds18b20_scratchpad_t *scratchpad_p,
                                   uint8_t *id_p)
{
    uint8_t b;

    owi_reset(self_p->owi_p);
    b = OWI_MATCH_ROM;
    owi_write(self_p->owi_p, &b, 8);
    owi_write(self_p->owi_p, id_p, 64);
    b = READ_SCRATCHPAD;
    owi_write(self_p->owi_p, &b, 8);
    owi_read(self_p->owi_p, scratchpad_p, 8 * sizeof(*scratchpad_p));

    return (0);
}

int ds18b20_module_init()
{
    fs_command_init(&cmd_list,
                    FSTR("/drivers/ds18b20/list"),
                    cmd_list_cb,
                    NULL);
    fs_command_register(&cmd_list);

    return (0);
}

int ds18b20_init(struct ds18b20_driver_t *self_p,
                 struct owi_driver_t *owi_p)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(owi_p != NULL, EINVAL);

    self_p->owi_p = owi_p;
    self_p->next_p = list_p;
    list_p = self_p;

    return (0);
}

int ds18b20_convert(struct ds18b20_driver_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    uint8_t b;

    owi_reset(self_p->owi_p);
    b = OWI_SKIP_ROM;
    owi_write(self_p->owi_p, &b, 8);
    b = CONVERT_T;
    owi_write(self_p->owi_p, &b, 8);
    thrd_usleep(750000);

    return (0);
}

int ds18b20_get_temperature(struct ds18b20_driver_t *self_p,
                            uint8_t *id_p,
                            int *temp_p)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(id_p != NULL, EINVAL);
    ASSERTN(temp_p != NULL, EINVAL);

    struct ds18b20_scratchpad_t scratchpad;

    ds18b20_read_scratchpad(self_p, &scratchpad, id_p);
    *temp_p = scratchpad.temperature;

    return (0);
}

char *ds18b20_get_temperature_str(struct ds18b20_driver_t *self_p,
                                  uint8_t *id_p,
                                  char *buf_p)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(id_p != NULL, EINVAL);
    ASSERTN(buf_p != NULL, EINVAL);

    int temp;

    ds18b20_get_temperature(self_p, id_p, &temp);
    std_sprintf(buf_p,
                FSTR("%d.%d"),
                (temp >> 4),
                (temp & 0xf) * 625);

    return (buf_p);
}
