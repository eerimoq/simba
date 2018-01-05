/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2017-2018, Erik Moqvist
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
#include "drivers/basic/pin_mock.h"

static int mock_write_clock_low(void)
{
    mock_write_pin_write(0, 0);

    return (0);
}

static int mock_write_clock_high(void)
{
    mock_write_pin_write(1, 0);

    return (0);
}

static int mock_write_clock_pulse(void)
{
    mock_write_clock_low();
    mock_write_clock_high();

    return (0);
}

static int mock_write_move(uint8_t bits,
                           size_t number_of_bits)
{
    size_t i;

    for (i = 0; i < number_of_bits; i++) {
        /* TMS. */
        mock_write_pin_write(((bits >> i) & 1), 0);
        mock_write_clock_pulse();
    }

    return (0);
}

static int mock_write_move_to_reset(void)
{
    return (mock_write_move(0x1f, 5));
}

static int mock_write_move_to_idle_from_reset(void)
{
    return (mock_write_move(0x00, 1));
}

static int mock_write_move_to_shift_ir_from_idle(void)
{
    return (mock_write_move(0x03, 4));
}

static int mock_write_move_to_shift_dr_from_idle(void)
{
    return (mock_write_move(0x01, 3));
}

static int mock_write_move_to_idle_from_shift(void)
{
    return (mock_write_move(0x01, 2));
}

static int mock_write_transfer(uint8_t *rxbuf_p,
                               const uint8_t *txbuf_p,
                               size_t number_of_bits)
{
    size_t i;
    size_t byte_index;
    size_t bit_index;
    int value;

    for (i = 0; i < number_of_bits; i++) {
        byte_index = (i / 8);
        bit_index = (7 - (i % 8));

        if (i == number_of_bits - 1) {
            mock_write_pin_write(1, 0);
        }

        if (txbuf_p != NULL) {
            value = ((txbuf_p[byte_index] >> bit_index) & 1);
            mock_write_pin_write(value, 0);
        }

        mock_write_clock_low();

        if (rxbuf_p != NULL) {
            value = ((rxbuf_p[byte_index] >> bit_index) & 1);
            mock_write_pin_read(value);
        }

        mock_write_clock_high();
    }

    mock_write_move_to_idle_from_shift();

    return (0);
}

static int mock_write_init(void)
{
    mock_write_pin_init(&pin_device[0], PIN_INPUT, 0);
    mock_write_pin_init(&pin_device[1], PIN_INPUT, 0);
    mock_write_pin_init(&pin_device[2], PIN_INPUT, 0);
    mock_write_pin_init(&pin_device[3], PIN_INPUT, 0);

    return (0);
}

static int mock_write_reset(void)
{
    mock_write_move_to_reset();
    mock_write_move_to_idle_from_reset();

    return (0);
}

static int mock_write_start(void)
{
    mock_write_pin_set_mode(PIN_OUTPUT, 0);
    mock_write_pin_set_mode(PIN_OUTPUT, 0);
    mock_write_pin_set_mode(PIN_OUTPUT, 0);

    return (mock_write_reset());
}

static int mock_write_stop(void)
{
    mock_write_move_to_reset();
    mock_write_pin_set_mode(PIN_INPUT, 0);
    mock_write_pin_set_mode(PIN_INPUT, 0);
    mock_write_pin_set_mode(PIN_INPUT, 0);

    return (0);
}

static int test_module_init(void)
{
    BTASSERT(jtag_soft_module_init() == 0);
    BTASSERT(jtag_soft_module_init() == 0);

    return (0);
}

static int test_stop(void)
{
    struct jtag_soft_driver_t jtag;

    mock_write_init();
    mock_write_start();

    BTASSERT(jtag_soft_init(&jtag,
                            &pin_device[0],
                            &pin_device[1],
                            &pin_device[2],
                            &pin_device[3]) == 0);
    BTASSERT(jtag_soft_start(&jtag) == 0);

    mock_write_stop();

    BTASSERT(jtag_soft_stop(&jtag) == 0);

    return (0);
}

static int test_reset(void)
{
    struct jtag_soft_driver_t jtag;

    mock_write_init();
    mock_write_start();

    BTASSERT(jtag_soft_init(&jtag,
                            &pin_device[0],
                            &pin_device[1],
                            &pin_device[2],
                            &pin_device[3]) == 0);
    BTASSERT(jtag_soft_start(&jtag) == 0);

    mock_write_reset();

    BTASSERT(jtag_soft_reset(&jtag) == 0);

    return (0);
}

static int test_instruction_write(void)
{
    struct jtag_soft_driver_t jtag;
    uint8_t instruction;

    mock_write_init();
    mock_write_start();

    BTASSERT(jtag_soft_init(&jtag,
                            &pin_device[0],
                            &pin_device[1],
                            &pin_device[2],
                            &pin_device[3]) == 0);
    BTASSERT(jtag_soft_start(&jtag) == 0);

    instruction = 0x12;

    mock_write_move_to_shift_ir_from_idle();
    mock_write_transfer(NULL, &instruction, 8);

    BTASSERT(jtag_soft_instruction_write(&jtag, &instruction, 8) == 0);

    return (0);
}

static int test_data_transfer(void)
{
    struct jtag_soft_driver_t jtag;
    uint8_t txbuf[2];
    uint8_t rxbuf[2];
    uint8_t actual[2];

    mock_write_init();
    mock_write_start();

    BTASSERT(jtag_soft_init(&jtag,
                            &pin_device[0],
                            &pin_device[1],
                            &pin_device[2],
                            &pin_device[3]) == 0);
    BTASSERT(jtag_soft_start(&jtag) == 0);

    txbuf[0] = 0x12;
    txbuf[1] = 0x34;

    rxbuf[0] = 0x56;
    rxbuf[1] = 0x78;

    mock_write_move_to_shift_dr_from_idle();
    mock_write_transfer(&rxbuf[0], &txbuf[0], 16);

    BTASSERT(jtag_soft_data_transfer(&jtag, &actual[0], &txbuf[0], 16) == 0);
    BTASSERTM(&rxbuf[0], &actual[0], sizeof(rxbuf));

    return (0);
}

static int test_data_write(void)
{
    struct jtag_soft_driver_t jtag;
    uint8_t buf[2];

    mock_write_init();
    mock_write_start();

    BTASSERT(jtag_soft_init(&jtag,
                            &pin_device[0],
                            &pin_device[1],
                            &pin_device[2],
                            &pin_device[3]) == 0);
    BTASSERT(jtag_soft_start(&jtag) == 0);

    buf[0] = 0x34;
    buf[1] = 0x35;

    mock_write_move_to_shift_dr_from_idle();
    mock_write_transfer(NULL, &buf[0], 16);

    BTASSERT(jtag_soft_data_write(&jtag, &buf[0], 16) == 0);

    return (0);
}

static int test_data_read(void)
{
    struct jtag_soft_driver_t jtag;
    uint8_t buf[2];
    uint8_t actual[2];

    mock_write_init();
    mock_write_start();

    BTASSERT(jtag_soft_init(&jtag,
                            &pin_device[0],
                            &pin_device[1],
                            &pin_device[2],
                            &pin_device[3]) == 0);
    BTASSERT(jtag_soft_start(&jtag) == 0);

    buf[0] = 0x34;
    buf[1] = 0x35;

    mock_write_move_to_shift_dr_from_idle();
    mock_write_transfer(&buf[0], NULL, 16);

    BTASSERT(jtag_soft_data_read(&jtag, &actual[0], 16) == 0);
    BTASSERTM(&buf[0], &actual[0], sizeof(buf));

    return (0);
}

static int test_make_transition(void)
{
    struct jtag_soft_driver_t jtag;

    mock_write_init();
    mock_write_start();

    BTASSERT(jtag_soft_init(&jtag,
                            &pin_device[0],
                            &pin_device[1],
                            &pin_device[2],
                            &pin_device[3]) == 0);
    BTASSERT(jtag_soft_start(&jtag) == 0);

    /* Zero transition. */
    mock_write_pin_write(0, 0);
    mock_write_clock_pulse();

    BTASSERT(jtag_soft_make_transition(&jtag, 0) == 0);

    /* One transition. */
    mock_write_pin_write(1, 0);
    mock_write_clock_pulse();

    BTASSERT(jtag_soft_make_transition(&jtag, 1) == 0);

    return (0);
}

int main()
{
    struct harness_testcase_t testcases[] = {
        { test_module_init, "test_module_init" },
        { test_stop, "test_stop" },
        { test_reset, "test_reset" },
        { test_instruction_write, "test_instruction_write" },
        { test_data_transfer, "test_data_transfer" },
        { test_data_write, "test_data_write" },
        { test_data_read, "test_data_read" },
        { test_make_transition, "test_make_transition" },
        { NULL, NULL }
    };

    sys_start();

    harness_run(testcases);

    return (0);
}
