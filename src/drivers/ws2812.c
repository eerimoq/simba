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

#if CONFIG_WS2812 == 1

#if defined(ARCH_ESP32)

static inline void nop(int number)
{
    int i;

    for (i = 0; i < number; i++) {
        asm volatile ("nop");
    }
}

static inline void busy_wait_300_ns(void)
{
    int i;

    for (i = 0; i < 13; i++) {
        asm volatile ("nop");
    }
}

static inline void busy_wait_600_ns(void)
{
    int i;

    for (i = 0; i < 20; i++) {
        asm volatile ("nop");
    }
}

#else
#    error "Architecture not supported."
#endif

int ws2812_module_init()
{
    return (0);
}

int ws2812_init(struct ws2812_driver_t *self_p,
                struct pin_device_t **pins_pp,
                int number_of_pins)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(pins_pp != NULL, EINVAL);
    ASSERTN((number_of_pins > 0)
            && (number_of_pins <= WS2812_PIN_DEVICES_MAX),
            EINVAL);

    int i;

    self_p->pins_pp = pins_pp;
    self_p->number_of_pins = number_of_pins;
    self_p->mask = 0;

    for (i = 0; i < number_of_pins; i++) {
        pin_device_set_mode(pins_pp[i], PIN_OUTPUT);
        pin_device_write_low(pins_pp[i]);
        self_p->mask |= (1 << pins_pp[i]->id);
    }

    return (0);
}

int ws2812_write(struct ws2812_driver_t *self_p,
                 const uint8_t *buffer_p,
                 int number_of_pixles)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(buffer_p != NULL, EINVAL);

    int i, j, k, l;
    uint32_t ones, zeros;

    /* Refresh, 50 us is not needed, 5 us is enough. */
    time_busy_wait_us(5);

    sys_lock();

    for (i = 0; i < number_of_pixles; i++) {
        for (j = 0; j < 3; j++) {
            for (k = 7; k >= 0; k--) {
                ones = 0;

                for (l = 0; l < 8; l++) {
                    if (l < self_p->number_of_pins) {
                        if ((buffer_p[l] >> k) & 0x01) {
                            ones |= (1 << self_p->pins_pp[l]->id);
                        } else {
                            nop(1);
                        }
                    } else {
                            nop(3);
                    }
                }

                zeros = ~ones;
                zeros &= self_p->mask;

                ESP32_GPIO->OUT[0].W1TS = self_p->mask;

                /* Zeros. */
                busy_wait_300_ns();
                ESP32_GPIO->OUT[0].W1TC = zeros;

                /* Ones. */
                busy_wait_600_ns();
                ESP32_GPIO->OUT[0].W1TC = ones;
            }

            buffer_p += self_p->number_of_pins;
        }
    }

    sys_unlock();

    return (0);
}

#endif
