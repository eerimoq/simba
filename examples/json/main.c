/**
 * @file main.c
 * @version 6.0.0
 *
 * @section License
 * Copyright (C) 2016, Erik Moqvist
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

static const char text[] =
    "{"
    "    \"boolean\": true,"
    "    \"empty\": null,"
    "    \"string\": \"igelkott\","
    "    \"array\": [3, [], {}],"
    "    \"object\": {}"
    "}";

int main()
{
    int number_of_tokens;
    struct json_tok_t tokens[64];
    
    sys_start();

    number_of_tokens = json_parse(text,
                                  strlen(text),
                                  tokens,
                                  membersof(tokens));
    
    if (number_of_tokens <= 0) {
        std_printf(FSTR("Failed to decode JSON text with error %d.\r\n"),
                   number_of_tokens);
        return (-1);
    }
    
    /* Dump all. */
    std_printf(FSTR("all = "));
    json_dump(sys_get_stdout(), tokens, number_of_tokens);
    std_printf(FSTR("\r\n"));
    
    /* Dump the array. */
    std_printf(FSTR("array = "));
    json_dump(sys_get_stdout(), &tokens[8], number_of_tokens - 8);
    std_printf(FSTR("\r\n"));
    
    return (0);
}
