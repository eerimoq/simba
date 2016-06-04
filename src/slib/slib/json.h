/**
 * @file slib/json.h
 * @version 0.6.0
 *
 * @section License
 * Copyright (C) 2015-2016, Erik Moqvist
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

/*
  This module is based on the MIT Licensed JSMN implementation.

  http://zserge.com/jsmn.html

  Copyright (c) 2010 Serge A. Zaitsev

  Permission is hereby granted, free of charge, to any person
  obtaining a copy of this software and associated documentation files
  (the "Software"), to deal in the Software without restriction,
  including without limitation the rights to use, copy, modify, merge,
  publish, distribute, sublicense, and/or sell copies of the Software,
  and to permit persons to whom the Software is furnished to do so,
  subject to the following conditions:

  The above copyright notice and this permission notice shall be
  included in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
  ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#ifndef __SLIB_JSON_H__
#define __SLIB_JSON_H__

#include "simba.h"

/**
 * JSON type identifier.
 */
enum json_type_t {
    /** Undefined type. */
    JSON_UNDEFINED = 0,

    /** Object, ``{}``. */
    JSON_OBJECT = 1,

    /** Array, ``[]``. */
    JSON_ARRAY = 2,

    /** String, ``\"...\"``. */
    JSON_STRING = 3,

    /** Other primitive: number, boolean (true/false) or null. */
    JSON_PRIMITIVE = 4
};

enum json_err_t {
    /** Not enough tokens were provided. */
    JSON_ERROR_NOMEM = -1,

    /** Invalid character inside JSON string. */
    JSON_ERROR_INVAL = -2,

    /** The string is not a full JSON packet, more bytes expected. */
    JSON_ERROR_PART = -3
};

/*
 * JSON token description.
 */
struct json_tok_t {
    enum json_type_t type;
    int start;
    int end;
    int size;
#ifdef JSON_PARENT_LINKS
    int parent;
#endif
};

/*
 * JSON parser. Contains an array of token blocks available. Also
 * stores the string being parsed now and current position in that
 * string
 */
struct json_parser_t {
    /** Offset in the JSON string. */
    unsigned int pos;
    /** Next token to allocate. */
    unsigned int toknext;
    /** Superior token node, e.g parent object or array. */
    int toksuper;
};

/**
 * Initialize given JSON parser.
 *
 * @param[in,out] parser_p Parser to initialize.
 *
 * @return zero(0) or negative error code.
 */
int json_init(struct json_parser_t *parser_p);

/**
 * Parse given JSON data string into and array of tokens, each
 * describing a single JSON object.
 *
 * @param[in] parser_p Initialized parser.
 * @param[in] js_p JSON string to parse.
 * @param[in] len JSON string length in bytes.
 * @param[out] tokens_p Array of parsed tokens.
 * @param[in] num_tokens Number of tokens.
 *
 * @return zero(0) or negative error code.
 */
int json_parse(struct json_parser_t *parser_p,
               const char *js_p,
               size_t len,
               struct json_tok_t *tokens_p,
               unsigned int num_tokens);

#endif
