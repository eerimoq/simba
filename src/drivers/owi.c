/**
 * @file owi.c
 * @version 1.0
 *
 * @section License
 * Copyright (C) 2014-2015, Erik Moqvist
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

int owi_init(struct owi_driver_t *drv_p,
             struct pin_device_t *dev_p,
             struct owi_device_t *devices_p,
             size_t nmemb)
{
    pin_init(&drv_p->pin, dev_p, PIN_OUTPUT);
    pin_write(&drv_p->pin, 1);
    drv_p->devices_p = devices_p;
    drv_p->len = 0;
    drv_p->nmemb = nmemb;

    return (0);
}

int owi_reset(struct owi_driver_t *drv_p)
{
    int attempts = 5;
    int err;
    core_irq_t irq;

    do {
        pin_write(&drv_p->pin, 0);
        time_sleep(480);
        irq = core_lock();
        pin_write(&drv_p->pin, 1);
        pin_set_mode(&drv_p->pin, PIN_INPUT);
        time_sleep(70);
        err = pin_read(&drv_p->pin);
        core_unlock(irq);
        time_sleep(410);
        pin_set_mode(&drv_p->pin, PIN_OUTPUT);
    } while ((--attempts > 0) && (err != 0));

    return (err == 0);
}

int owi_search(struct owi_driver_t *drv_p)
{
    struct owi_device_t *dev_p;
    int8_t prev_discr = -1, discr = 0;
    uint8_t b;
    int i, j, p;

    drv_p->len = 0;
    dev_p = drv_p->devices_p;

    while ((discr != -1) && (drv_p->len < drv_p->nmemb)) {
        discr = -1;

        /* Issue reset command. */
        owi_reset(drv_p);

        /* Issue search command. */
        b = OWI_SEARCH_ROM;
        owi_write(drv_p, &b, 8);

        /* Search for next id in tree. */
        p = 0;

        for (i = 0; i < 8; i++) {
            for (j = 0; j < 8; j++) {
                dev_p->id[i] >>= 1;
                b = 0;
                owi_read(drv_p, &b, 2);

                switch (b) {

                case 0:
                    /* Disrcpency. */
                    if (p < prev_discr) {
                        /* Follow previous branch.*/
                        b = ((dev_p[-1].id[i] >> j) & 1);
                        if (b == 0) {
                            discr = p;
                        }
                    } else if (p > prev_discr) {
                        /* Go left. */
                        b = 0;
                        discr = p;
                    } else {
                        /* Go right.*/
                        b = 1;
                    }
                    break;

                case 1:
                    /* One. */
                    b = 1;
                    break;

                case 2:
                    /* Zero. */
                    b = 0;
                    break;

                case 3:
                    /* No device answered. */
                    return (drv_p->len);

                default:
                    break;
                }

                owi_write(drv_p, &b, 1);
                dev_p->id[i] |= (b << 7);
                p++;
                time_sleep(1);
            }
        }

        prev_discr = discr;
        drv_p->len++;
        dev_p++;
    }

    return (drv_p->len);
}

ssize_t owi_read(struct owi_driver_t *drv_p,
                 void *buf_p,
                 size_t size)
{
    uint8_t *b_p = buf_p;
    int i;
    core_irq_t irq;

    for (i = 0; i < size; i++) {
        irq = core_lock();
        pin_write(&drv_p->pin, 0);
        time_sleep(5);
        pin_set_mode(&drv_p->pin, PIN_INPUT);
        time_sleep(9);
        *b_p >>= 1;
        *b_p |= (pin_read(&drv_p->pin) << 7);
        core_unlock(irq);
        time_sleep(55);
        pin_set_mode(&drv_p->pin, PIN_OUTPUT);
        pin_write(&drv_p->pin, 1);

        if ((i & 0x7) == 0x7) {
            b_p++;
        }
    }

    /* Adjust last byte. */
    if ((size & 0x7) != 0) {
        *b_p >>= (8 - (size & 0x7));
    }

    return (size);
}

ssize_t owi_write(struct owi_driver_t *drv_p,
                  const void *buf_p,
                  size_t size)
{
    int i;
    uint8_t value = 0;
    const uint8_t *b_p = buf_p;
    core_irq_t irq;

    for (i = 0; i < size; i++) {
        if ((i & 0x7) == 0) {
            value = *b_p++;
        }

        irq = core_lock();
        pin_write(&drv_p->pin, 0);

        if (value & 1) {
            time_sleep(5);
            pin_write(&drv_p->pin, 1);
            time_sleep(64);
        } else {
            time_sleep(59);
            pin_write(&drv_p->pin, 1);
            time_sleep(10);
        }

        core_unlock(irq);
        value >>= 1;
    }

    return (size);
}
