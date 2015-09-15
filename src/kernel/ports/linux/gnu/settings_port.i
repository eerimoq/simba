/**
 * @file avr/gnu/settings_port.i
 * @version 1.0
 *
 * @section License
 * Copyright (C) 2014-2015, Erik Moqvist
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

#define SETTINGS_FILENAME "simba_settings.bin"

static FILE *settings_p = NULL;

static int settings_port_module_init(void)
{
    settings_p = fopen(SETTINGS_FILENAME, "rb");

    if (settings_p == NULL) {
        fprintf(stderr, "%s: file not found\n", SETTINGS_FILENAME);
        exit(1);
    }

    return (0);
}

static int settings_port_read(void *dst_p, size_t src, size_t size)
{
    fseek(settings_p, src, SEEK_SET);

    return (fread(dst_p, 1, size, settings_p));
}

static int settings_port_write(size_t dst, const void *src_p, size_t size)
{
    fseek(settings_p, dst, SEEK_SET);

    return (fwrite(src_p, 1, size, settings_p));
}
