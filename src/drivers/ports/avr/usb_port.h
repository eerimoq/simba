/**
 * @file drivers/ports/avr/usb_port.h
 * @version 4.1.0
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

#ifndef __DRIVERS_USB_PORT_H__
#define __DRIVERS_USB_PORT_H__

#define USB_PIPE_STATE_FREE      0
#define USB_PIPE_STATE_ALLOCATED 1

struct usb_pipe_t {
    int id;
    int state;
    struct thrd_t *thrd_p;
};

struct usb_device_t {
    void *drv_p;
    volatile struct sam_uotghs_t *regs_p;
    int id;
    struct usb_pipe_t pipes[10];
};

#endif
