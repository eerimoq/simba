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

#include "simba.h"

#if CONFIG_PIN == 1

struct module_t {
    int initialized;
#if CONFIG_FS_CMD_PIN_SET_MODE == 1
    struct fs_command_t cmd_set_mode;
#endif
#if CONFIG_FS_CMD_PIN_READ == 1
    struct fs_command_t cmd_read;
#endif
#if CONFIG_FS_CMD_PIN_WRITE == 1
    struct fs_command_t cmd_write;
#endif
};

#include "pin_port.i"

struct module_t module;

#if CONFIG_FS_CMD_PIN_SET_MODE == 1

static int cmd_set_mode_cb(int argc,
                           const char *argv[],
                           void *out_p,
                           void *in_p,
                           void *arg_p,
                           void *call_arg_p)
{
    int pin;
    int mode;

    if (argc != 3) {
        std_fprintf(out_p, OSTR("Usage: set_mode <pin> <mode>\r\n"));

        return (-EINVAL);
    }

    /* Get pin. */
    pin = board_pin_string_to_device_index(argv[1]);

    if (pin == -1) {
        std_fprintf(out_p, OSTR("%s: bad pin\r\n"), argv[1]);

        return (-ENODEV);
    }

    /* Get mode. */
    if (strcmp(argv[2], "output") == 0) {
        mode = PIN_OUTPUT;
    } else if (strcmp(argv[2], "input") == 0) {
        mode = PIN_INPUT;
    } else {
        std_fprintf(out_p, OSTR("%s: bad mode\r\n"), argv[2]);

        return (-EINVAL);
    }

    pin_device_set_mode(&pin_device[pin], mode);

    return (0);
}

#endif

#if CONFIG_FS_CMD_PIN_READ == 1

static int cmd_read_cb(int argc,
                       const char *argv[],
                       void *out_p,
                       void *in_p,
                       void *arg_p,
                       void *call_arg_p)
{
    int pin;
    int value;

    if (argc != 2) {
        std_fprintf(out_p, OSTR("Usage: read <pin>\r\n"));

        return (-EINVAL);
    }

    /* Get pin. */
    pin = board_pin_string_to_device_index(argv[1]);

    if (pin == -1) {
        std_fprintf(out_p, OSTR("%s: bad pin\r\n"), argv[1]);

        return (-ENODEV);
    }

    value = pin_device_read(&pin_device[pin]);

    if (value == 1) {
        std_fprintf(out_p, OSTR("high\r\n"));
    } else {
        std_fprintf(out_p, OSTR("low\r\n"));
    }

    return (0);
}

#endif

#if CONFIG_FS_CMD_PIN_WRITE == 1

static int cmd_write_cb(int argc,
                        const char *argv[],
                        void *out_p,
                        void *in_p,
                        void *arg_p,
                        void *call_arg_p)
{
    int pin;

    if (argc != 3) {
        std_fprintf(out_p, OSTR("Usage: write <pin> <value>\r\n"));

        return (-EINVAL);
    }

    /* Get pin. */
    pin = board_pin_string_to_device_index(argv[1]);

    if (pin == -1) {
        std_fprintf(out_p, OSTR("%s: bad pin\r\n"), argv[1]);

        return (-ENODEV);
    }

    /* Get mode. */
    if (strcmp(argv[2], "high") == 0) {
        pin_device_write_high(&pin_device[pin]);
    } else if (strcmp(argv[2], "low") == 0) {
        pin_device_write_low(&pin_device[pin]);
    } else {
        std_fprintf(out_p, OSTR("%s: bad value\r\n"), argv[2]);

        return (-EINVAL);
    }

    return (0);
}

#endif

int pin_module_init(void)
{
    /* Return immediately if the module is already initialized. */
    if (module.initialized == 1) {
        return (0);
    }

    module.initialized = 1;

#if CONFIG_FS_CMD_PIN_SET_MODE == 1

    fs_command_init(&module.cmd_set_mode,
                    CSTR("/drivers/pin/set_mode"),
                    cmd_set_mode_cb,
                    NULL);
    fs_command_register(&module.cmd_set_mode);

#endif

#if CONFIG_FS_CMD_PIN_READ == 1

    fs_command_init(&module.cmd_read,
                    CSTR("/drivers/pin/read"),
                    cmd_read_cb,
                    NULL);
    fs_command_register(&module.cmd_read);

#endif

#if CONFIG_FS_CMD_PIN_WRITE == 1

    fs_command_init(&module.cmd_write,
                    CSTR("/drivers/pin/write"),
                    cmd_write_cb,
                    NULL);
    fs_command_register(&module.cmd_write);

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

int pin_is_valid_device(struct pin_device_t *dev_p)
{
    return ((dev_p != NULL)
            && (dev_p >= &pin_device[0])
            && (dev_p < &pin_device[PIN_DEVICE_MAX]));
}

#endif
