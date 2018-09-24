/**
 * Initialize ICSP soft module. This function must be called before
 * calling any other function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
 *
 * @return zero(0) or negative error code.
 */

#ifndef __ICSP_SOFT_MOCK_H__
#define __ICSP_SOFT_MOCK_H__

#include "simba.h"

int mock_write_icsp_soft_module_init(int res);

int mock_write_icsp_soft_init(struct pin_device_t *pgec_p,
                              struct pin_device_t *pged_p,
                              struct pin_device_t *mclrn_p,
                              int res);

int mock_write_icsp_soft_start(int res);

int mock_write_icsp_soft_stop(int res);

int mock_write_icsp_soft_reset(int res);

int mock_write_icsp_soft_instruction_write(const void *buf_p,
                                           size_t number_of_bits,
                                           int res);

int mock_write_icsp_soft_data_transfer(void *rxbuf_p,
                                       const void *txbuf_p,
                                       size_t number_of_bits,
                                       int res);

int mock_write_icsp_soft_data_read(void *buf_p,
                                   size_t number_of_bits,
                                   int res);

int mock_write_icsp_soft_data_write(const void *buf_p,
                                    size_t number_of_bits,
                                    int res);

int mock_write_icsp_soft_fast_data_transfer(void *rxbuf_p,
                                            const void *txbuf_p,
                                            size_t number_of_bits,
                                            int res);

int mock_write_icsp_soft_fast_data_read(uint32_t *data_p,
                                        int res);

int mock_write_icsp_soft_fast_data_write(uint32_t data,
                                         int res);

int mock_write_icsp_soft_make_transition(int transition,
                                         int res);

#endif
