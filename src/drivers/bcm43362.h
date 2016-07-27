/**
 * @file drivers/bcm43362.h
 * @version 3.1.1
 *
 * @section License
 * Copyright (C) 2014-2016, Erik Moqvist
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

#ifndef __DRIVERS_BCM43362_H__
#define __DRIVERS_BCM43362_H__

#include "simba.h"

#include "lwip/pbuf.h"

struct bcm43362_driver_t {
    struct sdio_driver_t sdio;
};

/**
 * Initialize the BCM43362 module.
 */
int bcm43362_module_init(void);

/**
 * Initialize driver object from given configuration.
 *
 * @param[in] self_p Driver object to be initialized.
 * @param[in] sdio_dev_p SDIO device to use.
 *
 * @return zero(0) or negative error code.
 */
int bcm43362_init(struct bcm43362_driver_t *self_p,
                  struct sdio_device_t *sdio_dev_p);

/**
 * Starts the BCM43362 device using given driver object.
 *
 * After a successful start of the device the application may call
 * `bcm43362_connect()` to connect to an AP.
 *
 * @param[in] self_p Initialized driver object.
 *
 * @return zero(0) or negative error code.
 */
int bcm43362_start(struct bcm43362_driver_t *self_p);

/**
 * Stops the BCM43362 device referenced by given driver object.
 *
 * @param[in] self_p Initialized driver object.
 *
 * @return zero(0) or negative error code.
 */
int bcm43362_stop(struct bcm43362_driver_t *self_p);

/**
 * Connect to an WiFi Access Point (AP) with given SSID and password.
 *
 * @param[in] self_p Initialized driver object.
 * @param[in] ssid_p SSID of the WiFi AP to connect to.
 * @param[in] password_p Password.
 *
 * @return zero(0) or negative error code.
 */
int bcm43362_connect(struct bcm43362_driver_t *self_p,
                     const char *ssid_p,
                     const char *password_p);

/**
 * Disconnect from any connected WiFi AP.
 *
 * @param[in] self_p Initialized driver object.
 *
 * @return zero(0) or negative error code.
 */
int bcm43362_disconnect(struct bcm43362_driver_t *self_p);

/**
 * Read a packet from the BCM43362 device.
 *
 * @param[in] self_p Initialized driver object.
 * @param[in] pbuf_p Buffer to read into.
 * @param[in] size Number of bytes to receive.
 *
 * @return Number of read bytes or negative error code.
 */
ssize_t bcm43362_read(struct bcm43362_driver_t *self_p,
                      struct pbuf *pbuf_p,
                      size_t size);

/**
 * Write given packet to the BCM43362 device to transmit it on the
 * network.
 *
 * This function is normally called by a network interface to send a
 * frame on the network.
 *
 * @param[in] self_p Initialized driver object.
 * @param[in] pbuf_p Buffer to write.
 * @param[in] size Number of bytes to write.
 *
 * @return Number of written bytes or negative error code.
 */
ssize_t bcm43362_write(struct bcm43362_driver_t *self_p,
                       struct pbuf *pbuf_p,
                       size_t size);

#endif
