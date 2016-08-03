/**
 * @file pin.c
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

#include "simba.h"

#include "pin_port.i"

#if CONFIG_FS_CMD_PIN_SET_MODE == 1

static struct fs_command_t cmd_set_mode;

static int cmd_set_mode_cb(int argc,
                           const char *argv[],
                           chan_t *out_p,
                           chan_t *in_p,
                           void *arg_p,
                           void *call_arg_p)
{
    int pin;
    int mode;

    if (argc != 3) {
        std_fprintf(out_p, FSTR("Usage: %s <pin> <mode>\r\n"),
                    argv[0]);

        return (-EINVAL);
    }

    /* Get pin. */
    pin = board_pin_string_to_device_index(argv[1]);

    if (pin == -1) {
        std_fprintf(out_p, FSTR("%s: bad pin\r\n"), argv[1]);
        
        return (-EINVAL);
    }

    /* Get mode. */
    if (strcmp(argv[2], "output") == 0) {
        mode = PIN_OUTPUT;
    } else if (strcmp(argv[2], "input") == 0) {
        mode = PIN_INPUT;
    } else {
        std_fprintf(out_p, FSTR("%s: bad mode\r\n"), argv[2]);

        return (-EINVAL);
    }

    pin_device_set_mode(&pin_device[pin], mode);

    return (0);
}

#endif

#if CONFIG_FS_CMD_PIN_READ == 1

static struct fs_command_t cmd_read;

static int cmd_read_cb(int argc,
                       const char *argv[],
                       chan_t *out_p,
                       chan_t *in_p,
                       void *arg_p,
                       void *call_arg_p)
{
    int pin;
    int value;

    if (argc != 2) {
        std_fprintf(out_p, FSTR("Usage: %s <pin>\r\n"), argv[0]);

        return (-EINVAL);
    }

    /* Get pin. */
    pin = board_pin_string_to_device_index(argv[1]);

    if (pin == -1) {
        std_fprintf(out_p, FSTR("%s: bad pin\r\n"), argv[1]);
        
        return (-EINVAL);
    }

    value = pin_device_read(&pin_device[pin]);

    if (value == 1) {
        std_fprintf(out_p, FSTR("high\r\n"));
    } else {
        std_fprintf(out_p, FSTR("low\r\n"));
    }

    return (0);
}

#endif

#if CONFIG_FS_CMD_PIN_WRITE == 1

static struct fs_command_t cmd_write;

static int cmd_write_cb(int argc,
                        const char *argv[],
                        chan_t *out_p,
                        chan_t *in_p,
                        void *arg_p,
                        void *call_arg_p)
{
    int pin;

    if (argc != 3) {
        std_fprintf(out_p, FSTR("Usage: %s <pin> <value>\r\n"), argv[0]);

        return (-EINVAL);
    }

    /* Get pin. */
    pin = board_pin_string_to_device_index(argv[1]);

    if (pin == -1) {
        std_fprintf(out_p, FSTR("%s: bad pin\r\n"), argv[1]);
        
        return (-EINVAL);
    }

    /* Get mode. */
    if (strcmp(argv[2], "high") == 0) {
        pin_device_write_high(&pin_device[pin]);
    } else if (strcmp(argv[2], "low") == 0) {
        pin_device_write_low(&pin_device[pin]);
    } else {
        std_fprintf(out_p, FSTR("Bad value '%s',\r\n"), argv[2]);

        return (-EINVAL);
    }

    return (0);
}

#endif

int pin_module_init(void)
{
#if CONFIG_FS_CMD_PIN_SET_MODE == 1
    
    fs_command_init(&cmd_set_mode,
                    FSTR("/drivers/pin/set_mode"),
                    cmd_set_mode_cb,
                    NULL);
    fs_command_register(&cmd_set_mode);
    
#endif

#if CONFIG_FS_CMD_PIN_READ == 1

    fs_command_init(&cmd_read,
                    FSTR("/drivers/pin/read"),
                    cmd_read_cb,
                    NULL);
    fs_command_register(&cmd_read);

#endif

#if CONFIG_FS_CMD_PIN_WRITE == 1

    fs_command_init(&cmd_write,
                    FSTR("/drivers/pin/write"),
                    cmd_write_cb,
                    NULL);
    fs_command_register(&cmd_write);

#endif

    return (pin_port_module_init());
}

int pin_init(struct pin_driver_t *self_p,
             struct pin_device_t *dev_p,
             int mode)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(dev_p != NULL, EINVAL);

    self_p->dev_p = dev_p;

    return (pin_port_init(self_p, dev_p, mode));
}

int pin_read(struct pin_driver_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    return (pin_port_read(self_p));
}

int pin_write(struct pin_driver_t *self_p, int value)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN((value == 0) || (value == 1), EINVAL);

    return (pin_port_write(self_p, value));
}

int pin_toggle(struct pin_driver_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    return (pin_port_toggle(self_p));
}

int pin_set_mode(struct pin_driver_t *self_p, int mode)
{
    ASSERTN(self_p != NULL, EINVAL);

    return (pin_port_set_mode(self_p, mode));
}
