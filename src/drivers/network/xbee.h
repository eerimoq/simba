/*
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

#ifndef __DRIVERS_XBEE_H__
#define __DRIVERS_XBEE_H__

#include "simba.h"

/* Maximum number of data bytes in a frame (excluding escape
   characters and the frame type). */
#define XBEE_DATA_MAX                             CONFIG_XBEE_DATA_MAX

#define XBEE_FRAME_ID_NO_ACK                                      0x00

/* Frame types transmitted to the XBee. */
#define XBEE_FRAME_TYPE_TX_REQUEST_64_BIT_ADDRESS                 0x00
#define XBEE_FRAME_TYPE_TX_REQUEST_16_BIT_ADDRESS                 0x01
#define XBEE_FRAME_TYPE_AT_COMMAND                                0x08
#define XBEE_FRAME_TYPE_AT_COMMAND_QUEUE_PARAMETER_VALUE          0x09
#define XBEE_FRAME_TYPE_ZIGBEE_TRANSMIT_REQUEST                   0x10
#define XBEE_FRAME_TYPE_EXPLICIT_ADDRESSING_ZIGBEE_COMMAND_FRAME  0x11
#define XBEE_FRAME_TYPE_REMOTE_COMMAND_REQUEST                    0x17
#define XBEE_FRAME_TYPE_CREATE_SOURCE_ROUTE                       0x21

/* Frame types received from the XBee. */
#define XBEE_FRAME_TYPE_RX_PACKET_64_BIT_ADDRESS                  0x80
#define XBEE_FRAME_TYPE_RX_PACKET_16_BIT_ADDRESS                  0x81
#define XBEE_FRAME_TYPE_RX_PACKET_64_BIT_ADDRESS_IO               0x82
#define XBEE_FRAME_TYPE_RX_PACKET_16_BIT_ADDRESS_IO               0x83
#define XBEE_FRAME_TYPE_AT_COMMAND_RESPONSE                       0x88
#define XBEE_FRAME_TYPE_TX_STATUS                                 0x89
#define XBEE_FRAME_TYPE_MODEM_STATUS                              0x8a
#define XBEE_FRAME_TYPE_ZIGBEE_TRANSMIT_STATUS                    0x8b
#define XBEE_FRAME_TYPE_ZIGBEE_RECEIVE_PACKET_AO_0                0x90
#define XBEE_FRAME_TYPE_ZIGBEE_EXPLICIT_RX_INDICATOR_AO_1         0x91
#define XBEE_FRAME_TYPE_ZIGBEE_IO_DATA_SAMPLE_RX_INDICATOR        0x92
#define XBEE_FRAME_TYPE_XBEE_SENSOR_READ_INDICATOR_AO_0           0x94
#define XBEE_FRAME_TYPE_NODE_IDENTIFICATION_INDICATOR_AO_0        0x95
#define XBEE_FRAME_TYPE_REMOTE_COMMAND_RESPONSE                   0x97
#define XBEE_FRAME_TYPE_EXTENDED_MODEM_STATUS                     0x98
#define XBEE_FRAME_TYPE_OVER_THE_AIR_FIRMWARE_UPDATE_STATUS       0xa0
#define XBEE_FRAME_TYPE_ROUTE_RECORD_INDICATOR                    0xa1
#define XBEE_FRAME_TYPE_MANY_TO_ONE_ROUTE_REQUEST_INDICATOR       0xa3

#define XBEE_PIN_DIO0                                             0
#define XBEE_PIN_DIO1                                             1
#define XBEE_PIN_DIO2                                             2
#define XBEE_PIN_DIO3                                             3
#define XBEE_PIN_DIO4                                             4
#define XBEE_PIN_DIO5                                             5
#define XBEE_PIN_DIO6                                             6
#define XBEE_PIN_DIO7                                             7
#define XBEE_PIN_DIO8                                             8

#define XBEE_PIN_AD0                                              0
#define XBEE_PIN_AD1                                              1
#define XBEE_PIN_AD2                                              2
#define XBEE_PIN_AD3                                              3

#define XBEE_PIN_MODE_DISABLED                                    0
#define XBEE_PIN_MODE_ADC                                         2
#define XBEE_PIN_MODE_INPUT                                       3
#define XBEE_PIN_MODE_OUTPUT_LOW                                  4
#define XBEE_PIN_MODE_OUTPUT_HIGH                                 5

/* An XBee frame (without crc). */
struct xbee_frame_t {
    uint8_t type;
    struct {
        uint8_t buf[XBEE_DATA_MAX];
        size_t size;
    } data;
};

/* The XBee driver. */
struct xbee_driver_t {
    struct {
        struct chan_t *chin_p;
        struct chan_t *chout_p;
    } transport;
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
int xbee_module_init(void);

/**
 * Initialize given driver object from given configuration.
 *
 * @param[in,out] self_p Driver object to initialize.
 * @param[in] chin_p Input channel from the XBee module, often a UART
 *                   driver.
 * @param[in] chout_p Output channel to the XBee module, often a UART
 *                    driver output channel.
 *
 * @return zero(0) or negative error code.
 */
int xbee_init(struct xbee_driver_t *self_p,
              void *chin_p,
              void *chout_p);

/**
 * Read a frame from the XBee module. Blocks until a frame is received
 * or an error occurs.
 *
 * @param[in] self_p Initialized driver object.
 * @param[out] frame_p Frame to read into.
 *
 * @return zero(0) or negative error code.
 */
int xbee_read(struct xbee_driver_t *self_p,
              struct xbee_frame_t *frame_p);

/**
 * Write given frame to the XBee module. Blocks until the frame have
 * been transmitted or an error occurs.
 *
 * @param[in] self_p Initialized driver object.
 * @param[in] frame_p Frame to write.
 *
 * @return zero(0) or negative error code.
 */
int xbee_write(struct xbee_driver_t *self_p,
               const struct xbee_frame_t *frame_p);

/**
 * Decode given frame and write it as a human readable string to given
 * channel.
 *
 * @param[in] chan_p Channel to write the human readable string to.
 * @param[in] frame_p Frame to decode.
 *
 * @return zero(0) or negative error code.
 */
int xbee_print_frame(void *chan_p, struct xbee_frame_t *frame_p);

/**
 * Map given frame type to a human readable string.
 *
 * @param[in] frame_type Frame type.
 *
 * @return Human readable frame type string or NULL if given frame
 *         type is unknown.
 */
const char *xbee_frame_type_as_string(uint8_t frame_type);

/**
 * Map given TX status to a human readable string.
 *
 * @param[in] tx_status TX status.
 *
 * @return Human readable TX status string or NULL if given TX status
 *         is unknown.
 */
const char *xbee_tx_status_as_string(uint8_t tx_status);

/**
 * Map given modem status to a human readable string.
 *
 * @param[in] modem_status Modem status.
 *
 * @return Human readable modem status string or NULL if given modem
 *         status is unknown.
 */
const char *xbee_modem_status_as_string(uint8_t modem_status);

/**
 * Map given AT command response status to a human readable string.
 *
 * @param[in] response_status AT command response status.
 *
 * @return Human readable AT command response status string or NULL if
 *         given response status is unknown.
 */
const char *xbee_at_command_response_status_as_string(uint8_t response_status);

#endif
