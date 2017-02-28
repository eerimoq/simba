/**
 * @section License
 *
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org/>
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
