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

#include "simba.h"

struct dump_t {
    struct json_t *self_p;
    struct json_tok_t *tokens_p;
    int num_tokens;
    void *out_p;
};

struct string_t {
    struct chan_t base;
    ssize_t pos;
    char *buf_p;
};

static ssize_t dump(struct dump_t *state_p);

/**
 * Copy given buffer to the output buffer. It is not null terminated.
 */
static ssize_t string_write(void *chan_p, const void *buf_p, size_t size)
{
    struct string_t *string_p;

    string_p = chan_p;
    memcpy(&string_p->buf_p[string_p->pos], buf_p, size);
    string_p->pos += size;

    return (size);
}

/**
 * Allocates a fresh unused token from the token pull.
 */
static struct json_tok_t *alloc_token(struct json_t *self_p)
{
    struct json_tok_t *tok_p;

    if (self_p->toknext >= self_p->num_tokens) {
        return NULL;
    }

    tok_p = &self_p->tokens_p[self_p->toknext++];
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
 * Recursively dump given object token and its children.
 */
static ssize_t dump_object(struct dump_t *state_p,
                           struct json_tok_t *token_p)
{
    ssize_t size, res, i;
    const char *delim_p;

    std_fprintf(state_p->out_p, FSTR("{"));
    size = 2;
    delim_p = FSTR("");

    for (i = 0; i < token_p->num_tokens; i++) {
        /* Delimiter. */
        res = std_fprintf(state_p->out_p, delim_p);
        size += res;

        /* Key. */

        /* The key must be a string or primitive type. */
        if (!((state_p->tokens_p->type == JSON_STRING)
              || (state_p->tokens_p->type == JSON_PRIMITIVE))) {
            return (-1);
        }

        res = dump(state_p);

        if (res < 0) {
            return (res);
        }

        size += res;

        /* Key-value pair colon. */
        std_fprintf(state_p->out_p, FSTR(":"));
        size++;

        /* Value. */
        res = dump(state_p);

        if (res < 0) {
            return (-1);
        }

        size += res;
        delim_p = FSTR(",");
    }

    std_fprintf(state_p->out_p, FSTR("}"));

    return (size);
}

/**
 * Recursively dump given array token and its children.
 */
static ssize_t dump_array(struct dump_t *state_p,
                          struct json_tok_t *token_p)
{
    ssize_t size, res, i;
    const char *delim_p;

    std_fprintf(state_p->out_p, FSTR("["));
    size = 2;
    delim_p = FSTR("");

    for (i = 0; i < token_p->num_tokens; i++) {
        /* Delimiter. */
        res = std_fprintf(state_p->out_p, delim_p);
        size += res;

        res = dump(state_p);

        if (res < 0) {
            return (-1);
        }

        size += res;
        delim_p = FSTR(",");
    }

    std_fprintf(state_p->out_p, FSTR("]"));

    return (size);
}

/**
 * Dump given string token.
 */
static ssize_t dump_string(struct dump_t *state_p,
                           struct json_tok_t *token_p)
{
    int i;

    std_fprintf(state_p->out_p, FSTR("\""));

    for (i = 0; i < token_p->size; i++) {
        if (!isprint((int)token_p->buf_p[i])) {
            return (-1);
        }

        std_fprintf(state_p->out_p, FSTR("%c"), token_p->buf_p[i]);
    }

    std_fprintf(state_p->out_p, FSTR("\""));

    return (token_p->size + 2);
}

/**
 * Dump given primitive token.
 */
static ssize_t dump_primitive(struct dump_t *state_p,
                              struct json_tok_t *token_p)
{
    int i;

    for (i = 0; i < token_p->size; i++) {
        std_fprintf(state_p->out_p, FSTR("%c"), token_p->buf_p[i]);
    }

    return (token_p->size);
}

/**
 * Recursively dumps one token and it's children.
 */
static ssize_t dump(struct dump_t *state_p)
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
        size = dump_object(state_p, token_p);
        break;

    case JSON_ARRAY:
        size = dump_array(state_p, token_p);
        break;

    case JSON_STRING:
        size = dump_string(state_p, token_p);
        break;

    case JSON_PRIMITIVE:
        size = dump_primitive(state_p, token_p);
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
static int parse_primitive(struct json_t *self_p,
                           const char *js_p,
                           size_t len)
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

    if (self_p->tokens_p == NULL) {
        self_p->pos--;
        return (0);
    }

    token_p = alloc_token(self_p);

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
static int parse_string(struct json_t *self_p,
                        const char *js_p,
                        size_t len)
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
            if (self_p->tokens_p == NULL) {
                return (0);
            }

            token_p = alloc_token(self_p);

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

/**
 * Recursively get the number of child tokens of given token.
 */
static int get_number_of_children(struct json_tok_t *token_p)
{
    int i;
    int number_of_children;
    struct json_tok_t *child_p;

    number_of_children = 0;

    for (i = 0; i < token_p->num_tokens; i++) {
        child_p = &token_p[number_of_children + 1 + i];

        if (child_p->num_tokens > 0) {
            number_of_children += get_number_of_children(child_p);
        }
    }

    number_of_children += token_p->num_tokens;

    return (number_of_children);
}

static struct json_tok_t *object_get(struct json_t *self_p,
                                     const char *key_p,
                                     struct json_tok_t *object_p,
                                     int type)
{
    ASSERTNRN(self_p != NULL, EINVAL);
    ASSERTNRN(key_p != NULL, EINVAL);

    int i;
    int key_length;
    int number_of_children;
    struct json_tok_t *token_p;

    /* Return immediatly if no object is found. */
    if (object_p == NULL) {
        return (NULL);
    }

    /* Check that the token is an object. */
    if (object_p->type != JSON_OBJECT) {
        return (NULL);
    }

    key_length = strlen(key_p);

    /* The first child token. */
    token_p = (object_p + 1);

    /* Find given key in the object. */
    for (i = 0; i < object_p->num_tokens; i++) {
        if (token_p->type == type) {
            if (strncmp(key_p, token_p->buf_p, key_length) == 0) {
                return (token_p + 1);
            }
        }

        /* Get the next child token. */
        number_of_children = get_number_of_children(token_p);
        token_p += (number_of_children + 1);
    }

    return (NULL);
}

int json_init(struct json_t *self_p,
              struct json_tok_t *tokens_p,
              int num_tokens)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(((tokens_p != NULL) && (num_tokens > 0))
            || (num_tokens == 0), EINVAL);

    self_p->pos = 0;
    self_p->toknext = 0;
    self_p->toksuper = -1;
    self_p->tokens_p = tokens_p;
    self_p->num_tokens = num_tokens;

    return (0);
}

int json_parse(struct json_t *self_p,
               const char *js_p,
               size_t len)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(js_p != NULL, EINVAL);
    ASSERTN(len > 0, EINVAL);

    int r;
    int i;
    struct json_tok_t *token_p;
    int count;

    count = self_p->toknext;

    for (; ((self_p->pos < len)
            && (js_p[self_p->pos] != '\0')); self_p->pos++) {
        char c;
        enum json_type_t type;

        c = js_p[self_p->pos];

        switch (c) {

        case '{':
        case '[':
            count++;

            if (self_p->tokens_p == NULL) {
                break;
            }

            token_p = alloc_token(self_p);

            if (token_p == NULL) {
                return (JSON_ERROR_NOMEM);
            }

            if (self_p->toksuper != -1) {
                self_p->tokens_p[self_p->toksuper].num_tokens++;
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
            if (self_p->tokens_p == NULL) {
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
                token_p = &self_p->tokens_p[i];

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
                token_p = &self_p->tokens_p[i];

                if ((token_p->buf_p != NULL) && (token_p->size == -1)) {
                    self_p->toksuper = i;
                    break;
                }
            }
#endif
            break;
        case '\"':
            r = parse_string(self_p, js_p, len);

            if (r < 0) {
                return (r);
            }

            count++;

            if ((self_p->toksuper != -1) && (self_p->tokens_p != NULL)) {
                self_p->tokens_p[self_p->toksuper].num_tokens++;
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
            if ((self_p->tokens_p != NULL)
                && (self_p->toksuper != -1)
                && (self_p->tokens_p[self_p->toksuper].type != JSON_ARRAY)
                && (self_p->tokens_p[self_p->toksuper].type != JSON_OBJECT)) {
#ifdef JSON_PARENT_LINKS
                self_p->toksuper = tokens_p[self_p->toksuper].parent;
#else
                for (i = self_p->toknext - 1; i >= 0; i--) {
                    if ((self_p->tokens_p[i].type == JSON_ARRAY)
                        || (self_p->tokens_p[i].type == JSON_OBJECT)) {
                        if ((self_p->tokens_p[i].buf_p != NULL)
                            && (self_p->tokens_p[i].size == -1)) {
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
            r = parse_primitive(self_p, js_p, len);

            if (r < 0) {
                return (r);
            }

            count++;

            if ((self_p->toksuper != -1) && (self_p->tokens_p != NULL)) {
                self_p->tokens_p[self_p->toksuper].num_tokens++;
            }

            break;

#ifdef JSON_STRICT

            /* Unexpected char in strict mode */
        default:
            return (JSON_ERROR_INVAL);
#endif
        }
    }

    if (self_p->tokens_p != NULL) {
        for (i = self_p->toknext - 1; i >= 0; i--) {
            /* Unmatched opened object or array */
            if ((self_p->tokens_p[i].buf_p != NULL) && (self_p->tokens_p[i].size == -1)) {
                return (JSON_ERROR_PART);
            }
        }
    }

    return (count);
}

ssize_t json_dumps(struct json_t *self_p,
                   struct json_tok_t *tokens_p,
                   char *js_p)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(js_p != NULL, EINVAL);

    ssize_t res;
    struct string_t string;

    chan_init(&string.base, chan_read_null, string_write, chan_size_null);
    string.buf_p = js_p;
    string.pos = 0;

    res = json_dump(self_p, tokens_p, &string);

    if (res < 0) {
        return (res);
    }

    return (chan_write(&string, "\0", 1) == 1 ? res : -1);
}

ssize_t json_dump(struct json_t *self_p,
                  struct json_tok_t *tokens_p,
                  void *out_p)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(out_p != NULL, EINVAL);

    struct dump_t state;

    if (tokens_p == NULL) {
        tokens_p = self_p->tokens_p;
    }

    /* The first token must be an object or an array. */
    if (!((tokens_p->type == JSON_OBJECT)
          || (tokens_p->type == JSON_ARRAY))) {
        return (-1);
    }

    state.tokens_p = tokens_p;
    state.num_tokens = self_p->num_tokens;
    state.out_p = out_p;

    return (dump(&state));
}

struct json_tok_t *json_root(struct json_t *self_p)
{
    return (self_p->tokens_p);
}

struct json_tok_t *json_object_get(struct json_t *self_p,
                                   const char *key_p,
                                   struct json_tok_t *object_p)
{
    return (object_get(self_p, key_p, object_p, JSON_STRING));
}

struct json_tok_t *json_object_get_primitive(struct json_t *self_p,
                                             const char *key_p,
                                             struct json_tok_t *object_p)
{
    return (object_get(self_p, key_p, object_p, JSON_PRIMITIVE));
}

struct json_tok_t *json_array_get(struct json_t *self_p,
                                  int index,
                                  struct json_tok_t *array_p)
{
    ASSERTNRN(self_p != NULL, EINVAL);
    ASSERTNRN(index >= 0, EINVAL);

    int i;
    int number_of_children;
    struct json_tok_t *token_p;

    /* Return immediatly if no array is found. */
    if (array_p == NULL) {
        return (NULL);
    }

    /* Check that the token is an array. */
    if (array_p->type != JSON_ARRAY) {
        return (NULL);
    }

    /* The first child token. */
    token_p = (array_p + 1);

    /* Find given key in the object. */
    for (i = 0; i < array_p->num_tokens; i++) {
        if (i == index) {
            return (token_p);
        }

        /* Get the next child token. */
        number_of_children = get_number_of_children(token_p);
        token_p += (number_of_children + 1);
    }

    return (NULL);
}

void json_token_object(struct json_tok_t *token_p,
                       int num_keys)
{
    token_p->type = JSON_OBJECT;
    token_p->buf_p = NULL;
    token_p->size = -1;
    token_p->num_tokens = num_keys;
}

void json_token_array(struct json_tok_t *token_p,
                      int num_elements)
{
    token_p->type = JSON_ARRAY;
    token_p->buf_p = NULL;
    token_p->size = -1;
    token_p->num_tokens = num_elements;
}

void json_token_true(struct json_tok_t *token_p)
{
    token_p->type = JSON_PRIMITIVE;
    token_p->buf_p = "true";
    token_p->size = 4;
    token_p->num_tokens = -1;
}

void json_token_false(struct json_tok_t *token_p)
{
    token_p->type = JSON_PRIMITIVE;
    token_p->buf_p = "false";
    token_p->size = 5;
    token_p->num_tokens = -1;
}

void json_token_null(struct json_tok_t *token_p)
{
    token_p->type = JSON_PRIMITIVE;
    token_p->buf_p = "null";
    token_p->size = 4;
    token_p->num_tokens = -1;
}

void json_token_number(struct json_tok_t *token_p,
                       const char *buf_p,
                       size_t size)
{
    token_p->type = JSON_PRIMITIVE;
    token_p->buf_p = buf_p;
    token_p->size = size;
    token_p->num_tokens = -1;
}

void json_token_string(struct json_tok_t *token_p,
                       const char *buf_p,
                       size_t size)
{
    token_p->type = JSON_STRING;
    token_p->buf_p = buf_p;
    token_p->size = size;
    token_p->num_tokens = -1;
}
