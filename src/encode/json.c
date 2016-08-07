/**
 * @file json.c
 * @version 5.0.0
 *
 * @section License
 * Copyright (C) 2014-2016, Erik Moqvist
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

#include "simba.h"

struct encode_t {
    struct json_t *self_p;
    struct json_tok_t *tokens_p;
    int num_tokens;
    char *js_p;
};

static ssize_t encode(struct encode_t *state_p);

/**
 * Allocates a fresh unused token from the token pull.
 */
static struct json_tok_t *alloc_token(struct json_t *self_p,
                                      struct json_tok_t *tokens_p,
                                      size_t num_tokens)
{
    struct json_tok_t *tok_p;

    if (self_p->toknext >= num_tokens) {
        return NULL;
    }

    tok_p = &tokens_p[self_p->toknext++];
    tok_p->buf_p = NULL;
    tok_p->size = -1;
    tok_p->num_tokens = 0;
#ifdef JSON_PARENT_LINKS
    tok_p->parent = -1;
#endif

    return (tok_p);
}

/**
 * Fills token type and boundaries.
 */
static void fill_token(struct json_tok_t *token_p,
                       enum json_type_t type,
                       const void *buf_p,
                       int size)
{
    token_p->type = type;
    token_p->buf_p = buf_p;
    token_p->size = size;
    token_p->num_tokens = 0;
}

/**
 * Recursively encode given object token and its children.
 */
static ssize_t encode_object(struct encode_t *state_p,
                             struct json_tok_t *token_p)
{
    ssize_t size, res;
    const char *delim_p;
    int num_tokens_end;

    state_p->js_p += std_sprintf(state_p->js_p, FSTR("{"));
    size = 2;
    
    /* The first token not part of this object. */
    num_tokens_end = (state_p->num_tokens - token_p->num_tokens);

    for (delim_p = FSTR("");
         state_p->num_tokens > num_tokens_end;
         delim_p = FSTR(",")) {
        /* Delimiter. */
        res = std_sprintf(state_p->js_p, delim_p);
        size += res;
        state_p->js_p += res;

        /* Key. */

        /* The key must be a string or primitive type. */
        if (!((state_p->tokens_p->type == JSON_STRING)
              || (state_p->tokens_p->type == JSON_PRIMITIVE))) {
            return (-1);
        }

        res = encode(state_p);

        if (res < 0) {
            return (-1);
        }

        size += res;

        /* Key-value pair colon. */
        state_p->js_p += std_sprintf(state_p->js_p, FSTR(":"));
        size++;

        /* Value. */
        res = encode(state_p);

        if (res < 0) {
            return (-1);
        }

        size += res;
    }

    state_p->js_p += std_sprintf(state_p->js_p, FSTR("}"));

    return (size);
}

/**
 * Recursively encode given array token and its children.
 */
static ssize_t encode_array(struct encode_t *state_p,
                            struct json_tok_t *token_p)
{
    ssize_t size, res;
    const char *delim_p;
    int num_tokens_end;

    state_p->js_p += std_sprintf(state_p->js_p, FSTR("["));
    size = 2;

    /* The first token not part of this array. */
    num_tokens_end = (state_p->num_tokens - token_p->num_tokens);

    for (delim_p = FSTR("");
         state_p->num_tokens > num_tokens_end;
         delim_p = FSTR(",")) {
        /* Delimiter. */
        res = std_sprintf(state_p->js_p, delim_p);
        size += res;
        state_p->js_p += res;

        res = encode(state_p);

        if (res < 0) {
            return (-1);
        }

        size += res;
    }

    state_p->js_p += std_sprintf(state_p->js_p, FSTR("]"));

    return (size);
}

/**
 * Encode given string token.
 */
static ssize_t encode_string(struct encode_t *state_p,
                             struct json_tok_t *token_p)
{
    int i;

    state_p->js_p += std_sprintf(state_p->js_p, FSTR("\""));

    for (i = 0; i < token_p->size; i++) {
        state_p->js_p += std_sprintf(state_p->js_p,
                                     FSTR("%c"), token_p->buf_p[i]);
    }

    state_p->js_p += std_sprintf(state_p->js_p, FSTR("\""));

    return (token_p->size + 2);
}

/**
 * Encode given primitive token.
 */
static ssize_t encode_primitive(struct encode_t *state_p,
                                struct json_tok_t *token_p)
{
    int i;

    for (i = 0; i < token_p->size; i++) {
        state_p->js_p += std_sprintf(state_p->js_p,
                                     FSTR("%c"), token_p->buf_p[i]);
    }

    return (token_p->size);
}

/**
 * Recursively encodes one token and it's children.
 */
static ssize_t encode(struct encode_t *state_p)
{
    ssize_t size;
    struct json_tok_t *token_p;

    if (state_p->num_tokens == 0) {
        return (-1);
    }

    token_p = state_p->tokens_p;
    state_p->num_tokens--;
    state_p->tokens_p++;

    switch (token_p->type) {

    case JSON_OBJECT:
        size = encode_object(state_p, token_p);
        break;

    case JSON_ARRAY:
        size = encode_array(state_p, token_p);
        break;

    case JSON_STRING:
        size = encode_string(state_p, token_p);
        break;

    case JSON_PRIMITIVE:
        size = encode_primitive(state_p, token_p);
        break;

    default:
        size = -1;
        break;
    }

    return (size);
}

/**
 * Fills next available token with JSON primitive.
 */
static int decode_primitive(struct json_t *self_p,
                            const char *js_p,
                            size_t len,
                            struct json_tok_t *tokens_p,
                            size_t num_tokens)
{
    struct json_tok_t *token_p;
    int start;

    start = self_p->pos;

    for (; ((self_p->pos < len)
            && (js_p[self_p->pos] != '\0')); self_p->pos++) {
        switch (js_p[self_p->pos]) {

#ifndef JSON_STRICT
        case ':':
            /* In strict mode primitive must be followed by "," or "}"
               or "]" */
#endif

        case '\t':
        case '\r':
        case '\n':
        case ' ':
        case ',':
        case ']':
        case '}' :
            goto found;
        }

        if ((js_p[self_p->pos] < 32) || (js_p[self_p->pos] >= 127)) {
            self_p->pos = start;

            return (JSON_ERROR_INVAL);
        }
    }

#ifdef JSON_STRICT
    /* In strict mode primitive must be followed by a comma/object/array */
    self_p->pos = start;

    return (JSON_ERROR_PART);
#endif

 found:

    if (tokens_p == NULL) {
        self_p->pos--;
        return (0);
    }

    token_p = alloc_token(self_p, tokens_p, num_tokens);

    if (token_p == NULL) {
        self_p->pos = start;

        return (JSON_ERROR_NOMEM);
    }

    fill_token(token_p, JSON_PRIMITIVE, js_p + start, self_p->pos - start);

#ifdef JSON_PARENT_LINKS
    token_p->parent = self_p->toksuper;
#endif

    self_p->pos--;

    return (0);
}

/**
 * Fills next token with JSON string.
 */
static int decode_string(struct json_t *self_p,
                         const char *js_p,
                         size_t len,
                         struct json_tok_t *tokens_p,
                         size_t num_tokens)
{
    struct json_tok_t *token_p;

    int start = self_p->pos;

    self_p->pos++;

    /* Skip starting quote */
    for (; ((self_p->pos < len)
            && (js_p[self_p->pos] != '\0')); self_p->pos++) {
        char c = js_p[self_p->pos];

        /* Quote: end of string */
        if (c == '\"') {
            if (tokens_p == NULL) {
                return (0);
            }

            token_p = alloc_token(self_p, tokens_p, num_tokens);

            if (token_p == NULL) {
                self_p->pos = start;

                return (JSON_ERROR_NOMEM);
            }

            fill_token(token_p,
                       JSON_STRING,
                       js_p + start + 1,
                       self_p->pos - start - 1);
#ifdef JSON_PARENT_LINKS
            token_p->parent = self_p->toksuper;
#endif

            return (0);
        }

        /* Backslash: Quoted symbol expected */
        if ((c == '\\') && (self_p->pos + 1 < len)) {
            int i;

            self_p->pos++;

            switch (js_p[self_p->pos]) {

                /* Allowed escaped symbols */
            case '\"':
            case '/':
            case '\\':
            case 'b':
            case 'f':
            case 'r':
            case 'n':
            case 't':
                break;

                /* Allows escaped symbol \uXXXX */
            case 'u':
                self_p->pos++;

                for(i = 0; ((i < 4)
                            && (self_p->pos < len)
                            && (js_p[self_p->pos] != '\0')); i++) {
                    /* If it isn't a hex character we have an error */
                    if(!(((js_p[self_p->pos] >= 48)
                          && (js_p[self_p->pos] <= 57))
                         || /* 0-9 */
                         ((js_p[self_p->pos] >= 65)
                          && (js_p[self_p->pos] <= 70))
                         || /* A-F */
                         ((js_p[self_p->pos] >= 97)
                          && (js_p[self_p->pos] <= 102)))) { /* a-f */
                        self_p->pos = start;

                        return (JSON_ERROR_INVAL);
                    }

                    self_p->pos++;
                }

                self_p->pos--;
                break;

                /* Unexpected symbol */
            default:
                self_p->pos = start;

                return (JSON_ERROR_INVAL);
            }
        }
    }

    self_p->pos = start;

    return (JSON_ERROR_PART);
}

int json_init(struct json_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    self_p->pos = 0;
    self_p->toknext = 0;
    self_p->toksuper = -1;

    return (0);
}

ssize_t json_encode(struct json_t *self_p,
                    struct json_tok_t *tokens_p,
                    unsigned int num_tokens,
                    char *js_p)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(tokens_p != NULL, EINVAL);
    ASSERTN(js_p != NULL, EINVAL);

    struct encode_t state;

    /* The first token must be an object or an array. */
    if (!((tokens_p->type == JSON_OBJECT)
          || (tokens_p->type == JSON_ARRAY))) {
        return (-1);
    }

    state.self_p = self_p;
    state.tokens_p = tokens_p;
    state.num_tokens = num_tokens;
    state.js_p = js_p;

    return (encode(&state));
}

int json_decode(struct json_t *self_p,
                const char *js_p,
                size_t len,
                struct json_tok_t *tokens_p,
                unsigned int num_tokens)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(js_p != NULL, EINVAL);
    ASSERTN(len > 0, EINVAL);
    ASSERTN(((tokens_p != NULL) && (num_tokens > 0))
            || (num_tokens == 0), EINVAL);

    int r;
    int i;
    struct json_tok_t *token_p;
    int count = self_p->toknext;

    for (; ((self_p->pos < len)
            && (js_p[self_p->pos] != '\0')); self_p->pos++) {
        char c;
        enum json_type_t type;

        c = js_p[self_p->pos];

        switch (c) {

        case '{':
        case '[':
            count++;

            if (tokens_p == NULL) {
                break;
            }

            token_p = alloc_token(self_p, tokens_p, num_tokens);

            if (token_p == NULL) {
                return (JSON_ERROR_NOMEM);
            }

            if (self_p->toksuper != -1) {
                tokens_p[self_p->toksuper].num_tokens++;
#ifdef JSON_PARENT_LINKS
                token_p->parent = self_p->toksuper;
#endif
            }

            token_p->type = (c == '{' ? JSON_OBJECT : JSON_ARRAY);
            token_p->buf_p = js_p + self_p->pos;
            self_p->toksuper = self_p->toknext - 1;
            break;

        case '}':
        case ']':
            if (tokens_p == NULL) {
                break;
            }

            type = (c == '}' ? JSON_OBJECT : JSON_ARRAY);

#ifdef JSON_PARENT_LINKS
            if (self_p->toknext < 1) {
                return (JSON_ERROR_INVAL);
            }

            token = &tokens_p[self_p->toknext - 1];

            for (;;) {
                if ((token_p->start != -1) && (token_p->end == -1)) {
                    if (token_p->type != type) {
                        return (JSON_ERROR_INVAL);
                    }

                    token_p->size = (&js_p[self_p->pos]
                                     - self_p->buf_p + 1);
                    self_p->toksuper = token_p->parent;
                    break;
                }

                if (token_p->parent == -1) {
                    break;
                }

                token = &tokens_p[token_p->parent];
            }
#else
            for (i = self_p->toknext - 1; i >= 0; i--) {
                token_p = &tokens_p[i];

                if ((token_p->buf_p != NULL) && (token_p->size == -1)) {
                    if (token_p->type != type) {
                        return (JSON_ERROR_INVAL);
                    }

                    self_p->toksuper = -1;
                    token_p->size = (&js_p[self_p->pos]
                                     - token_p->buf_p + 1);
                    break;
                }
            }

            /* Error if unmatched closing bracket */
            if (i == -1) {
                return (JSON_ERROR_INVAL);
            }

            for (; i >= 0; i--) {
                token_p = &tokens_p[i];

                if ((token_p->buf_p != NULL) && (token_p->size == -1)) {
                    self_p->toksuper = i;
                    break;
                }
            }
#endif
            break;
        case '\"':
            r = decode_string(self_p, js_p, len, tokens_p, num_tokens);

            if (r < 0) {
                return (r);
            }

            count++;

            if ((self_p->toksuper != -1) && (tokens_p != NULL)) {
                tokens_p[self_p->toksuper].num_tokens++;
            }

            break;

        case '\t':
        case '\r':
        case '\n':
        case ' ':
            break;

        case ':':
            self_p->toksuper = self_p->toknext - 1;
            break;

        case ',':
            if ((tokens_p != NULL)
                && (self_p->toksuper != -1)
                && (tokens_p[self_p->toksuper].type != JSON_ARRAY)
                && (tokens_p[self_p->toksuper].type != JSON_OBJECT)) {
#ifdef JSON_PARENT_LINKS
                self_p->toksuper = tokens_p[self_p->toksuper].parent;
#else
                for (i = self_p->toknext - 1; i >= 0; i--) {
                    if ((tokens_p[i].type == JSON_ARRAY)
                        || (tokens_p[i].type == JSON_OBJECT)) {
                        if ((tokens_p[i].buf_p != NULL)
                            && (tokens_p[i].size == -1)) {
                            self_p->toksuper = i;
                            break;
                        }
                    }
                }
#endif
            }
            break;

#ifdef JSON_STRICT
            /* In strict mode primitives are: numbers and booleans */
        case '-':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case 't':
        case 'f':
        case 'n':
            /* And they must not be keys of the object */
            if ((tokens != NULL) && (self_p->toksuper != -1)) {
                struct json_tok_t *t = &tokens_p[self_p->toksuper];

                if ((t_p->type == JSON_OBJECT)
                    || ((t_p->type == JSON_STRING)
                        && (t_p->size != 0))) {
                    return (JSON_ERROR_INVAL);
                }
            }
#else

            /* In non-strict mode every unquoted value is a primitive */
        default:
#endif
            r = decode_primitive(self_p, js_p, len, tokens_p, num_tokens);

            if (r < 0) {
                return (r);
            }

            count++;

            if ((self_p->toksuper != -1) && (tokens_p != NULL)) {
                tokens_p[self_p->toksuper].num_tokens++;
            }

            break;

#ifdef JSON_STRICT

            /* Unexpected char in strict mode */
        default:
            return (JSON_ERROR_INVAL);
#endif
        }
    }

    if (tokens_p != NULL) {
        for (i = self_p->toknext - 1; i >= 0; i--) {
            /* Unmatched opened object or array */
            if ((tokens_p[i].buf_p != NULL) && (tokens_p[i].size == -1)) {
                return (JSON_ERROR_PART);
            }
        }
    }

    return (count);
}

struct json_tok_t json_token_object(int num_tokens)
{
    struct json_tok_t token;

    token.type = JSON_OBJECT;
    token.buf_p = NULL;
    token.size = -1;
    token.num_tokens = num_tokens;

    return (token);
}

struct json_tok_t json_token_array(int num_tokens)
{
    struct json_tok_t token;

    token.type = JSON_ARRAY;
    token.buf_p = NULL;
    token.size = -1;
    token.num_tokens = num_tokens;

    return (token);
}

struct json_tok_t json_token_true()
{
    struct json_tok_t token;

    token.type = JSON_PRIMITIVE;
    token.buf_p = "true";
    token.size = 4;
    token.num_tokens = -1;

    return (token);
}

struct json_tok_t json_token_false()
{
    struct json_tok_t token;

    token.type = JSON_PRIMITIVE;
    token.buf_p = "false";
    token.size = 5;
    token.num_tokens = -1;

    return (token);
}

struct json_tok_t json_token_null()
{
    struct json_tok_t token;

    token.type = JSON_PRIMITIVE;
    token.buf_p = "null";
    token.size = 4;
    token.num_tokens = -1;

    return (token);
}

struct json_tok_t json_token_number(const char *buf_p,
                                    size_t size)
{
    struct json_tok_t token;

    token.type = JSON_PRIMITIVE;
    token.buf_p = buf_p;
    token.size = size;
    token.num_tokens = -1;

    return (token);
}

struct json_tok_t json_token_string(const char *buf_p,
                                    size_t size)
{
    struct json_tok_t token;

    token.type = JSON_STRING;
    token.buf_p = buf_p;
    token.size = size;
    token.num_tokens = -1;

    return (token);
}
