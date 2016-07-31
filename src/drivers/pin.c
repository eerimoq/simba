/**
 * @file pin.c
 * @version 4.0.0
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

#if (CONFIG_FS_CMD_PIN_INIT == 1) || (CONFIG_FS_CMD_PIN_WRITE == 1)

static struct pin_driver_t drivers[PIN_DEVICE_MAX];

#endif

#if CONFIG_FS_CMD_PIN_INIT == 1

static struct fs_command_t cmd_init;

static int cmd_init_cb(int argc,
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
        std_fprintf(out_p, FSTR("Bad pin '%s',\r\n"), argv[1]);
        
        return (-EINVAL);
    }

    /* Get mode. */
    if (strcmp(argv[2], "output") == 0) {
        mode = PIN_OUTPUT;
    } else if (strcmp(argv[2], "input") == 0) {
        mode = PIN_INPUT;
    } else {
        std_fprintf(out_p, FSTR("Bad mode '%s',\r\n"), argv[2]);

        return (-EINVAL);
    }

    pin_init(&drivers[pin], &pin_device[pin], mode);

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
    int value;

    if (argc != 3) {
        std_fprintf(out_p, FSTR("Usage: %s <pin> <high/low>\r\n"),
                    argv[0]);

        return (-EINVAL);
    }

    /* Get pin. */
    pin = board_pin_string_to_device_index(argv[1]);

    if (pin == -1) {
        std_fprintf(out_p, FSTR("Bad pin '%s',\r\n"), argv[1]);
        
        return (-EINVAL);
    }

    /* Get mode. */
    if (strcmp(argv[2], "high") == 0) {
        value = 1;
    } else if (strcmp(argv[2], "low") == 0) {
        value = 0;
    } else {
        std_fprintf(out_p, FSTR("Bad value '%s',\r\n"), argv[2]);

        return (-EINVAL);
    }

    pin_write(&drivers[pin], value);

    return (0);
}

#endif

int pin_module_init(void)
{
#if CONFIG_FS_CMD_PIN_INIT == 1
    
    fs_command_init(&cmd_init,
                    FSTR("/drivers/pin/init"),
                    cmd_init_cb,
                    NULL);
    fs_command_register(&cmd_init);
    
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
