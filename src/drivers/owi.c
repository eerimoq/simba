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

#if CONFIG_OWI == 1

int owi_init(struct owi_driver_t *self_p,
             struct pin_device_t *dev_p,
             struct owi_device_t *devices_p,
             size_t nmemb)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(dev_p != NULL, EINVAL);
    ASSERTN(devices_p != NULL, EINVAL);
    ASSERTN(nmemb > 0, EINVAL);

    pin_init(&self_p->pin, dev_p, PIN_OUTPUT);
    pin_write(&self_p->pin, 1);
    self_p->devices_p = devices_p;
    self_p->len = 0;
    self_p->nmemb = nmemb;

    return (0);
}

int owi_reset(struct owi_driver_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    int attempts = 5;
    int err;

    do {
        pin_write(&self_p->pin, 0);
        time_busy_wait_us(480);
        sys_lock();
        pin_write(&self_p->pin, 1);
        pin_set_mode(&self_p->pin, PIN_INPUT);
        time_busy_wait_us(70);
        err = pin_read(&self_p->pin);
        sys_unlock();
        time_busy_wait_us(410);
        pin_set_mode(&self_p->pin, PIN_OUTPUT);
    } while ((--attempts > 0) && (err != 0));

    return (err == 0);
}

int owi_search(struct owi_driver_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    struct owi_device_t *dev_p;
    int8_t prev_discr = -1, discr = 0;
    uint8_t b;
    int i, j, p;

    self_p->len = 0;
    dev_p = self_p->devices_p;

    while ((discr != -1) && (self_p->len < self_p->nmemb)) {
        discr = -1;

        /* Issue reset command. */
        owi_reset(self_p);

        /* Issue search command. */
        b = OWI_SEARCH_ROM;
        owi_write(self_p, &b, 8);

        /* Search for next id in tree. */
        p = 0;

        for (i = 0; i < 8; i++) {
            for (j = 0; j < 8; j++) {
                dev_p->id[i] >>= 1;
                b = 0;
                owi_read(self_p, &b, 2);

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
                    return (self_p->len);

                default:
                    break;
                }

                owi_write(self_p, &b, 1);
                dev_p->id[i] |= (b << 7);
                p++;
                time_busy_wait_us(1);
            }
        }

        prev_discr = discr;
        self_p->len++;
        dev_p++;
    }

    return (self_p->len);
}

ssize_t owi_read(struct owi_driver_t *self_p,
                 void *buf_p,
                 size_t size)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(buf_p != NULL, EINVAL);
    ASSERTN(size > 0, EINVAL);

    uint8_t *b_p = buf_p;
    int i;

    for (i = 0; i < size; i++) {
        sys_lock();
        pin_write(&self_p->pin, 0);
        time_busy_wait_us(5);
        pin_set_mode(&self_p->pin, PIN_INPUT);
        time_busy_wait_us(9);
        *b_p >>= 1;
        *b_p |= (pin_read(&self_p->pin) << 7);
        sys_unlock();
        time_busy_wait_us(55);
        pin_set_mode(&self_p->pin, PIN_OUTPUT);
        pin_write(&self_p->pin, 1);

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

ssize_t owi_write(struct owi_driver_t *self_p,
                  const void *buf_p,
                  size_t size)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(buf_p != NULL, EINVAL);
    ASSERTN(size > 0, EINVAL);

    int i;
    uint8_t value = 0;
    const uint8_t *b_p = buf_p;

    for (i = 0; i < size; i++) {
        if ((i & 0x7) == 0) {
            value = *b_p++;
        }

        sys_lock();
        pin_write(&self_p->pin, 0);

        if (value & 1) {
            time_busy_wait_us(5);
            pin_write(&self_p->pin, 1);
            time_busy_wait_us(64);
        } else {
            time_busy_wait_us(59);
            pin_write(&self_p->pin, 1);
            time_busy_wait_us(10);
        }

        sys_unlock();
        value >>= 1;
    }

    return (size);
}

#endif
