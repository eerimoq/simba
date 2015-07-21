/**
 * @file drivers/enc28j60.h
 * @version 1.0
 *
 * @section License
 * Copyright (C) 2014-2015, Erik Moqvist
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERSOCKTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * This file is part of the Simba project.
 */

#include "simba.h"

#ifndef __DRIVERS_ENC28J60_H__
#define __DRIVERS_ENC28J60_H__

struct enc28j60_driver_t {
    struct spi_driver_t *spi_p;     /* SPI driver. */
    struct exti_driver_t *exti_p;   /* Interrupt from device. */
    void (*on_interrupt)(struct enc28j60_driver_t *drv_p,
                         uint8_t status);
    uint16_t next_packet_addr;
    size_t txsize;
};

/**
 * Initialize driver.
 * @return zero(0) or negative error code.
 */
int enc28j60_init(struct enc28j60_driver_t *drv_p,
                  struct spi_driver_t *spi_p,
                  struct exti_driver_t *exti_p,
                  const uint8_t* macaddr_p,
                  void (*on_interrupt)(struct enc28j60_driver_t *drv_p,
                                       uint8_t status));

/**
 * Start device using driver.
 * @param[in] drv_p Driver.
 * @return zero(0) or negative error code.
 */
int enc28j60_start(struct enc28j60_driver_t *drv_p);

/**
 * Stop device using driver.
 * @param[in] drv_p Driver.
 * @return zero(0) or negative error code.
 */
int enc28j60_stop(struct enc28j60_driver_t *drv_p);

/**
 * Begin reading a packet from enc28j60.
 * @param[in] drv_p Driver.
 * @return packet size(>0) or zero(0) on error
 */
size_t enc28j60_read_begin(struct enc28j60_driver_t *drv_p);

/**
 * Read packet payload from enc28j60.
 * @param[in] drv_p Driver.
 * @return packet size(>0) or zero(0)
 */
size_t enc28j60_read(struct enc28j60_driver_t *drv_p, 
                     void *buf_p,
                     size_t size);

/**
 * Finalize packet read.
 * @param[in] drv_p Driver.
 */
void enc28j60_read_end(struct enc28j60_driver_t *drv_p);

/**
 * Begin transmitting a packet.
 * @param[in] drv_p Driver.
 * @return packet size(>0) or zero(0) on error
 */
void enc28j60_write_begin(struct enc28j60_driver_t *drv_p);

/**
 * Write packet payload to enc28j60.
 * @param[in] drv_p Driver.
 * @return packet size(>0) or zero(0)
 */
size_t enc28j60_write(struct enc28j60_driver_t *drv_p,
                      const void *buf_p,
                      size_t size);

/**
 * Finalize packet transmission.
 * @param[in] drv_p Driver.
 * @return void
 */
void enc28j60_write_end(struct enc28j60_driver_t *drv_p);

/**
 * Finalize packet transmission.
 * @param[in] drv_p Driver.
 * @return Number of available packets.
 */
uint8_t enc28j60_get_pkt_cnt(struct enc28j60_driver_t *drv_p);

#endif
