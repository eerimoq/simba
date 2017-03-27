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

struct module_port_t {
    int stay_in_bootloader;
};

static struct module_port_t module_port;

static int upgrade_port_bootloader_enter()
{
    return (-1);
}

static int upgrade_port_bootloader_stay_set()
{
    module_port.stay_in_bootloader = 1;

    return (0);
}

static int upgrade_port_bootloader_stay_clear()
{
    module_port.stay_in_bootloader = 0;

    return (0);
}

static int upgrade_port_bootloader_stay_get()
{
    return (module_port.stay_in_bootloader);
}

static int upgrade_port_application_enter()
{
    return (-1);
}

static int upgrade_port_application_erase()
{
    return (0);
}

static int upgrade_port_application_is_valid()
{
    return (0);
}

static int upgrade_port_binary_upload_begin()
{
    return (0);
}

static int upgrade_port_binary_upload(const void *buf_p,
                                      size_t size)
{
    return (0);
}

static int upgrade_port_binary_upload_end()
{
    return (0);
}
