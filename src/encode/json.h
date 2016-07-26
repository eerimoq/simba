/**
 * @file encode/json.h
 * @version 3.1.0
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

#ifndef __ENCODE_JSON_H__
#define __ENCODE_JSON_H__

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
    const char *buf_p;
    size_t size;
    int num_tokens;
#ifdef JSON_PARENT_LINKS
    int parent;
#endif
};

/*
 * JSON parser. Contains an array of token blocks available. Also
 * stores the string being parsed now and current position in that
 * string
 */
struct json_t {
    /** Offset in the JSON string. */
    unsigned int pos;
    /** Next token to allocate. */
    unsigned int toknext;
    /** Superior token node, e.g parent object or array. */
    int toksuper;
};

/**
 * Initialize given JSON object.
 *
 * @param[in,out] parser_p Object to initialize.
 *
 * @return zero(0) or negative error code.
 */
int json_init(struct json_t *self_p);

/**
 * Encode given JSON tokens into a string.
 *
 * @param[in] parser_p Initialized parser.
 * @param[in] tokens_p Array of tokens to encode.
 * @param[in] num_tokens Number of tokens.
 * @param[out] js_p Encoded null terminated JSON string.
 *
 * @return Encoded string length (not including termination) or
 *         negative error code.
 */
ssize_t json_encode(struct json_t *self_p,
                    struct json_tok_t *tokens_p,
                    unsigned int num_tokens,
                    char *js_p);

/**
 * Decode given JSON data string into and array of tokens, each
 * describing a single JSON object.
 *
 * @param[in] parser_p Initialized parser.
 * @param[in] js_p JSON string to parse.
 * @param[in] len JSON string length in bytes.
 * @param[out] tokens_p Array of parsed tokens.
 * @param[in] num_tokens Number of tokens.
 *
 * @return Number of decoded tokens or negative error code.
 */
int json_decode(struct json_t *self_p,
                const char *js_p,
                size_t len,
                struct json_tok_t *tokens_p,
                unsigned int num_tokens);

/**
 * Initialize a JSON object token.
 *
 * @param[in] num_tokens Number of tokens.
 *
 * @return Initialized object token.
 */
struct json_tok_t json_token_object(int num_tokens);

/**
 * Initialize a JSON array token.
 *
 * @param[in] num_tokens Number of tokens.
 *
 * @return Initialized array token.
 */
struct json_tok_t json_token_array(int num_tokens);

/**
 * Initialize a JSON boolean true token.
 *
 * @return Initialized boolean true token.
 */
struct json_tok_t json_token_true(void);

/**
 * Initialize a JSON boolean false token.
 *
 * @return Initialized boolean false token.
 */
struct json_tok_t json_token_false(void);

/**
 * Initialize a JSON null token.
 *
 * @return Initialized null token.
 */
struct json_tok_t json_token_null(void);

/**
 * Initialize a JSON number (integer/float) token.
 *
 * @param[in] buf_p Number as a string.
 * @param[in] size String length.
 *
 * @return Initialized number token.
 */
struct json_tok_t json_token_number(const char *buf_p,
                                    size_t size);

/**
 * Initialize a JSON string token.
 *
 * @param[in] buf_p String.
 * @param[in] size String length.
 *
 * @return Initialized string token.
 */
struct json_tok_t json_token_string(const char *buf_p,
                                    size_t size);

#endif
