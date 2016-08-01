/**
 * @file drivers/mcp2515.h
 * @version 4.1.0
 *
 * @section License
 * Copyright (C) 2015-2016, Erik Moqvist
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * This file is part of the Simba project.
 */

#ifndef __DRIVERS_MCP2515_H__
#define __DRIVERS_MCP2515_H__

#include "simba.h"

/* CAN BUS speed. */
#define MCP2515_SPEED_1000KBPS 1000
#define MCP2515_SPEED_500KBPS   500

/* Device mode. */
#define MCP2515_MODE_NORMAL       0x00
#define MCP2515_MODE_LOOPBACK     0x40

/**/
struct mcp2515_frame_t {
    uint32_t id;        /* Frame ID. */
    int size;           /* Number of bytes in data array. */
    int rtr;            /* Remote transmission request. */
    uint32_t timestamp; /* Receive timestamp. */
    uint8_t data[8];    /* Payload. */
};

/* Driver data structure. */
struct mcp2515_driver_t {
    struct spi_driver_t spi;
    struct exti_driver_t exti;
    int mode;
    int speed;
    struct chan_t chout;
    struct chan_t *chin_p;
    struct sem_t isr_sem;
    struct sem_t tx_sem;
    THRD_STACK(stack, 1024);
};

/**
 * Initialize given driver object.
 *
 * @param[out] self_p Driver object to initialize.
 * @param[in] spi_p SPI driver to use.
 * @param[in] cs_p SPI chip select pin.
 * @param[in] exti_p External interrupt tp use.
 * @param[in] chin_p Frames received from the hardware are written to
 *                   this channel.
 * @param[in] mode Device mode.
 * @param[in] speed CAN bus speed in kbps.
 *
 * @return zero(0) or negative error code.
 */
int mcp2515_init(struct mcp2515_driver_t *self_p,
                 struct spi_device_t *spi_p,
                 struct pin_device_t *cs_p,
                 struct exti_device_t *exti_p,
                 chan_t *chin_p,
                 int mode,
                 int speed);

/**
 * Starts the CAN device using given driver object.
 *
 * @param[in] self_p Initialized driver object.
 *
 * @return zero(0) or negative error code.
 */
int mcp2515_start(struct mcp2515_driver_t *self_p);

/**
 * Stops the CAN device referenced by driver object.
 *
 * @param[in] self_p Initialized driver object.
 *
 * @return zero(0) or negative error code.
 */
int mcp2515_stop(struct mcp2515_driver_t *self_p);

/**
 * Read a CAN frame.
 *
 * @param[in] self_p Initialized driver object.
 * @param[out] frame_p Read frame.
 *
 * @return zero(0) or negative error code.
 */
ssize_t mcp2515_read(struct mcp2515_driver_t *self_p,
                     struct mcp2515_frame_t *frame_p);

/**
 * Write a CAN frame.
 *
 * @param[in] self_p Initialized driver object.
 * @param[out] frame_p Frame to write.
 *
 * @return zero(0) or negative error code.
 */
ssize_t mcp2515_write(struct mcp2515_driver_t *self_p,
                      const struct mcp2515_frame_t *frame_p);

#endif
