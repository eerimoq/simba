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

#ifndef __DRIVERS_XBEE_H__
#define __DRIVERS_XBEE_H__

#include "simba.h"

/* Maximum number of data bytes in a command (excluding escape
   characters and the command identifier). */
#define XBEE_COMMAND_DATA_MAX                              110

#define XBEE_FRAME_ID_NO_ACK                              0x00

/* Command identifiers transmitted to the XBee. */
#define XBEE_COMMAND_ID_TX_REQUEST_64_BIT_ADDRESS         0x00
#define XBEE_COMMAND_ID_TX_REQUEST_16_BIT_ADDRESS         0x01
#define XBEE_COMMAND_ID_AT_COMMAND                        0x08
#define XBEE_COMMAND_ID_AT_COMMAND_QUEUE_PARAMETER_VALUE  0x09

/* Command identifiers received from the XBee. */
#define XBEE_COMMAND_ID_RX_PACKET_64_BIT_ADDRESS          0x80
#define XBEE_COMMAND_ID_RX_PACKET_16_BIT_ADDRESS          0x81
#define XBEE_COMMAND_ID_AT_COMMAND_RESPONSE               0x88
#define XBEE_COMMAND_ID_TX_STATUS                         0x89
#define XBEE_COMMAND_ID_MODEM_STATUS                      0x8a

/* An XBee command. */
struct xbee_command_t {
    uint8_t id;
    struct {
        uint8_t buf[XBEE_COMMAND_DATA_MAX];
        size_t size;
    } data;
};

/* The XBee driver. */
struct xbee_driver_t {
    struct chan_t base;
    struct chan_t *transport_p;
};

/**
 * Initialize XBEE module. This function must be called before calling
 * any other function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
 *
 * @return zero(0) or negative error code.
 */
int xbee_module_init(void);

/**
 * Initialize given driver object from given configuration.
 *
 * @param[in,out] self_p Driver object to initialize.
 * @param[in] transport_p Channel to the Xbee module, often a UART
 *                        driver.
 *
 * @return zero(0) or negative error code.
 */
int xbee_init(struct xbee_driver_t *self_p,
              void *transport_p);

/**
 * Read one XBee command from the XBee module. Blocks until the
 * command is received or an error occurs.
 *
 * @param[in] self_p Initialized driver object.
 * @param[out] command_p Read command.
 *
 * @return zero(0) or negative error code.
 */
int xbee_read(struct xbee_driver_t *self_p,
              struct xbee_command_t *command_p);

/**
 * Write a XBee command to the XBee module. Blocks until the command
 * have been transmitted or an error occurs.
 *
 * @param[in] self_p Initialized driver object.
 * @param[in] command_p Command to write.
 *
 * @return zero(0) or negative error code.
 */
int xbee_write(struct xbee_driver_t *self_p,
               const struct xbee_command_t *command_p);

/**
 * Map given command identifier to a human readable string.
 *
 * @param[in] command_id Command identifier.
 *
 * @return Human readable command identifer string.
 */
const char *xbee_command_id_as_string(uint8_t command_id);

#endif
