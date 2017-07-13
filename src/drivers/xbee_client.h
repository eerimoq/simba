/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2017, Erik Moqvist
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

#ifndef __DRIVERS_XBEE_CLIENT_H__
#define __DRIVERS_XBEE_CLIENT_H__

#include "simba.h"

#define XBEE_CLIENT_PIN_OUTPUT                                      1
#define XBEE_CLIENT_PIN_INPUT                                       2
#define XBEE_CLIENT_PIN_ADC                                         3

/* Configure ``xbee_client_init()`` for non-blocking read, returning
   -EAGAIN immediately if no frame is available. */
#define XBEE_CLIENT_NON_BLOCKING_READ                             0x1

/* Give this flag to ``xbee_client_write_to()`` to write a packet
   without requesting an acknowledgement from the XBee. */
#define XBEE_CLIENT_NO_ACK                                        0x1

enum xbee_client_address_type_t {
    xbee_client_address_type_invalid_t = 0,
    xbee_client_address_type_16_bits_t,
    xbee_client_address_type_64_bits_t
};

struct xbee_client_address_t {
    enum xbee_client_address_type_t type;
    uint8_t buf[8];
};

/* The XBee client. */
struct xbee_client_t {
    struct queue_t chin;
    struct xbee_driver_t driver;
    uint8_t frame_id;
    struct {
        uint8_t value;
    } pins;
    struct {
        struct {
            struct sem_t sem;
            struct xbee_frame_t *frame_p;
            void *buf_p;
            size_t size;
            struct thrd_t *thrd_p;
        } rx;
        struct sem_t sem;
    } rpc;
#if CONFIG_XBEE_CLIENT_DEBUG_LOG_MASK > -1
    struct log_object_t log;
#endif
};

/**
 * Initialize the xbee module. This function must be called before
 * calling any other function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
 *
 * @return zero(0) or negative error code.
 */
int xbee_client_module_init(void);

/**
 * Initialize given driver object from given configuration.
 *
 * @param[in,out] self_p Driver object to initialize.
 * @param[in] chin_p Input channel from the XBee module, often a UART
 *                   driver.
 * @param[in] chout_p Output channel to the XBee module, often a UART
 *                    driver output channel.
 * @param[in] buf_p Frame reception buffer.
 * @param[in] size Frame reception buffer size in words.
 * @param[in] flags Client configuration flags. May be any combination
 *                  of ``XBEE_CLIENT_NON_BLOCKING_READ``.
 *
 * @return zero(0) or negative error code.
 */
int xbee_client_init(struct xbee_client_t *self_p,
                     void *chin_p,
                     void *chout_p,
                     void *buf_p,
                     size_t size,
                     int flags);

/**
 * The client thread entry function.
 *
 * @param[in] self_p Initialized client object.
 *
 * @return zero(0) or negative error code.
 */
void *xbee_client_main(void *arg_p);

/**
 * Read data from one RX packet into given buffer. The sender 16 or 64
 * bits address is written to ``address_p``.
 *
 * @param[in] self_p Initialized client object.
 * @param[out] buf_p Buffer to read into.
 * @param[in] size Number of bytes to read.
 * @param[out] address_p Sender address.
 *
 * @return Number of bytes read or negative error code.
 */
ssize_t xbee_client_read_from(struct xbee_client_t *self_p,
                              void *buf_p,
                              size_t size,
                              struct xbee_client_address_t *address_p);

/**
 * Create a TX packet of given data and write it to given 16 or 64
 * bits XBee address.
 *
 * @param[in] self_p Initialized client object.
 * @param[in] buf_p Buffer to write.
 * @param[in] size Number of bytes to write.
 * @param[in] flags Read flags.
 * @param[in] flags May be any combination of ``XBEE_CLIENT_NO_ACK``.
 * @param[out] address_p Receiver address.
 *
 * @return Number of bytes written or negative error code.
 */
ssize_t xbee_client_write_to(struct xbee_client_t *self_p,
                             const void *buf_p,
                             size_t size,
                             int flags,
                             struct xbee_client_address_t *address_p);

/**
 * Configure given pin to given mode.
 *
 * @param[in] self_p Initialized client object.
 * @param[in] pin Pin to set mode for.
 * @param[in] mode Pin mode to set.
 *
 * @return zero(0) or negative error code.
 */
int xbee_client_pin_set_mode(struct xbee_client_t *self_p,
                             int pin,
                             int mode);

/**
 * Read the current value of given pin.
 *
 * @param[in] self_p Initialized client object.
 * @param[in] pin Pin to read from.
 *
 * @return zero(0), one(1) or negative error code.
 */
int xbee_client_pin_read(struct xbee_client_t *self_p,
                         int pin);

/**
 * Write given value to given pin.
 *
 * @param[in] self_p Initialized client object.
 * @param[in] pin Pin to write to.
 * @param[in] value ``1`` for high and ``0`` for low output.
 *
 * @return zero(0) or negative error code.
 */
int xbee_client_pin_write(struct xbee_client_t *self_p,
                          int pin,
                          int value);

/**
 * Toggle the pin output value (high/low).
 *
 * @param[in] self_p Initialized client object.
 * @param[in] pin Pin to toggle.
 *
 * @return zero(0) or negative error code.
 */
int xbee_client_pin_toggle(struct xbee_client_t *self_p,
                           int pin);

/**
 * Start a synchronous convertion of an analog signal a digital
 * sample.
 *
 * @param[in] self_p Initialized client object.
 * @param[in] pin ADC pin to convert.
 *
 * @return Sample or negative error code.
 */
int xbee_client_pin_convert(struct xbee_client_t *self_p,
                            int pin);

/**
 * Execute given AT command and store its read parameter in given
 * buffer.
 *
 * @param[in] self_p Initialized client object.
 * @param[in] command_p Two letters AT command.
 * @param[out] parameter_p Parameter buffer to read into.
 * @param[in] size Buffer size in bytes.
 *
 * @return Number of read bytes or negative error code.
 */
ssize_t xbee_client_at_command_read(struct xbee_client_t *self_p,
                                    const char *command_p,
                                    uint8_t *parameter_p,
                                    size_t size);

/**
 * Execute given AT command with given parameter.
 *
 * @param[in] self_p Initialized client object.
 * @param[in] command_p Two letters AT command.
 * @param[in] parameter_p Parameter buffer to write.
 * @param[in] size Buffer size in bytes.
 *
 * @return Number of read bytes or negative error code.
 */
ssize_t xbee_client_at_command_write(struct xbee_client_t *self_p,
                                     const char *command_p,
                                     const uint8_t *parameter_p,
                                     size_t size);

/**
 * Execute given AT command and store its read 8 bits parameter in
 * given variable.
 *
 * @param[in] self_p Initialized client object.
 * @param[in] command_p Two letters AT command.
 * @param[out] parameter_p Read parameter.
 *
 * @return zero(0) or negative error code.
 */
int xbee_client_at_command_read_u8(struct xbee_client_t *self_p,
                                   const char *command_p,
                                   uint8_t *parameter_p);

/**
 * Execute given AT command with given 8 bits parameter.
 *
 * @param[in] self_p Initialized client object.
 * @param[in] command_p Two letters AT command.
 * @param[in] parameter Parameter to write.
 *
 * @return zero(0) or negative error code.
 */
int xbee_client_at_command_write_u8(struct xbee_client_t *self_p,
                                    const char *command_p,
                                    uint8_t parameter);

/**
 * Execute given AT command and store its read 16 bits parameter in
 * given variable.
 *
 * @param[in] self_p Initialized client object.
 * @param[in] command_p Two letters AT command.
 * @param[out] parameter_p Read parameter.
 *
 * @return zero(0) or negative error code.
 */
int xbee_client_at_command_read_u16(struct xbee_client_t *self_p,
                                    const char *command_p,
                                    uint16_t *parameter_p);

/**
 * Execute given AT command with given 16 bits parameter.
 *
 * @param[in] self_p Initialized client object.
 * @param[in] command_p Two letters AT command.
 * @param[in] parameter Parameter to write.
 *
 * @return zero(0) or negative error code.
 */
int xbee_client_at_command_write_u16(struct xbee_client_t *self_p,
                                     const char *command_p,
                                     uint16_t parameter);

/**
 * Execute given AT command and store its read 32 bits parameter in
 * given variable.
 *
 * @param[in] self_p Initialized client object.
 * @param[in] command_p Two letters AT command.
 * @param[out] parameter_p Read parameter.
 *
 * @return zero(0) or negative error code.
 */
int xbee_client_at_command_read_u32(struct xbee_client_t *self_p,
                                    const char *command_p,
                                    uint32_t *parameter_p);

/**
 * Execute given AT command with given 32 bits parameter.
 *
 * @param[in] self_p Initialized client object.
 * @param[in] command_p Two letters AT command.
 * @param[in] parameter Parameter to write.
 *
 * @return zero(0) or negative error code.
 */
int xbee_client_at_command_write_u32(struct xbee_client_t *self_p,
                                     const char *command_p,
                                     uint32_t parameter);

/**
 * Format and print given address to given channel.
 *
 * @param[in] chan_p Channel to print to.
 * @param[in] address_p Address to print.
 *
 * @return zero(0) or negative error code.
 */
int xbee_client_print_address(void *chan_p,
                              struct xbee_client_address_t *address_p);

#endif
