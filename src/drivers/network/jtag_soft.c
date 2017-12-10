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

#if CONFIG_JTAG_SOFT == 1

static int clock_low(struct jtag_soft_driver_t *self_p)
{
    pin_write(&self_p->tck, 0);
    time_busy_wait_us(1);

    return (0);
}

static int clock_high(struct jtag_soft_driver_t *self_p)
{
    pin_write(&self_p->tck, 1);
    time_busy_wait_us(1);

    return (0);
}

static int clock_pulse(struct jtag_soft_driver_t *self_p)
{
    clock_low(self_p);
    clock_high(self_p);

    return (0);
}

static int move(struct jtag_soft_driver_t *self_p,
                uint8_t bits,
                size_t number_of_bits)
{
    size_t i;

    for (i = 0; i < number_of_bits; i++) {
        jtag_soft_make_transition(self_p, ((bits >> i) & 1));
    }

    return (0);
}

static int move_to_reset(struct jtag_soft_driver_t *self_p)
{
    return (move(self_p, 0x1f, 5));
}

static int move_to_idle_from_reset(struct jtag_soft_driver_t *self_p)
{
    return (move(self_p, 0x00, 1));
}

static int move_to_shift_ir_from_idle(struct jtag_soft_driver_t *self_p)
{
    return (move(self_p, 0x03, 4));
}

static int move_to_shift_dr_from_idle(struct jtag_soft_driver_t *self_p)
{
    return (move(self_p, 0x01, 3));
}

static int move_to_idle_from_shift(struct jtag_soft_driver_t *self_p)
{
    return (move(self_p, 0x01, 2));
}

static int transfer(struct jtag_soft_driver_t *self_p,
                    void *rxbuf_p,
                    const void *txbuf_p,
                    size_t number_of_bits)
{
    size_t i;
    size_t byte_index;
    size_t bit_index;
    uint8_t *u8_rxbuf_p;
    const uint8_t *u8_txbuf_p;
    int value;

    u8_rxbuf_p = rxbuf_p;
    u8_txbuf_p = txbuf_p;

    if (u8_rxbuf_p != NULL) {
        memset(u8_rxbuf_p, 0, DIV_CEIL(number_of_bits, 8));
    }

    for (i = 0; i < number_of_bits; i++) {
        byte_index = (i / 8);
        bit_index = (7 - (i % 8));

        /* The last bit is transferred with TMS=1. */
        if (i == (number_of_bits - 1)) {
            pin_write(&self_p->tms, 1);
        }

        if (u8_txbuf_p != NULL) {
            value = ((u8_txbuf_p[byte_index] >> bit_index) & 1);
            pin_write(&self_p->tdi, value);
        }

        clock_low(self_p);

        if (u8_rxbuf_p != NULL) {
            value = pin_read(&self_p->tdo);
            u8_rxbuf_p[byte_index] |= (value << bit_index);
        }

        clock_high(self_p);
    }

    return (move_to_idle_from_shift(self_p));
}

int jtag_soft_module_init(void)
{
    return (0);
}

int jtag_soft_init(struct jtag_soft_driver_t *self_p,
                   struct pin_device_t *tck_p,
                   struct pin_device_t *tms_p,
                   struct pin_device_t *tdi_p,
                   struct pin_device_t *tdo_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    pin_init(&self_p->tck, tck_p, PIN_INPUT);
    pin_init(&self_p->tms, tms_p, PIN_INPUT);
    pin_init(&self_p->tdi, tdi_p, PIN_INPUT);
    pin_init(&self_p->tdo, tdo_p, PIN_INPUT);

    return (0);
}

int jtag_soft_start(struct jtag_soft_driver_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    pin_set_mode(&self_p->tck, PIN_OUTPUT);
    pin_set_mode(&self_p->tms, PIN_OUTPUT);
    pin_set_mode(&self_p->tdi, PIN_OUTPUT);

    return (jtag_soft_reset(self_p));
}

int jtag_soft_stop(struct jtag_soft_driver_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    move_to_reset(self_p);

    pin_set_mode(&self_p->tck, PIN_INPUT);
    pin_set_mode(&self_p->tms, PIN_INPUT);
    pin_set_mode(&self_p->tdi, PIN_INPUT);

    return (0);
}

int jtag_soft_reset(struct jtag_soft_driver_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    move_to_reset(self_p);
    move_to_idle_from_reset(self_p);

    return (0);
}

int jtag_soft_instruction_write(struct jtag_soft_driver_t *self_p,
                                const void *buf_p,
                                size_t number_of_bits)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(buf_p != NULL, EINVAL);

    move_to_shift_ir_from_idle(self_p);

    return (transfer(self_p, NULL, buf_p, number_of_bits));
}

int jtag_soft_data_transfer(struct jtag_soft_driver_t *self_p,
                            void *rxbuf_p,
                            const void *txbuf_p,
                            size_t number_of_bits)
{
    ASSERTN(self_p != NULL, EINVAL);

    move_to_shift_dr_from_idle(self_p);

    return (transfer(self_p, rxbuf_p, txbuf_p, number_of_bits));
}

int jtag_soft_data_read(struct jtag_soft_driver_t *self_p,
                        void *buf_p,
                        size_t number_of_bits)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(buf_p != NULL, EINVAL);

    return (jtag_soft_data_transfer(self_p, buf_p, NULL, number_of_bits));
}

int jtag_soft_data_write(struct jtag_soft_driver_t *self_p,
                         const void *buf_p,
                         size_t number_of_bits)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(buf_p != NULL, EINVAL);

    return (jtag_soft_data_transfer(self_p, NULL, buf_p, number_of_bits));
}

int jtag_soft_make_transition(struct jtag_soft_driver_t *self_p,
                              int transition)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN((transition == 0) || (transition == 1), EINVAL);

    pin_write(&self_p->tms, transition);

    return (clock_pulse(self_p));
}

#endif
