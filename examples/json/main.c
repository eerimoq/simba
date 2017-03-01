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
