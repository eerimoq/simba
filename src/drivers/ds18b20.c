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

#if CONFIG_DS18B20 == 1

/* Commands. */
#define CONVERT_T         0x44
#define READ_SCRATCHPAD   0xbe
#define WRITE_SCRATCHPAD  0x4e
#define COPY_SCRATCHPAD   0x48
#define RECALL_E          0xb8
#define READ_POWER_SUPPLY 0xb4

struct ds18b20_scratchpad_t {
    int16_t temperature;
    int8_t high_trigger;
    int8_t low_trigger;
    uint8_t configuration;
    uint8_t reserved[3];
    uint8_t crc;
} PACKED;

struct module_t {
    int initialized;
    struct ds18b20_driver_t *list_p;
#if CONFIG_FS_CMD_DS18B20_LIST == 1
    struct fs_command_t cmd_list;
#endif
};

static struct module_t module;

#if CONFIG_FS_CMD_DS18B20_LIST == 1

static int cmd_list_cb(int argc,
                       const char *argv[],
                       void *chout_p,
                       void *chin_p,
                       void *arg_p,
                       void *call_arg_p)
{
    struct ds18b20_driver_t *self_p;
    char buf[10];
    struct owi_device_t *dev_p;
    int i;

    std_fprintf(chout_p, OSTR("              ID      TEMP\r\n"));

    self_p = module.list_p;

    while (self_p != NULL) {
        owi_search(self_p->owi_p);
        ds18b20_convert(self_p);
        dev_p = self_p->owi_p->devices_p;

        for (i = 0; i < self_p->owi_p->len; i++, dev_p++) {
            if (dev_p->id[0] != DS18B20_FAMILY_CODE) {
                continue;
            }

            if (ds18b20_get_temperature_str(self_p, dev_p->id, buf) == NULL) {
                buf[0] = '-';
                buf[1] = '\0';
            }

            std_fprintf(chout_p,
                        OSTR("%02x%02x%02x%02x%02x%02x%02x%02x %9s\r\n"),
                        (unsigned int)dev_p->id[0],
                        (unsigned int)dev_p->id[1],
                        (unsigned int)dev_p->id[2],
                        (unsigned int)dev_p->id[3],
                        (unsigned int)dev_p->id[4],
                        (unsigned int)dev_p->id[5],
                        (unsigned int)dev_p->id[6],
                        (unsigned int)dev_p->id[7],
                        buf);
        }

        self_p = self_p->next_p;
    }

    return (0);
}

#endif

/**
 * Read scratchpad in device.
 * @param[in] drv Driver object to be initialized.
 * @param[out] scratchpad Read scratchpad.
 * @param[in] id Device identity.
 * @return zero(0) or negative error code.
 */
static int ds18b20_read_scratchpad(struct ds18b20_driver_t *self_p,
                                   struct ds18b20_scratchpad_t *scratchpad_p,
                                   const uint8_t *id_p)
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
    /* Return immediately if the module is already initialized. */
    if (module.initialized == 1) {
        return (0);
    }

    module.initialized = 1;

#if CONFIG_FS_CMD_DS18B20_LIST == 1

    fs_command_init(&module.cmd_list,
                    CSTR("/drivers/ds18b20/list"),
                    cmd_list_cb,
                    NULL);
    fs_command_register(&module.cmd_list);

#endif

    return (0);
}

int ds18b20_init(struct ds18b20_driver_t *self_p,
                 struct owi_driver_t *owi_p)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(owi_p != NULL, EINVAL);

    self_p->owi_p = owi_p;
    self_p->next_p = module.list_p;
    module.list_p = self_p;

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
    thrd_sleep_ms(750);

    return (0);
}

int ds18b20_get_temperature(struct ds18b20_driver_t *self_p,
                            const uint8_t *id_p,
                            int *temp_p)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(id_p != NULL, EINVAL);
    ASSERTN(temp_p != NULL, EINVAL);

    struct ds18b20_scratchpad_t scratchpad;
    uint8_t crc;

    ds18b20_read_scratchpad(self_p, &scratchpad, id_p);
    crc = crc_8(0,
                CRC_8_POLYNOMIAL_8_5_4_0,
                &scratchpad,
                sizeof(scratchpad));

    if (crc != 0) {
        return (-1);
    }

    *temp_p = scratchpad.temperature;

    return (0);
}

char *ds18b20_get_temperature_str(struct ds18b20_driver_t *self_p,
                                  const uint8_t *id_p,
                                  char *temp_p)
{
    ASSERTNRN(self_p != NULL, EINVAL);
    ASSERTNRN(id_p != NULL, EINVAL);
    ASSERTNRN(temp_p != NULL, EINVAL);

    int temp;

    if (ds18b20_get_temperature(self_p, id_p, &temp) != 0) {
        return (NULL);
    }

    std_sprintf(temp_p,
                FSTR("%d.%d"),
                (temp >> 4),
                (temp & 0xf) * 625);

    return (temp_p);
}

#endif
