/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2018, Erik Moqvist
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

#if CONFIG_ICSP_SOFT == 1

#define P6_US  1
#define P7_US  1
#define P14_US 1
#define P19_US 1
#define P20_US 250

#define KEY    0x4d434850

static inline int clock_low(struct icsp_soft_driver_t *self_p)
{
    asm volatile ("nop");
    asm volatile ("nop");

    return (pin_device_write_low(self_p->pgec_p));
}

static inline int clock_high(struct icsp_soft_driver_t *self_p)
{
    asm volatile ("nop");
    asm volatile ("nop");

    return (pin_device_write_high(self_p->pgec_p));
}

static inline int clock_pulse(struct icsp_soft_driver_t *self_p)
{
    clock_high(self_p);
    clock_low(self_p);

    return (0);
}

static inline int clock_bit_in(struct icsp_soft_driver_t *self_p)
{
    int tdo;

    pin_device_write_low(self_p->pged_p);
    clock_pulse(self_p);
    clock_pulse(self_p);
    pin_device_set_mode(self_p->pged_p, PIN_INPUT);
    clock_pulse(self_p);
    time_busy_wait_us(0);
    tdo = pin_device_read(self_p->pged_p);
    pin_device_set_mode(self_p->pged_p, PIN_OUTPUT);
    clock_pulse(self_p);

    return (tdo);
}

static inline int clock_bit_tms(struct icsp_soft_driver_t *self_p,
                                int tms)
{
    pin_device_write_low(self_p->pged_p);
    clock_pulse(self_p);
    pin_device_write(self_p->pged_p, tms);
    clock_pulse(self_p);
    pin_device_write_high(self_p->pged_p);
    clock_pulse(self_p);

    return (clock_pulse(self_p));
}

static int move(struct icsp_soft_driver_t *self_p,
                uint8_t bits,
                size_t number_of_bits)
{
    size_t i;

    for (i = 0; i < number_of_bits; i++) {
        icsp_soft_make_transition(self_p, ((bits >> i) & 1));
    }

    return (0);
}

static int move_to_reset(struct icsp_soft_driver_t *self_p)
{
    return (move(self_p, 0x1f, 5));
}

static int move_to_idle_from_reset(struct icsp_soft_driver_t *self_p)
{
    return (move(self_p, 0x00, 1));
}

static int move_to_shift_ir_from_idle(struct icsp_soft_driver_t *self_p)
{
    return (move(self_p, 0x03, 4));
}

static int move_to_capture_dr_from_idle(struct icsp_soft_driver_t *self_p)
{
    return (move(self_p, 0x01, 2));
}

static int move_to_capture_dr_from_shift_dr(struct icsp_soft_driver_t *self_p)
{
    return (move(self_p, 0x07, 4));
}

static int move_to_idle_from_shift(struct icsp_soft_driver_t *self_p)
{
    return (move(self_p, 0x01, 2));
}

static int transfer(struct icsp_soft_driver_t *self_p,
                    void *rxbuf_p,
                    const void *txbuf_p,
                    size_t number_of_bits,
                    int rx_lsb)
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
        u8_rxbuf_p[0] = (rx_lsb << 7);
    }

    for (i = 0; i < number_of_bits; i++) {
        /* TDI. */
        if (u8_txbuf_p != NULL) {
            byte_index = (i / 8);
            bit_index = (7 - (i % 8));
            value = ((u8_txbuf_p[byte_index] >> bit_index) & 1);
        } else {
            value = 0;
        }

        pin_device_write(self_p->pged_p, value);
        clock_pulse(self_p);

        /* TMS. */
        value = (i == (number_of_bits - 1));

        pin_device_write(self_p->pged_p, value);
        clock_pulse(self_p);

        /* TDO. */
        clock_pulse(self_p);

        if (u8_rxbuf_p != NULL) {
            pin_device_set_mode(self_p->pged_p, PIN_INPUT);

            if (i != (number_of_bits - 1)) {
                byte_index = ((i + 1) / 8);
                bit_index = (7 - ((i + 1) % 8));
                value = pin_device_read(self_p->pged_p);
                u8_rxbuf_p[byte_index] |= (value << bit_index);
            }

            pin_device_set_mode(self_p->pged_p, PIN_OUTPUT);
        }

        clock_pulse(self_p);
    }

    return (move_to_idle_from_shift(self_p));
}

static int send_key(struct icsp_soft_driver_t *self_p)
{
    int i;

    for (i = 31; i >= 0; i--) {
        pin_device_write(self_p->pged_p, (KEY >> i) & 1);
        clock_pulse(self_p);
    }

    return (0);
}

int icsp_soft_module_init(void)
{
    return (0);
}

int icsp_soft_init(struct icsp_soft_driver_t *self_p,
                   struct pin_device_t *pgec_p,
                   struct pin_device_t *pged_p,
                   struct pin_device_t *mclrn_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    self_p->pgec_p = pgec_p;
    self_p->pged_p = pged_p;
    self_p->mclrn_p = mclrn_p;

    pin_device_set_mode(self_p->pgec_p, PIN_INPUT);
    pin_device_set_mode(self_p->pged_p, PIN_INPUT);
    pin_device_set_mode(self_p->mclrn_p, PIN_INPUT);

    return (0);
}

int icsp_soft_start(struct icsp_soft_driver_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    pin_device_set_mode(self_p->pgec_p, PIN_OUTPUT);
    pin_device_write_low(self_p->pgec_p);
    pin_device_set_mode(self_p->pged_p, PIN_OUTPUT);
    pin_device_write_low(self_p->pged_p);
    pin_device_set_mode(self_p->mclrn_p, PIN_OUTPUT);
    pin_device_write_low(self_p->mclrn_p);

    /* 1. Toggle MCLRN. */
    pin_device_write_low(self_p->mclrn_p);
    time_busy_wait_us(P6_US);
    pin_device_write_high(self_p->mclrn_p);
    time_busy_wait_us(P14_US + P20_US);
    pin_device_write_low(self_p->mclrn_p);

    /* 2. Send key. */
    send_key(self_p);

    /* 3. Set MCLRN high. */
    time_busy_wait_us(P19_US);
    pin_device_write_high(self_p->mclrn_p);
    time_busy_wait_us(P7_US);

    return (icsp_soft_reset(self_p));
}

int icsp_soft_stop(struct icsp_soft_driver_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    move_to_reset(self_p);

    pin_device_set_mode(self_p->pgec_p, PIN_INPUT);
    pin_device_set_mode(self_p->pged_p, PIN_INPUT);
    pin_device_set_mode(self_p->mclrn_p, PIN_INPUT);

    return (0);
}

int icsp_soft_reset(struct icsp_soft_driver_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    move_to_reset(self_p);
    move_to_idle_from_reset(self_p);

    return (0);
}

int icsp_soft_instruction_write(struct icsp_soft_driver_t *self_p,
                                const void *buf_p,
                                size_t number_of_bits)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(buf_p != NULL, EINVAL);

    move_to_shift_ir_from_idle(self_p);

    return (transfer(self_p, NULL, buf_p, number_of_bits, 0));
}

int icsp_soft_data_transfer(struct icsp_soft_driver_t *self_p,
                            void *rxbuf_p,
                            const void *txbuf_p,
                            size_t number_of_bits)
{
    ASSERTN(self_p != NULL, EINVAL);

    int value;

    move_to_capture_dr_from_idle(self_p);

    /* TDO LSB captured during transition. */
    value = clock_bit_in(self_p);

    return (transfer(self_p, rxbuf_p, txbuf_p, number_of_bits, value));
}

int icsp_soft_data_read(struct icsp_soft_driver_t *self_p,
                        void *buf_p,
                        size_t number_of_bits)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(buf_p != NULL, EINVAL);

    return (icsp_soft_data_transfer(self_p, buf_p, NULL, number_of_bits));
}

int icsp_soft_data_write(struct icsp_soft_driver_t *self_p,
                         const void *buf_p,
                         size_t number_of_bits)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(buf_p != NULL, EINVAL);

    return (icsp_soft_data_transfer(self_p, NULL, buf_p, number_of_bits));
}

int icsp_soft_fast_data_transfer(struct icsp_soft_driver_t *self_p,
                                 void *rxbuf_p,
                                 const void *txbuf_p,
                                 size_t number_of_bits)
{
    ASSERTN(self_p != NULL, EINVAL);

    int value;
    int pracc;

    move_to_capture_dr_from_idle(self_p);

    /* PrACC must be 1 for a successful transfer. */
    while (1) {
        pracc = clock_bit_in(self_p);

        if (pracc == 1) {
            break;
        }

        move_to_capture_dr_from_shift_dr(self_p);
    }

    /* First read bit in the data transfer. */
    value = clock_bit_in(self_p);

    return (transfer(self_p, rxbuf_p, txbuf_p, number_of_bits, value));
}

int icsp_soft_make_transition(struct icsp_soft_driver_t *self_p,
                              int transition)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN((transition == 0) || (transition == 1), EINVAL);

    return (clock_bit_tms(self_p, transition));
}

#endif
