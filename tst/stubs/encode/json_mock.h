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

#ifndef __JSON_MOCK_H__
#define __JSON_MOCK_H__

#include "simba.h"

int mock_write_json_init(struct json_tok_t *tokens_p,
                         int num_tokens,
                         int res);

int mock_write_json_parse(const char *js_p,
                          size_t len,
                          int res);

int mock_write_json_dumps(struct json_tok_t *tokens_p,
                          char *js_p,
                          ssize_t res);

int mock_write_json_dump(struct json_tok_t *tokens_p,
                         void *out_p,
                         ssize_t res);

int mock_write_json_root(struct json_tok_t *res);

int mock_write_json_object_get(const char *key_p,
                               struct json_tok_t *object_p,
                               struct json_tok_t *res);

int mock_write_json_object_get_primitive(const char *key_p,
                                         struct json_tok_t *object_p,
                                         struct json_tok_t *res);

int mock_write_json_array_get(int index,
                              struct json_tok_t *array_p,
                              struct json_tok_t *res);

int mock_write_json_token_object(struct json_tok_t *token_p,
                                 int num_keys);

int mock_write_json_token_array(struct json_tok_t *token_p,
                                int num_elements);

int mock_write_json_token_true(struct json_tok_t *token_p);

int mock_write_json_token_false(struct json_tok_t *token_p);

int mock_write_json_token_null(struct json_tok_t *token_p);

int mock_write_json_token_number(struct json_tok_t *token_p,
                                 const char *buf_p,
                                 size_t size);

int mock_write_json_token_string(struct json_tok_t *token_p,
                                 const char *buf_p,
                                 size_t size);

#endif
