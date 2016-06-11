/**
 * @file drivers/linux/pin_port.i
 * @version 0.6.0
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

static struct fs_command_t cmd_init;
static struct fs_command_t cmd_write;
static struct pin_driver_t drivers[PIN_DEVICE_MAX];

static int pin_str_to_pin(const char *pin_p)
{
    int gpio;
    long bit;

    /* Get gpio and bit. */
    if (pin_p[0] != 'p') {
        return (-1);
    }

    if ((pin_p[1] < 'a') || (pin_p[1] > 'd')) {
        return (-1);
    }

    gpio = (pin_p[1] - 'a');

    if (std_strtol(&pin_p[2], &bit) != 0) {
        return (-1);
    }

    if (bit < 0) {
        return (-1);
    }

    if ((gpio == 'd' && bit > 3) || (bit > 15)) {
        return (-1);
    }

    return (16 * gpio + bit);
}

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
    pin = pin_str_to_pin(argv[1]);

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
    pin = pin_str_to_pin(argv[1]);

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

static int pin_port_module_init(void)
{
    fs_command_init(&cmd_init,
                    FSTR("/drivers/pin/init"),
                    cmd_init_cb,
                    NULL);
    fs_command_register(&cmd_init);

    fs_command_init(&cmd_write,
                    FSTR("/drivers/pin/write"),
                    cmd_write_cb,
                    NULL);
    fs_command_register(&cmd_write);

    return (0);
}

static int pin_port_init(struct pin_driver_t *self_p,
                         const struct pin_device_t *dev,
                         int mode)
{
    return (pin_set_mode(self_p, mode));
}

static int pin_port_read(struct pin_driver_t *self_p)
{
    return (0);
}

static int pin_port_write(struct pin_driver_t *self_p, int value)
{
    if (value != 0) {
        self_p->dev_p->regs_p->BSRR = (1 << self_p->dev_p->bit);
    } else {
        self_p->dev_p->regs_p->BSRR = (1 << (16 + self_p->dev_p->bit));
    }

    return (0);
}

static int pin_port_toggle(struct pin_driver_t *self_p)
{
    int value;

    value = ((self_p->dev_p->regs_p->ODR & (1 << self_p->dev_p->bit)) != 0);

    return (pin_port_write(self_p, !value));
}

static int pin_port_set_mode(struct pin_driver_t *self_p, int mode)
{
    int shift;
    volatile uint32_t *cr_p;
    struct pin_device_t  *dev_p;

    dev_p = self_p->dev_p;
    shift = (4 * dev_p->bit);
    cr_p = &dev_p->regs_p->CRL;

    if (dev_p->bit >= 8) {
        shift -= 32;
        cr_p++;
    }

    if (mode == PIN_OUTPUT) {
        *cr_p = bits_insert_32(*cr_p, shift, 4, 0x1);
    } else {
        *cr_p = bits_insert_32(*cr_p, shift, 4, 0x4);
    }

    return (0);
}
