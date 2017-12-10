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
#include "jtag_soft_mock.h"

int mock_write_jtag_soft_module_init(int res)
{
    harness_mock_write("jtag_soft_module_init()",
                       NULL,
                       0);

    harness_mock_write("jtag_soft_module_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(jtag_soft_module_init)()
{
    int res;

    harness_mock_assert("jtag_soft_module_init()",
                        NULL,
                        0);

    harness_mock_read("jtag_soft_module_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_jtag_soft_init(struct pin_device_t *tck_p,
                              struct pin_device_t *tms_p,
                              struct pin_device_t *tdi_p,
                              struct pin_device_t *tdo_p,
                              int res)
{
    harness_mock_write("jtag_soft_init(tck_p)",
                       tck_p,
                       sizeof(*tck_p));

    harness_mock_write("jtag_soft_init(tms_p)",
                       tms_p,
                       sizeof(*tms_p));

    harness_mock_write("jtag_soft_init(tdi_p)",
                       tdi_p,
                       sizeof(*tdi_p));

    harness_mock_write("jtag_soft_init(tdo_p)",
                       tdo_p,
                       sizeof(*tdo_p));

    harness_mock_write("jtag_soft_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(jtag_soft_init)(struct jtag_soft_driver_t *self_p,
                                                struct pin_device_t *tck_p,
                                                struct pin_device_t *tms_p,
                                                struct pin_device_t *tdi_p,
                                                struct pin_device_t *tdo_p)
{
    int res;

    harness_mock_assert("jtag_soft_init(tck_p)",
                        tck_p,
                        sizeof(*tck_p));

    harness_mock_assert("jtag_soft_init(tms_p)",
                        tms_p,
                        sizeof(*tms_p));

    harness_mock_assert("jtag_soft_init(tdi_p)",
                        tdi_p,
                        sizeof(*tdi_p));

    harness_mock_assert("jtag_soft_init(tdo_p)",
                        tdo_p,
                        sizeof(*tdo_p));

    harness_mock_read("jtag_soft_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_jtag_soft_start(int res)
{
    harness_mock_write("jtag_soft_start(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(jtag_soft_start)(struct jtag_soft_driver_t *self_p)
{
    int res;

    harness_mock_read("jtag_soft_start(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_jtag_soft_stop(int res)
{
    harness_mock_write("jtag_soft_stop(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(jtag_soft_stop)(struct jtag_soft_driver_t *self_p)
{
    int res;

    harness_mock_read("jtag_soft_stop(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_jtag_soft_reset(int res)
{
    harness_mock_write("jtag_soft_reset(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(jtag_soft_reset)(struct jtag_soft_driver_t *self_p)
{
    int res;

    harness_mock_read("jtag_soft_reset(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_jtag_soft_instruction_write(const void *buf_p,
                                           size_t number_of_bits,
                                           int res)
{
    harness_mock_write("jtag_soft_instruction_write(buf_p)",
                       buf_p,
                       DIV_CEIL(number_of_bits, 8));

    harness_mock_write("jtag_soft_instruction_write(number_of_bits)",
                       &number_of_bits,
                       sizeof(number_of_bits));

    harness_mock_write("jtag_soft_instruction_write(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(jtag_soft_instruction_write)(struct jtag_soft_driver_t *self_p,
                                                             const void *buf_p,
                                                             size_t number_of_bits)
{
    int res;

    harness_mock_assert("jtag_soft_instruction_write(buf_p)",
                        buf_p,
                        DIV_CEIL(number_of_bits, 8));

    harness_mock_assert("jtag_soft_instruction_write(number_of_bits)",
                        &number_of_bits,
                        sizeof(number_of_bits));

    harness_mock_read("jtag_soft_instruction_write(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_jtag_soft_data_transfer(void *rxbuf_p,
                                       const void *txbuf_p,
                                       size_t number_of_bits,
                                       int res)
{
    harness_mock_write("jtag_soft_data_transfer(): return (rxbuf_p)",
                       rxbuf_p,
                       DIV_CEIL(number_of_bits, 8));

    harness_mock_write("jtag_soft_data_transfer(txbuf_p)",
                       txbuf_p,
                       DIV_CEIL(number_of_bits, 8));

    harness_mock_write("jtag_soft_data_transfer(number_of_bits)",
                       &number_of_bits,
                       sizeof(number_of_bits));

    harness_mock_write("jtag_soft_data_transfer(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(jtag_soft_data_transfer)(struct jtag_soft_driver_t *self_p,
                                                         void *rxbuf_p,
                                                         const void *txbuf_p,
                                                         size_t number_of_bits)
{
    int res;

    harness_mock_read("jtag_soft_data_transfer(): return (rxbuf_p)",
                      rxbuf_p,
                      DIV_CEIL(number_of_bits, 8));

    harness_mock_assert("jtag_soft_data_transfer(txbuf_p)",
                        txbuf_p,
                        DIV_CEIL(number_of_bits, 8));

    harness_mock_assert("jtag_soft_data_transfer(number_of_bits)",
                        &number_of_bits,
                        sizeof(number_of_bits));

    harness_mock_read("jtag_soft_data_transfer(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_jtag_soft_data_read(void *buf_p,
                                   size_t number_of_bits,
                                   int res)
{
    harness_mock_write("jtag_soft_data_read(): return (buf_p)",
                       buf_p,
                       DIV_CEIL(number_of_bits, 8));

    harness_mock_write("jtag_soft_data_read(number_of_bits)",
                       &number_of_bits,
                       sizeof(number_of_bits));

    harness_mock_write("jtag_soft_data_read(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(jtag_soft_data_read)(struct jtag_soft_driver_t *self_p,
                                                     void *buf_p,
                                                     size_t number_of_bits)
{
    int res;

    harness_mock_read("jtag_soft_data_read(): return (buf_p)",
                      buf_p,
                      DIV_CEIL(number_of_bits, 8));

    harness_mock_assert("jtag_soft_data_read(number_of_bits)",
                        &number_of_bits,
                        sizeof(number_of_bits));

    harness_mock_read("jtag_soft_data_read(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_jtag_soft_data_write(const void *buf_p,
                                    size_t number_of_bits,
                                    int res)
{
    harness_mock_write("jtag_soft_data_write(buf_p)",
                       buf_p,
                       DIV_CEIL(number_of_bits, 8));

    harness_mock_write("jtag_soft_data_write(number_of_bits)",
                       &number_of_bits,
                       sizeof(number_of_bits));

    harness_mock_write("jtag_soft_data_write(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(jtag_soft_data_write)(struct jtag_soft_driver_t *self_p,
                                                      const void *buf_p,
                                                      size_t number_of_bits)
{
    int res;

    harness_mock_assert("jtag_soft_data_write(buf_p)",
                        buf_p,
                        DIV_CEIL(number_of_bits, 8));

    harness_mock_assert("jtag_soft_data_write(number_of_bits)",
                        &number_of_bits,
                        sizeof(number_of_bits));

    harness_mock_read("jtag_soft_data_write(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_jtag_soft_make_transition(int transition,
                                         int res)
{
    harness_mock_write("jtag_soft_make_transition(transition)",
                       &transition,
                       sizeof(transition));

    harness_mock_write("jtag_soft_make_transition(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(jtag_soft_make_transition)(struct jtag_soft_driver_t *self_p,
                                                           int transition)
{
    int res;

    harness_mock_assert("jtag_soft_make_transition(transition)",
                        &transition,
                        sizeof(transition));

    harness_mock_read("jtag_soft_make_transition(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
