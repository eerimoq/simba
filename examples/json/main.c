/**
 * @file main.c
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
    struct json_t json;
    struct json_tok_t tokens[64];
    
    sys_start();

    json_init(&json, tokens, membersof(tokens));
    json_parse(&json, text, strlen(text));
        
    /* Dump 'root'. */
    std_printf(FSTR("root             = "));
    json_dump(&json, NULL, sys_get_stdout());
    std_printf(FSTR("\r\n"));
    
    /* Dump 'root["array"]'. */
    std_printf(FSTR("root[\"array\"]    = "));
    json_dump(&json,
              json_object_get(&json, "array", json_root(&json)),
              sys_get_stdout());
    std_printf(FSTR("\r\n"));

    /* Dump 'root["array"][1]'. */
    std_printf(FSTR("root[\"array\"][1] = "));
    json_dump(&json,
              json_array_get(&json,
                             1,
                             json_object_get(&json, "array", json_root(&json))),
              sys_get_stdout());
    
    return (0);
}
