/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2018, Erik Moqvist
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
#include "json_mock.h"

int mock_write_json_init(struct json_tok_t *tokens_p,
                         int num_tokens,
                         int res)
{
    harness_mock_write("json_init(tokens_p)",
                       tokens_p,
                       sizeof(*tokens_p));

    harness_mock_write("json_init(num_tokens)",
                       &num_tokens,
                       sizeof(num_tokens));

    harness_mock_write("json_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(json_init)(struct json_t *self_p,
                                           struct json_tok_t *tokens_p,
                                           int num_tokens)
{
    int res;

    harness_mock_assert("json_init(tokens_p)",
                        tokens_p,
                        sizeof(*tokens_p));

    harness_mock_assert("json_init(num_tokens)",
                        &num_tokens,
                        sizeof(num_tokens));

    harness_mock_read("json_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_json_parse(const char *js_p,
                          size_t len,
                          int res)
{
    harness_mock_write("json_parse(js_p)",
                       js_p,
                       strlen(js_p) + 1);

    harness_mock_write("json_parse(len)",
                       &len,
                       sizeof(len));

    harness_mock_write("json_parse(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(json_parse)(struct json_t *self_p,
                                            const char *js_p,
                                            size_t len)
{
    int res;

    harness_mock_assert("json_parse(js_p)",
                        js_p,
                        sizeof(*js_p));

    harness_mock_assert("json_parse(len)",
                        &len,
                        sizeof(len));

    harness_mock_read("json_parse(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_json_dumps(struct json_tok_t *tokens_p,
                          char *js_p,
                          ssize_t res)
{
    harness_mock_write("json_dumps(tokens_p)",
                       tokens_p,
                       sizeof(*tokens_p));

    harness_mock_write("json_dumps(): return (js_p)",
                       js_p,
                       strlen(js_p) + 1);

    harness_mock_write("json_dumps(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(json_dumps)(struct json_t *self_p,
                                                struct json_tok_t *tokens_p,
                                                char *js_p)
{
    ssize_t res;

    harness_mock_assert("json_dumps(tokens_p)",
                        tokens_p,
                        sizeof(*tokens_p));

    harness_mock_read("json_dumps(): return (js_p)",
                      js_p,
                      sizeof(*js_p));

    harness_mock_read("json_dumps(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_json_dump(struct json_tok_t *tokens_p,
                         void *out_p,
                         ssize_t res)
{
    harness_mock_write("json_dump(tokens_p)",
                       tokens_p,
                       sizeof(*tokens_p));

    harness_mock_write("json_dump(out_p)",
                       out_p,
                       sizeof(out_p));

    harness_mock_write("json_dump(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(json_dump)(struct json_t *self_p,
                                               struct json_tok_t *tokens_p,
                                               void *out_p)
{
    ssize_t res;

    harness_mock_assert("json_dump(tokens_p)",
                        tokens_p,
                        sizeof(*tokens_p));

    harness_mock_assert("json_dump(out_p)",
                        out_p,
                        sizeof(*out_p));

    harness_mock_read("json_dump(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_json_root(struct json_tok_t *res)
{
    harness_mock_write("json_root(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

struct json_tok_t *__attribute__ ((weak)) STUB(json_root)(struct json_t *self_p)
{
    struct json_tok_t *res;

    harness_mock_read("json_root(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_json_object_get(const char *key_p,
                               struct json_tok_t *object_p,
                               struct json_tok_t *res)
{
    harness_mock_write("json_object_get(key_p)",
                       key_p,
                       strlen(key_p) + 1);

    harness_mock_write("json_object_get(object_p)",
                       object_p,
                       sizeof(*object_p));

    harness_mock_write("json_object_get(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

struct json_tok_t *__attribute__ ((weak)) STUB(json_object_get)(struct json_t *self_p,
                                                                const char *key_p,
                                                                struct json_tok_t *object_p)
{
    struct json_tok_t *res;

    harness_mock_assert("json_object_get(key_p)",
                        key_p,
                        sizeof(*key_p));

    harness_mock_assert("json_object_get(object_p)",
                        object_p,
                        sizeof(*object_p));

    harness_mock_read("json_object_get(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_json_object_get_primitive(const char *key_p,
                                         struct json_tok_t *object_p,
                                         struct json_tok_t *res)
{
    harness_mock_write("json_object_get_primitive(key_p)",
                       key_p,
                       strlen(key_p) + 1);

    harness_mock_write("json_object_get_primitive(object_p)",
                       object_p,
                       sizeof(*object_p));

    harness_mock_write("json_object_get_primitive(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

struct json_tok_t *__attribute__ ((weak)) STUB(json_object_get_primitive)(struct json_t *self_p,
                                                                          const char *key_p,
                                                                          struct json_tok_t *object_p)
{
    struct json_tok_t *res;

    harness_mock_assert("json_object_get_primitive(key_p)",
                        key_p,
                        sizeof(*key_p));

    harness_mock_assert("json_object_get_primitive(object_p)",
                        object_p,
                        sizeof(*object_p));

    harness_mock_read("json_object_get_primitive(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_json_array_get(int index,
                              struct json_tok_t *array_p,
                              struct json_tok_t *res)
{
    harness_mock_write("json_array_get(index)",
                       &index,
                       sizeof(index));

    harness_mock_write("json_array_get(array_p)",
                       array_p,
                       sizeof(*array_p));

    harness_mock_write("json_array_get(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

struct json_tok_t *__attribute__ ((weak)) STUB(json_array_get)(struct json_t *self_p,
                                                               int index,
                                                               struct json_tok_t *array_p)
{
    struct json_tok_t *res;

    harness_mock_assert("json_array_get(index)",
                        &index,
                        sizeof(index));

    harness_mock_assert("json_array_get(array_p)",
                        array_p,
                        sizeof(*array_p));

    harness_mock_read("json_array_get(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_json_token_object(struct json_tok_t *token_p,
                                 int num_keys)
{
    harness_mock_write("json_token_object(): return (token_p)",
                       token_p,
                       sizeof(*token_p));

    harness_mock_write("json_token_object(num_keys)",
                       &num_keys,
                       sizeof(num_keys));

    return (0);
}

void __attribute__ ((weak)) STUB(json_token_object)(struct json_tok_t *token_p,
                                                    int num_keys)
{
    harness_mock_read("json_token_object(): return (token_p)",
                      token_p,
                      sizeof(*token_p));

    harness_mock_assert("json_token_object(num_keys)",
                        &num_keys,
                        sizeof(num_keys));
}

int mock_write_json_token_array(struct json_tok_t *token_p,
                                int num_elements)
{
    harness_mock_write("json_token_array(): return (token_p)",
                       token_p,
                       sizeof(*token_p));

    harness_mock_write("json_token_array(num_elements)",
                       &num_elements,
                       sizeof(num_elements));

    return (0);
}

void __attribute__ ((weak)) STUB(json_token_array)(struct json_tok_t *token_p,
                                                   int num_elements)
{
    harness_mock_read("json_token_array(): return (token_p)",
                      token_p,
                      sizeof(*token_p));

    harness_mock_assert("json_token_array(num_elements)",
                        &num_elements,
                        sizeof(num_elements));
}

int mock_write_json_token_true(struct json_tok_t *token_p)
{
    harness_mock_write("json_token_true(): return (token_p)",
                       token_p,
                       sizeof(*token_p));

    return (0);
}

void __attribute__ ((weak)) STUB(json_token_true)(struct json_tok_t *token_p)
{
    harness_mock_read("json_token_true(): return (token_p)",
                      token_p,
                      sizeof(*token_p));
}

int mock_write_json_token_false(struct json_tok_t *token_p)
{
    harness_mock_write("json_token_false(): return (token_p)",
                       token_p,
                       sizeof(*token_p));

    return (0);
}

void __attribute__ ((weak)) STUB(json_token_false)(struct json_tok_t *token_p)
{
    harness_mock_read("json_token_false(): return (token_p)",
                      token_p,
                      sizeof(*token_p));
}

int mock_write_json_token_null(struct json_tok_t *token_p)
{
    harness_mock_write("json_token_null(): return (token_p)",
                       token_p,
                       sizeof(*token_p));

    return (0);
}

void __attribute__ ((weak)) STUB(json_token_null)(struct json_tok_t *token_p)
{
    harness_mock_read("json_token_null(): return (token_p)",
                      token_p,
                      sizeof(*token_p));
}

int mock_write_json_token_number(struct json_tok_t *token_p,
                                 const char *buf_p,
                                 size_t size)
{
    harness_mock_write("json_token_number(): return (token_p)",
                       token_p,
                       sizeof(*token_p));

    harness_mock_write("json_token_number(buf_p)",
                       buf_p,
                       strlen(buf_p) + 1);

    harness_mock_write("json_token_number(size)",
                       &size,
                       sizeof(size));

    return (0);
}

void __attribute__ ((weak)) STUB(json_token_number)(struct json_tok_t *token_p,
                                                    const char *buf_p,
                                                    size_t size)
{
    harness_mock_read("json_token_number(): return (token_p)",
                      token_p,
                      sizeof(*token_p));

    harness_mock_assert("json_token_number(buf_p)",
                        buf_p,
                        sizeof(*buf_p));

    harness_mock_assert("json_token_number(size)",
                        &size,
                        sizeof(size));
}

int mock_write_json_token_string(struct json_tok_t *token_p,
                                 const char *buf_p,
                                 size_t size)
{
    harness_mock_write("json_token_string(): return (token_p)",
                       token_p,
                       sizeof(*token_p));

    harness_mock_write("json_token_string(buf_p)",
                       buf_p,
                       strlen(buf_p) + 1);

    harness_mock_write("json_token_string(size)",
                       &size,
                       sizeof(size));

    return (0);
}

void __attribute__ ((weak)) STUB(json_token_string)(struct json_tok_t *token_p,
                                                    const char *buf_p,
                                                    size_t size)
{
    harness_mock_read("json_token_string(): return (token_p)",
                      token_p,
                      sizeof(*token_p));

    harness_mock_assert("json_token_string(buf_p)",
                        buf_p,
                        sizeof(*buf_p));

    harness_mock_assert("json_token_string(size)",
                        &size,
                        sizeof(size));
}
