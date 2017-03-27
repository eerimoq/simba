/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2017, Erik Moqvist
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
    /* Token type. */
    enum json_type_t type;
    /* Token value buffer. */
    const char *buf_p;
    /* Size of the token buffer. */
    size_t size;
    /* Number of children of this token. Not recursive. */
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
    /** Array of tokens. */
    struct json_tok_t *tokens_p;
    /** Number of tokens in the tokens array. */
    int num_tokens;
};

 /**
  * Initialize given JSON object. The JSON object must be initialized
  * before it can be used to parse and dump JSON data.
  *
  * @param[out] self_p JSON object to initialize.
  * @param[in] tokens_p Array of tokens. The tokens are either filled
  *                     by the parsing function `json_parse()`, or
  *                     already filled by the user when calling this
  *                     function. The latter can be used to dump the
  *                     tokens as a string by calling `json_dump()` or
  *                     `json_dumps()`.
  * @param[in] num_tokens Number of tokens in the array.
  *
  * @return zero(0) or negative error code.
  */
int json_init(struct json_t *self_p,
              struct json_tok_t *tokens_p,
              int num_tokens);

/**
 * Parse given JSON data string into and array of tokens, each
 * describing a single JSON object.
 *
 * @param[in] self_p JSON object.
 * @param[in] js_p JSON string to parse.
 * @param[in] len JSON string length in bytes.
 *
 * @return Number of decoded tokens or negative error code.
 */
int json_parse(struct json_t *self_p,
               const char *js_p,
               size_t len);

/**
 * Format and write given JSON tokens into a string.
 *
 * @param[in] self_p JSON object.
 * @param[in] tokens_p Root token to dump. Set to NULL to dump the
 *                     whole object.
 * @param[out] js_p Dumped null terminated JSON string.
 *
 * @return Dumped string length (not including termination) or
 *         negative error code.
 */
ssize_t json_dumps(struct json_t *self_p,
                   struct json_tok_t *tokens_p,
                   char *js_p);

/**
 * Format and write given JSON tokens to given channel.
 *
 * @param[in] self_p JSON object.
 * @param[in] tokens_p Root token to dump. Set to NULL to dump the
 *                     whole object.
 * @param[in] out_p Channel to dump the null terminated JSON string to.
 *
 * @return Dumped string length (not including termination) or
 *         negative error code.
 */
ssize_t json_dump(struct json_t *self_p,
                  struct json_tok_t *tokens_p,
                  void *out_p);

/**
 * Get the root token.
 *
 * @param[in] self_p JSON object.
 *
 * @return The root token or NULL on failure.
 */
struct json_tok_t *json_root(struct json_t *self_p);

/**
 * Get the value the string token with given key.
 *
 * @param[in] self_p JSON object.
 * @param[in] key_p Key of the value to get.
 * @param[in] object_p The object to get the value from.
 *
 * @return Token or NULL on error.
 */
struct json_tok_t *json_object_get(struct json_t *self_p,
                                   const char *key_p,
                                   struct json_tok_t *object_p);

/**
 * Get the value of the primitive token with given key.
 *
 * @param[in] self_p JSON object.
 * @param[in] key_p Key of the value to get.
 * @param[in] object_p The object to get the value from.
 *
 * @return Token or NULL on error.
 */
struct json_tok_t *json_object_get_primitive(struct json_t *self_p,
                                             const char *key_p,
                                             struct json_tok_t *object_p);

/**
 * Get the token of given array index.
 *
 * @param[in] self_p JSON object.
 * @param[in] index Index to get.
 * @param[in] array_p The array to get the element from.
 *
 * @return Token or NULL on error.
 */
struct json_tok_t *json_array_get(struct json_t *self_p,
                                  int index,
                                  struct json_tok_t *array_p);

/**
 * Initialize a JSON object token.
 *
 * @param[out] token_p Initialized token.
 * @param[in] num_keys Number of keys in the object.
 */
void json_token_object(struct json_tok_t *token_p,
                       int num_keys);

/**
 * Initialize a JSON array token.
 *
 * @param[out] token_p Initialized token.
 * @param[in] num_elements Number of array elements.
 */
void json_token_array(struct json_tok_t *token_p,
                      int num_elements);

/**
 * Initialize a JSON boolean true token.
 *
 * @param[out] token_p Initialized token.
 */
void json_token_true(struct json_tok_t *token_p);

/**
 * Initialize a JSON boolean false token.
 *
 * @param[out] token_p Initialized token.
 */
void json_token_false(struct json_tok_t *token_p);

/**
 * Initialize a JSON null token.
 *
 * @param[out] token_p Initialized token.
 */
void json_token_null(struct json_tok_t *token_p);

/**
 * Initialize a JSON number (integer/float) token.
 *
 * @param[out] token_p Initialized token.
 * @param[in] buf_p Number as a string.
 * @param[in] size String length.
 */
void json_token_number(struct json_tok_t *token_p,
                       const char *buf_p,
                       size_t size);

/**
 * Initialize a JSON string token.
 *
 * @param[out] token_p Initialized token.
 * @param[in] buf_p String.
 * @param[in] size String length.
 */
void json_token_string(struct json_tok_t *token_p,
                       const char *buf_p,
                       size_t size);

#endif
