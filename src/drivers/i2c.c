/**
 * @file i2c.c
 * @version 3.1.0
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

#include "i2c_port.i"

#if CONFIG_FS_CMD_I2C_READ == 1

static struct fs_command_t cmd_read;

static int cmd_read_cb(int argc,
                       const char *argv[],
                       chan_t *chout_p,
                       chan_t *chin_p,
                       void *arg_p,
                       void *call_arg_p)
{
    struct i2c_driver_t i2c;
    long value;
    int slave_address;
    uint8_t data;

    if (argc != 2) {
        std_printf(FSTR("Usage: %s <slave address>\r\n"), argv[0]);

        return (-EINVAL);
    }

    if (std_strtol(argv[1], &value) == NULL) {
        std_printf(FSTR("Bad slave address.\r\n"));
        
        return (-EINVAL);
    }

    slave_address = value;
    
    i2c_init(&i2c, &i2c_0_dev, I2C_BAUDRATE_100KBPS, -1);
    i2c_start(&i2c);

    if (i2c_read(&i2c, slave_address, &data, 1) != 1) {
        std_printf(FSTR("Failed to read from slave device 0x%02x.\r\n"),
                   slave_address);
        goto out;
    }

    std_printf(FSTR("0x%02x\r\n"), data);

 out:
    i2c_stop(&i2c);

    return (0);
}

#endif

#if CONFIG_FS_CMD_I2C_WRITE == 1

static struct fs_command_t cmd_write;

static int cmd_write_cb(int argc,
                        const char *argv[],
                        chan_t *chout_p,
                        chan_t *chin_p,
                        void *arg_p,
                        void *call_arg_p)
{
    struct i2c_driver_t i2c;
    long value;
    int slave_address;
    uint8_t data;

    if (argc != 3) {
        std_printf(FSTR("Usage: %s <slave address> <data byte>\r\n"),
                   argv[0]);

        return (-EINVAL);
    }

    if (std_strtol(argv[1], &value) == NULL) {
        std_printf(FSTR("Bad slave address.\r\n"));
        
        return (-EINVAL);
    }

    slave_address = value;

    if (std_strtol(argv[2], &value) == NULL) {
        std_printf(FSTR("Bad data value.\r\n"));

        return (-EINVAL);
    }

    if ((value > 255) || (value < 0)) {
        std_printf(FSTR("Data byte value out of range.\r\n"));

        return (-EINVAL);
    }

    data = value;
    
    i2c_init(&i2c, &i2c_0_dev, I2C_BAUDRATE_100KBPS, -1);
    i2c_start(&i2c);

    if (i2c_write(&i2c, slave_address, &data, 1) != 1) {
        std_printf(FSTR("Failed to write data 0x%02x to slave"
                        " device 0x%02x.\r\n"),
                   data,
                   slave_address);
    }

    i2c_stop(&i2c);

    return (0);
}

#endif

int i2c_module_init()
{
#if CONFIG_FS_CMD_I2C_READ == 1

    fs_command_init(&cmd_read,
                    FSTR("/drivers/i2c/read"),
                    cmd_read_cb,
                    NULL);
    fs_command_register(&cmd_read);

#endif

#if CONFIG_FS_CMD_I2C_WRITE == 1

    fs_command_init(&cmd_write,
                    FSTR("/drivers/i2c/write"),
                    cmd_write_cb,
                    NULL);
    fs_command_register(&cmd_write);

#endif

    return (i2c_port_module_init());
}

int i2c_init(struct i2c_driver_t *self_p,
             struct i2c_device_t *dev_p,
             int baudrate,
             int address)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(dev_p != NULL, EINVAL);

    return (i2c_port_init(self_p, dev_p, baudrate, address));
}

int i2c_start(struct i2c_driver_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    return (i2c_port_start(self_p));
}

int i2c_stop(struct i2c_driver_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    return (i2c_port_stop(self_p));
}

ssize_t i2c_read(struct i2c_driver_t *self_p,
                 int address,
                 void *buf_p,
                 size_t size)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(address < 128, EINVAL);
    ASSERTN(buf_p != NULL, EINVAL);
    ASSERTN(size > 0, EINVAL);

    return (i2c_port_read(self_p, address, buf_p, size));
}

ssize_t i2c_write(struct i2c_driver_t *self_p,
                  int address,
                  const void *buf_p,
                  size_t size)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(address < 128, EINVAL);
    ASSERTN(buf_p != NULL, EINVAL);
    ASSERTN(size > 0, EINVAL);

    return (i2c_port_write(self_p, address, buf_p, size));
}

int i2c_slave_start(struct i2c_driver_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    return (i2c_port_slave_start(self_p));
}

int i2c_slave_stop(struct i2c_driver_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    return (i2c_port_slave_stop(self_p));
}

ssize_t i2c_slave_read(struct i2c_driver_t *self_p,
                       void *buf_p,
                       size_t size)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(buf_p != NULL, EINVAL);
    ASSERTN(size > 0, EINVAL);

    return (i2c_port_slave_read(self_p, buf_p, size));
}

ssize_t i2c_slave_write(struct i2c_driver_t *self_p,
                        const void *buf_p,
                        size_t size)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(buf_p != NULL, EINVAL);
    ASSERTN(size > 0, EINVAL);

    return (i2c_port_slave_write(self_p, buf_p, size));
}
