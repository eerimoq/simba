/**
 * @file json.c
 * @version 0.5.0
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

/**
 * Allocates a fresh unused token from the token pull.
 */
static struct json_tok_t *alloc_token(struct json_parser_t *parser_p,
                                      struct json_tok_t *tokens_p,
                                      size_t num_tokens)
{
    struct json_tok_t *tok_p;

    if (parser_p->toknext >= num_tokens) {
        return NULL;
    }

    tok_p = &tokens_p[parser_p->toknext++];
    tok_p->start = tok_p->end = -1;
    tok_p->size = 0;
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
                       int start,
                       int end)
{
    token_p->type = type;
    token_p->start = start;
    token_p->end = end;
    token_p->size = 0;
}

/**
 * Fills next available token with JSON primitive.
 */
static int parse_primitive(struct json_parser_t *parser_p,
                           const char *js_p,
                           size_t len,
                           struct json_tok_t *tokens_p,
                           size_t num_tokens)
{
    struct json_tok_t *token_p;
    int start;

    start = parser_p->pos;

    for (; ((parser_p->pos < len)
            && (js_p[parser_p->pos] != '\0')); parser_p->pos++) {
        switch (js_p[parser_p->pos]) {

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

        if ((js_p[parser_p->pos] < 32) || (js_p[parser_p->pos] >= 127)) {
            parser_p->pos = start;

            return (JSON_ERROR_INVAL);
        }
    }

#ifdef JSON_STRICT
    /* In strict mode primitive must be followed by a comma/object/array */
    parser_p->pos = start;

    return (JSON_ERROR_PART);
#endif

 found:

    if (tokens_p == NULL) {
        parser_p->pos--;
        return (0);
    }
    
    token_p = alloc_token(parser_p, tokens_p, num_tokens);

    if (token_p == NULL) {
        parser_p->pos = start;

        return (JSON_ERROR_NOMEM);
    }

    fill_token(token_p, JSON_PRIMITIVE, start, parser_p->pos);

#ifdef JSON_PARENT_LINKS
    token_p->parent = parser_p->toksuper;
#endif

    parser_p->pos--;

    return (0);
}

/**
 * Fills next token with JSON string.
 */
static int parse_string(struct json_parser_t *parser_p,
                        const char *js_p,
                        size_t len,
                        struct json_tok_t *tokens_p,
                        size_t num_tokens)
{
    struct json_tok_t *token_p;

    int start = parser_p->pos;

    parser_p->pos++;

    /* Skip starting quote */
    for (; ((parser_p->pos < len)
            && (js_p[parser_p->pos] != '\0')); parser_p->pos++) {
        char c = js_p[parser_p->pos];

        /* Quote: end of string */
        if (c == '\"') {
            if (tokens_p == NULL) {
                return (0);
            }

            token_p = alloc_token(parser_p, tokens_p, num_tokens);

            if (token_p == NULL) {
                parser_p->pos = start;

                return (JSON_ERROR_NOMEM);
            }

            fill_token(token_p, JSON_STRING, start+1, parser_p->pos);
#ifdef JSON_PARENT_LINKS
            token_p->parent = parser_p->toksuper;
#endif

            return (0);
        }

        /* Backslash: Quoted symbol expected */
        if ((c == '\\') && (parser_p->pos + 1 < len)) {
            int i;

            parser_p->pos++;

            switch (js_p[parser_p->pos]) {

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
                parser_p->pos++;
                
                for(i = 0; ((i < 4)
                            && (parser_p->pos < len)
                            && (js_p[parser_p->pos] != '\0')); i++) {
                    /* If it isn't a hex character we have an error */
                    if(!(((js_p[parser_p->pos] >= 48)
                          && (js_p[parser_p->pos] <= 57))
                         || /* 0-9 */
                         ((js_p[parser_p->pos] >= 65)
                          && (js_p[parser_p->pos] <= 70))
                         || /* A-F */
                         ((js_p[parser_p->pos] >= 97)
                          && (js_p[parser_p->pos] <= 102)))) { /* a-f */
                        parser_p->pos = start;
                        
                        return (JSON_ERROR_INVAL);
                    }

                    parser_p->pos++;
                }

                parser_p->pos--;
                break;

                /* Unexpected symbol */
            default:
                parser_p->pos = start;

                return (JSON_ERROR_INVAL);
            }
        }
    }

    parser_p->pos = start;

    return (JSON_ERROR_PART);
}

int json_init(struct json_parser_t *parser_p)
{
    ASSERTN(parser_p != NULL, EINVAL);

    parser_p->pos = 0;
    parser_p->toknext = 0;
    parser_p->toksuper = -1;

    return (0);
}

/**
 * Parse JSON string and fill tokens.
 */
int json_parse(struct json_parser_t *parser_p,
               const char *js_p,
               size_t len,
               struct json_tok_t *tokens_p, 
               unsigned int num_tokens)
{
    ASSERTN(parser_p != NULL, EINVAL);
    ASSERTN(js_p != NULL, EINVAL);
    ASSERTN(len > 0, EINVAL);
    ASSERTN(((tokens_p != NULL) && (num_tokens > 0))
            || (num_tokens == 0), EINVAL);

    int r;
    int i;
    struct json_tok_t *token_p;
    int count = parser_p->toknext;

    for (; ((parser_p->pos < len)
            && (js_p[parser_p->pos] != '\0')); parser_p->pos++) {
        char c;
        enum json_type_t type;

        c = js_p[parser_p->pos];

        switch (c) {

        case '{':
        case '[':
            count++;

            if (tokens_p == NULL) {
                break;
            }

            token_p = alloc_token(parser_p, tokens_p, num_tokens);

            if (token_p == NULL) {
                return (JSON_ERROR_NOMEM);
            }

            if (parser_p->toksuper != -1) {
                tokens_p[parser_p->toksuper].size++;
#ifdef JSON_PARENT_LINKS
                token_p->parent = parser_p->toksuper;
#endif
            }

            token_p->type = (c == '{' ? JSON_OBJECT : JSON_ARRAY);
            token_p->start = parser_p->pos;
            parser_p->toksuper = parser_p->toknext - 1;
            break;

        case '}':
        case ']':
            if (tokens_p == NULL) {
                break;
            }

            type = (c == '}' ? JSON_OBJECT : JSON_ARRAY);

#ifdef JSON_PARENT_LINKS
            if (parser_p->toknext < 1) {
                return (JSON_ERROR_INVAL);
            }

            token = &tokens_p[parser_p->toknext - 1];

            for (;;) {
                if ((token_p->start != -1) && (token_p->end == -1)) {
                    if (token_p->type != type) {
                        return (JSON_ERROR_INVAL);
                    }

                    token_p->end = parser_p->pos + 1;
                    parser_p->toksuper = token_p->parent;
                    break;
                }

                if (token_p->parent == -1) {
                    break;
                }

                token = &tokens_p[token_p->parent];
            }
#else
            for (i = parser_p->toknext - 1; i >= 0; i--) {
                token_p = &tokens_p[i];

                if ((token_p->start != -1) && (token_p->end == -1)) {
                    if (token_p->type != type) {
                        return (JSON_ERROR_INVAL);
                    }

                    parser_p->toksuper = -1;
                    token_p->end = parser_p->pos + 1;
                    break;
                }
            }

            /* Error if unmatched closing bracket */
            if (i == -1) {
                return (JSON_ERROR_INVAL);
            }

            for (; i >= 0; i--) {
                token_p = &tokens_p[i];

                if ((token_p->start != -1) && (token_p->end == -1)) {
                    parser_p->toksuper = i;
                    break;
                }
            }
#endif
            break;
        case '\"':
            r = parse_string(parser_p, js_p, len, tokens_p, num_tokens);
            
            if (r < 0) {
                return (r);
            }

            count++;

            if ((parser_p->toksuper != -1) && (tokens_p != NULL)) {
                tokens_p[parser_p->toksuper].size++;
            }

            break;

        case '\t':
        case '\r':
        case '\n':
        case ' ':
            break;

        case ':':
            parser_p->toksuper = parser_p->toknext - 1;
            break;

        case ',':
            if ((tokens_p != NULL)
                && (parser_p->toksuper != -1)
                && (tokens_p[parser_p->toksuper].type != JSON_ARRAY)
                && (tokens_p[parser_p->toksuper].type != JSON_OBJECT)) {
#ifdef JSON_PARENT_LINKS
                parser_p->toksuper = tokens_p[parser_p->toksuper].parent;
#else
                for (i = parser_p->toknext - 1; i >= 0; i--) {
                    if ((tokens_p[i].type == JSON_ARRAY)
                        || (tokens_p[i].type == JSON_OBJECT)) {
                        if ((tokens_p[i].start != -1)
                            && (tokens_p[i].end == -1)) {
                            parser_p->toksuper = i;
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
            if ((tokens != NULL) && (parser_p->toksuper != -1)) {
                struct json_tok_t *t = &tokens_p[parser_p->toksuper];

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
            r = parse_primitive(parser_p, js_p, len, tokens_p, num_tokens);
            
            if (r < 0) {
                return (r);
            }

            count++;

            if ((parser_p->toksuper != -1) && (tokens_p != NULL)) {
                tokens_p[parser_p->toksuper].size++;
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
        for (i = parser_p->toknext - 1; i >= 0; i--) {
            /* Unmatched opened object or array */
            if ((tokens_p[i].start != -1) && (tokens_p[i].end == -1)) {
                return (JSON_ERROR_PART);
            }
        }
    }

    return (count);
}
