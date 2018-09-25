/**
 * Initialize ICSP soft module. This function must be called before
 * calling any other function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
 *
 * @return zero(0) or negative error code.
 */

#include "simba.h"
#include "icsp_soft_mock.h"

int mock_write_icsp_soft_module_init(int res)
{
    harness_mock_write("icsp_soft_module_init()",
                       NULL,
                       0);

    harness_mock_write("icsp_soft_module_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(icsp_soft_module_init)()
{
    int res;

    harness_mock_assert("icsp_soft_module_init()",
                        NULL,
                        0);

    harness_mock_read("icsp_soft_module_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_icsp_soft_init(struct pin_device_t *pgec_p,
                              struct pin_device_t *pged_p,
                              struct pin_device_t *mclrn_p,
                              int res)
{
    harness_mock_write("icsp_soft_init(pgec_p)",
                       &pgec_p,
                       sizeof(pgec_p));

    harness_mock_write("icsp_soft_init(pged_p)",
                       &pged_p,
                       sizeof(pged_p));

    harness_mock_write("icsp_soft_init(mclrn_p)",
                       &mclrn_p,
                       sizeof(mclrn_p));

    harness_mock_write("icsp_soft_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(icsp_soft_init)(struct icsp_soft_driver_t *self_p,
                                                struct pin_device_t *pgec_p,
                                                struct pin_device_t *pged_p,
                                                struct pin_device_t *mclrn_p)
{
    int res;

    harness_mock_assert("icsp_soft_init(pgec_p)",
                        &pgec_p,
                        sizeof(pgec_p));

    harness_mock_assert("icsp_soft_init(pged_p)",
                        &pged_p,
                        sizeof(pged_p));

    harness_mock_assert("icsp_soft_init(mclrn_p)",
                        &mclrn_p,
                        sizeof(mclrn_p));

    harness_mock_read("icsp_soft_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_icsp_soft_start(int res)
{
    harness_mock_write("icsp_soft_start(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(icsp_soft_start)(struct icsp_soft_driver_t *self_p)
{
    int res;

    harness_mock_read("icsp_soft_start(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_icsp_soft_stop(int res)
{
    harness_mock_write("icsp_soft_stop(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(icsp_soft_stop)(struct icsp_soft_driver_t *self_p)
{
    int res;

    harness_mock_read("icsp_soft_stop(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_icsp_soft_reset(int res)
{
    harness_mock_write("icsp_soft_reset(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(icsp_soft_reset)(struct icsp_soft_driver_t *self_p)
{
    int res;

    harness_mock_read("icsp_soft_reset(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_icsp_soft_instruction_write(const void *buf_p,
                                           size_t number_of_bits,
                                           int res)
{
    harness_mock_write("icsp_soft_instruction_write(buf_p)",
                       buf_p,
                       DIV_CEIL(number_of_bits, 8));

    harness_mock_write("icsp_soft_instruction_write(number_of_bits)",
                       &number_of_bits,
                       sizeof(number_of_bits));

    harness_mock_write("icsp_soft_instruction_write(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(icsp_soft_instruction_write)(struct icsp_soft_driver_t *self_p,
                                                             const void *buf_p,
                                                             size_t number_of_bits)
{
    int res;

    harness_mock_assert("icsp_soft_instruction_write(buf_p)",
                        buf_p,
                        DIV_CEIL(number_of_bits, 8));

    harness_mock_assert("icsp_soft_instruction_write(number_of_bits)",
                        &number_of_bits,
                        sizeof(number_of_bits));

    harness_mock_read("icsp_soft_instruction_write(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_icsp_soft_data_transfer(void *rxbuf_p,
                                       const void *txbuf_p,
                                       size_t number_of_bits,
                                       int res)
{
    harness_mock_write("icsp_soft_data_transfer(): return (rxbuf_p)",
                       rxbuf_p,
                       DIV_CEIL(number_of_bits, 8));

    harness_mock_write("icsp_soft_data_transfer(txbuf_p)",
                       txbuf_p,
                       DIV_CEIL(number_of_bits, 8));

    harness_mock_write("icsp_soft_data_transfer(number_of_bits)",
                       &number_of_bits,
                       sizeof(number_of_bits));

    harness_mock_write("icsp_soft_data_transfer(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(icsp_soft_data_transfer)(struct icsp_soft_driver_t *self_p,
                                                         void *rxbuf_p,
                                                         const void *txbuf_p,
                                                         size_t number_of_bits)
{
    int res;

    harness_mock_read("icsp_soft_data_transfer(): return (rxbuf_p)",
                      rxbuf_p,
                      DIV_CEIL(number_of_bits, 8));

    harness_mock_assert("icsp_soft_data_transfer(txbuf_p)",
                        txbuf_p,
                        DIV_CEIL(number_of_bits, 8));

    harness_mock_assert("icsp_soft_data_transfer(number_of_bits)",
                        &number_of_bits,
                        sizeof(number_of_bits));

    harness_mock_read("icsp_soft_data_transfer(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_icsp_soft_data_read(void *buf_p,
                                   size_t number_of_bits,
                                   int res)
{
    harness_mock_write("icsp_soft_data_read(): return (buf_p)",
                       buf_p,
                       sizeof(buf_p));

    harness_mock_write("icsp_soft_data_read(number_of_bits)",
                       &number_of_bits,
                       sizeof(number_of_bits));

    harness_mock_write("icsp_soft_data_read(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(icsp_soft_data_read)(struct icsp_soft_driver_t *self_p,
                                                     void *buf_p,
                                                     size_t number_of_bits)
{
    int res;

    harness_mock_read("icsp_soft_data_read(): return (buf_p)",
                      buf_p,
                      sizeof(*buf_p));

    harness_mock_assert("icsp_soft_data_read(number_of_bits)",
                        &number_of_bits,
                        sizeof(number_of_bits));

    harness_mock_read("icsp_soft_data_read(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_icsp_soft_data_write(const void *buf_p,
                                    size_t number_of_bits,
                                    int res)
{
    harness_mock_write("icsp_soft_data_write(buf_p)",
                       buf_p,
                       DIV_CEIL(number_of_bits, 8));

    harness_mock_write("icsp_soft_data_write(number_of_bits)",
                       &number_of_bits,
                       sizeof(number_of_bits));

    harness_mock_write("icsp_soft_data_write(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(icsp_soft_data_write)(struct icsp_soft_driver_t *self_p,
                                                      const void *buf_p,
                                                      size_t number_of_bits)
{
    int res;

    harness_mock_assert("icsp_soft_data_write(buf_p)",
                        buf_p,
                        DIV_CEIL(number_of_bits, 8));

    harness_mock_assert("icsp_soft_data_write(number_of_bits)",
                        &number_of_bits,
                        sizeof(number_of_bits));

    harness_mock_read("icsp_soft_data_write(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_icsp_soft_fast_data_transfer(void *rxbuf_p,
                                            const void *txbuf_p,
                                            size_t number_of_bits,
                                            int res)
{
    harness_mock_write("icsp_soft_fast_data_transfer(): return (rxbuf_p)",
                       rxbuf_p,
                       DIV_CEIL(number_of_bits, 8));

    harness_mock_write("icsp_soft_fast_data_transfer(txbuf_p)",
                       txbuf_p,
                       sizeof(txbuf_p));

    harness_mock_write("icsp_soft_fast_data_transfer(number_of_bits)",
                       &number_of_bits,
                       sizeof(number_of_bits));

    harness_mock_write("icsp_soft_fast_data_transfer(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(icsp_soft_fast_data_transfer)(struct icsp_soft_driver_t *self_p,
                                                              void *rxbuf_p,
                                                              const void *txbuf_p,
                                                              size_t number_of_bits)
{
    int res;

    harness_mock_read("icsp_soft_fast_data_transfer(): return (rxbuf_p)",
                      rxbuf_p,
                      sizeof(*rxbuf_p));

    harness_mock_assert("icsp_soft_fast_data_transfer(txbuf_p)",
                        txbuf_p,
                        sizeof(*txbuf_p));

    harness_mock_assert("icsp_soft_fast_data_transfer(number_of_bits)",
                        &number_of_bits,
                        sizeof(number_of_bits));

    harness_mock_read("icsp_soft_fast_data_transfer(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_icsp_soft_fast_data_read(uint32_t *data_p,
                                        int res)
{
    harness_mock_write("icsp_soft_fast_data_read(): return (data_p)",
                       data_p,
                       sizeof(*data_p));

    harness_mock_write("icsp_soft_fast_data_read(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(icsp_soft_fast_data_read)(struct icsp_soft_driver_t *self_p,
                                                          uint32_t *data_p)
{
    int res;

    harness_mock_read("icsp_soft_fast_data_read(): return (data_p)",
                      data_p,
                      sizeof(*data_p));

    harness_mock_read("icsp_soft_fast_data_read(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_icsp_soft_fast_data_write(uint8_t *buf_p,
                                         size_t size,
                                         int res)
{
    harness_mock_write("icsp_soft_fast_data_write(): return (size)",
                       &size,
                       sizeof(size));

    harness_mock_write("icsp_soft_fast_data_write(buf_p)",
                       buf_p,
                       size);

    harness_mock_write("icsp_soft_fast_data_write(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(icsp_soft_fast_data_write)(struct icsp_soft_driver_t *self_p,
                                                           uint32_t data)
{
    int res;
    uint8_t buf[4];
    size_t size;

    harness_mock_read("icsp_soft_fast_data_write(): return (size)",
                      &size,
                      sizeof(size));

    buf[0] = (data >> 24);
    buf[1] = (data >> 16);
    buf[2] = (data >> 8);
    buf[3] = (data >> 0);

    harness_mock_assert("icsp_soft_fast_data_write(buf_p)",
                        &buf[0],
                        size);


    harness_mock_read("icsp_soft_fast_data_write(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_icsp_soft_make_transition(int transition,
                                         int res)
{
    harness_mock_write("icsp_soft_make_transition(transition)",
                       &transition,
                       sizeof(transition));

    harness_mock_write("icsp_soft_make_transition(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(icsp_soft_make_transition)(struct icsp_soft_driver_t *self_p,
                                                           int transition)
{
    int res;

    harness_mock_assert("icsp_soft_make_transition(transition)",
                        &transition,
                        sizeof(transition));

    harness_mock_read("icsp_soft_make_transition(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
