/**
 * @file avr/gnu/settings_port.i
 * @version 7.0.0
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
