/**
 * @file avr/gnu/setting_port.i
 * @version 5.0.0
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

extern const uint8_t setting_default_area[CONFIG_SETTING_AREA_SIZE];

static FILE *setting_p = NULL;

static int setting_port_module_init(void)
{
    setting_p = fopen(SETTINGS_FILENAME, "r+");

    if (setting_p == NULL) {
        setting_p = fopen(SETTINGS_FILENAME, "w+");
        setting_reset();
    }

    return (0);
}

static ssize_t setting_port_read(void *dst_p, size_t src, size_t size)
{
    fseek(setting_p, src, SEEK_SET);

    return (fread(dst_p, 1, size, setting_p));
}

static ssize_t setting_port_write(size_t dst, const void *src_p, size_t size)
{
    fseek(setting_p, dst, SEEK_SET);

    return (fwrite(src_p, 1, size, setting_p));
}

static ssize_t setting_port_reset()
{
    fseek(setting_p, 0, SEEK_SET);
    fwrite(setting_default_area,
           sizeof(setting_default_area),
           1,
           setting_p);

    return (0);
}
