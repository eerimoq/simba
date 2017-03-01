/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2016, Erik Moqvist
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

#define SETTINGS_FILENAME "settings.bin"

extern const uint8_t settings_default_area[CONFIG_SETTINGS_AREA_SIZE];

static FILE *settings_p = NULL;

static int settings_port_module_init(void)
{
    settings_p = fopen(SETTINGS_FILENAME, "r+");

    if (settings_p == NULL) {
        settings_p = fopen(SETTINGS_FILENAME, "w+");
        settings_reset();
    }

    return (0);
}

static ssize_t settings_port_read(void *dst_p, size_t src, size_t size)
{
    fseek(settings_p, src, SEEK_SET);

    return (fread(dst_p, 1, size, settings_p));
}

static ssize_t settings_port_write(size_t dst, const void *src_p, size_t size)
{
    fseek(settings_p, dst, SEEK_SET);

    return (fwrite(src_p, 1, size, settings_p));
}

static ssize_t settings_port_reset()
{
    fseek(settings_p, 0, SEEK_SET);
    fwrite(settings_default_area,
           sizeof(settings_default_area),
           1,
           settings_p);

    return (0);
}
