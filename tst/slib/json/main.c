/**
 * @file main.c
 * @version 0.7.0
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

static int vtokeq(const char *s_p,
                  struct json_tok_t *t_p,
                  int numtok,
                  va_list ap)
{
    if (numtok > 0) {
        int i, start, end, size;
        int type;
        char *value_p;

        size = -1;
        value_p = NULL;

        for (i = 0; i < numtok; i++) {
            type = va_arg(ap, int);

            if (type == JSON_STRING) {
                value_p = va_arg(ap, char *);
                size = va_arg(ap, int);
                start = end = -1;
            } else if (type == JSON_PRIMITIVE) {
                value_p = va_arg(ap, char *);
                start = end = size = -1;
            } else {
                start = va_arg(ap, int);
                end = va_arg(ap, int);
                size = va_arg(ap, int);
                value_p = NULL;
            }

            if (t_p[i].type != type) {
                printf("token %d type is %d, not %d\n",
                       i, t_p[i].type, type);
                return (0);
            }

            if ((start != -1) && (end != -1)) {
                if (t_p[i].start != start) {
                    printf("token %d start is %d, not %d\n",
                           i, t_p[i].start, start);
                    return (0);
                }

                if (t_p[i].end != end ) {
                    printf("token %d end is %d, not %d\n",
                           i, t_p[i].end, end);
                    return (0);
                }
            }

            if ((size != -1) && (t_p[i].size != size)) {
                printf("token %d size is %d, not %d\n",
                       i, t_p[i].size, size);
                return (0);
            }

            if ((s_p != NULL) && (value_p != NULL)) {
                const char *p_p = (s_p + t_p[i].start);

                if ((strlen(value_p) != t_p[i].end - t_p[i].start) ||
                    (strncmp(p_p, value_p, t_p[i].end - t_p[i].start) != 0)) {
                    printf("token %d value is %.*s, not %s\n",
                           i,
                           t_p[i].end - t_p[i].start,
                           s_p + t_p[i].start,
                           value_p);
                    return (0);
                }
            }
        }
    }

    return (1);
}

static int tokeq(const char *s_p,
                 struct json_tok_t *tokens_p,
                 int numtok,
                 ...)
{
    int ok;
    va_list args;

    va_start(args, numtok);
    ok = vtokeq(s_p, tokens_p, numtok, args); 
    va_end(args);

    return (ok);
}

static int parse(const char *s_p,
                 int status,
                 int numtok,
                 ...)
{
    int r;
    int ok = 1;
    va_list args;
    struct json_parser_t p;
    static struct json_tok_t t[128];

    if (numtok > 128) {
        printf("apa\r\n");
        exit(1);
        return (0);
    }

    json_init(&p);
    r = json_parse(&p, s_p, strlen(s_p), t, numtok);

    if (r != status) {
        printf("status is %d, not %d\n", r, status);
        return (0);
    }

    if (status >= 0) {
        va_start(args, numtok);
        ok = vtokeq(s_p, t, numtok, args); 
        va_end(args);
    }

    return (ok);
}

int test_empty(struct harness_t *harness_p)
{
    BTASSERT(parse("{}", 1, 1,
                   JSON_OBJECT, 0, 2, 0));
    BTASSERT(parse("[]", 1, 1,
                   JSON_ARRAY, 0, 2, 0));
    BTASSERT(parse("[{},{}]", 3, 3,
                   JSON_ARRAY, 0, 7, 2,
                   JSON_OBJECT, 1, 3, 0,
                   JSON_OBJECT, 4, 6, 0));

    return (0);
}

int test_object(struct harness_t *harness_p)
{
    BTASSERT(parse("{\"a\":0}", 3, 3,
                   JSON_OBJECT, 0, 7, 1,
                   JSON_STRING, "a", 1,
                   JSON_PRIMITIVE, "0"));
    BTASSERT(parse("{\"a\":[]}", 3, 3,
                   JSON_OBJECT, 0, 8, 1,
                   JSON_STRING, "a", 1,
                   JSON_ARRAY, 5, 7, 0));
    BTASSERT(parse("{\"a\":{},\"b\":{}}", 5, 5,
                   JSON_OBJECT, -1, -1, 2,
                   JSON_STRING, "a", 1,
                   JSON_OBJECT, -1, -1, 0,
                   JSON_STRING, "b", 1,
                   JSON_OBJECT, -1, -1, 0));
    BTASSERT(parse("{\n \"Day\": 26,\n \"Month\": 9,\n \"Year\": 12\n }",
                   7, 7,
                   JSON_OBJECT, -1, -1, 3,
                   JSON_STRING, "Day", 1,
                   JSON_PRIMITIVE, "26",
                   JSON_STRING, "Month", 1,
                   JSON_PRIMITIVE, "9",
                   JSON_STRING, "Year", 1,
                   JSON_PRIMITIVE, "12"));
    BTASSERT(parse("{\"a\": 0, \"b\": \"c\"}", 5, 5,
                   JSON_OBJECT, -1, -1, 2,
                   JSON_STRING, "a", 1,
                   JSON_PRIMITIVE, "0",
                   JSON_STRING, "b", 1,
                   JSON_STRING, "c", 0));

#ifdef JSON_STRICT
    BTASSERT(parse("{\"a\"\n0}", JSON_ERROR_INVAL, 3));
    BTASSERT(parse("{\"a\", 0}", JSON_ERROR_INVAL, 3));
    BTASSERT(parse("{\"a\": {2}}", JSON_ERROR_INVAL, 3));
    BTASSERT(parse("{\"a\": {2: 3}}", JSON_ERROR_INVAL, 3));
    BTASSERT(parse("{\"a\": {\"a\": 2 3}}", JSON_ERROR_INVAL, 5));
    /* FIXME */
    /*BTASSERT(parse("{\"a\"}", JSON_ERROR_INVAL, 2));*/
    /*BTASSERT(parse("{\"a\": 1, \"b\"}", JSON_ERROR_INVAL, 4));*/
    /*BTASSERT(parse("{\"a\",\"b\":1}", JSON_ERROR_INVAL, 4));*/
    /*BTASSERT(parse("{\"a\":1,}", JSON_ERROR_INVAL, 4));*/
    /*BTASSERT(parse("{\"a\":\"b\":\"c\"}", JSON_ERROR_INVAL, 4));*/
    /*BTASSERT(parse("{,}", JSON_ERROR_INVAL, 4));*/
#endif

    return (0);
}

int test_array(struct harness_t *harness_p)
{
    /* FIXME */
    /*BTASSERT(parse("[10}", JSON_ERROR_INVAL, 3));*/
    /*BTASSERT(parse("[1,,3]", JSON_ERROR_INVAL, 3)*/
    BTASSERT(parse("[10]", 2, 2,
                   JSON_ARRAY, -1, -1, 1,
                   JSON_PRIMITIVE, "10"));
    BTASSERT(parse("{\"a\": 1]", JSON_ERROR_INVAL, 3));
    /* FIXME */
    /*BTASSERT(parse("[\"a\": 1]", JSON_ERROR_INVAL, 3));*/

    return (0);
}

int test_primitive(struct harness_t *harness_p)
{
    BTASSERT(parse("{\"boolVar\" : true }", 3, 3,
                   JSON_OBJECT, -1, -1, 1,
                   JSON_STRING, "boolVar", 1,
                   JSON_PRIMITIVE, "true"));
    BTASSERT(parse("{\"boolVar\" : false }", 3, 3,
                   JSON_OBJECT, -1, -1, 1,
                   JSON_STRING, "boolVar", 1,
                   JSON_PRIMITIVE, "false"));
    BTASSERT(parse("{\"nullVar\" : null }", 3, 3,
                   JSON_OBJECT, -1, -1, 1,
                   JSON_STRING, "nullVar", 1,
                   JSON_PRIMITIVE, "null"));
    BTASSERT(parse("{\"intVar\" : 12}", 3, 3,
                   JSON_OBJECT, -1, -1, 1,
                   JSON_STRING, "intVar", 1,
                   JSON_PRIMITIVE, "12"));
    BTASSERT(parse("{\"floatVar\" : 12.345}", 3, 3,
                   JSON_OBJECT, -1, -1, 1,
                   JSON_STRING, "floatVar", 1,
                   JSON_PRIMITIVE, "12.345"));

    return (0);
}

int test_string(struct harness_t *harness_p)
{
    BTASSERT(parse("{\"strVar\" : \"hello world\"}", 3, 3,
                   JSON_OBJECT, -1, -1, 1,
                   JSON_STRING, "strVar", 1,
                   JSON_STRING, "hello world", 0));
    BTASSERT(parse("{\"strVar\" : \"escapes: \\/\\r\\n\\t\\b\\f\\\"\\\\\"}",
                   3, 3,
                   JSON_OBJECT, -1, -1, 1,
                   JSON_STRING, "strVar", 1,
                   JSON_STRING, "escapes: \\/\\r\\n\\t\\b\\f\\\"\\\\", 0));
    BTASSERT(parse("{\"strVar\": \"\"}", 3, 3,
                   JSON_OBJECT, -1, -1, 1,
                   JSON_STRING, "strVar", 1,
                   JSON_STRING, "", 0));
    BTASSERT(parse("{\"a\":\"\\uAbcD\"}", 3, 3,
                   JSON_OBJECT, -1, -1, 1,
                   JSON_STRING, "a", 1,
                   JSON_STRING, "\\uAbcD", 0));
    BTASSERT(parse("{\"a\":\"str\\u0000\"}", 3, 3,
                   JSON_OBJECT, -1, -1, 1,
                   JSON_STRING, "a", 1,
                   JSON_STRING, "str\\u0000", 0));
    BTASSERT(parse("{\"a\":\"\\uFFFFstr\"}", 3, 3,
                   JSON_OBJECT, -1, -1, 1,
                   JSON_STRING, "a", 1,
                   JSON_STRING, "\\uFFFFstr", 0));
    BTASSERT(parse("{\"a\":[\"\\u0280\"]}", 4, 4,
                   JSON_OBJECT, -1, -1, 1,
                   JSON_STRING, "a", 1,
                   JSON_ARRAY, -1, -1, 1,
                   JSON_STRING, "\\u0280", 0));

    BTASSERT(parse("{\"a\":\"str\\uFFGFstr\"}", JSON_ERROR_INVAL, 3));
    BTASSERT(parse("{\"a\":\"str\\u@FfF\"}", JSON_ERROR_INVAL, 3));
    BTASSERT(parse("{{\"a\":[\"\\u028\"]}", JSON_ERROR_INVAL, 4));

    return (0);
}

int test_partial_string(struct harness_t *harness_p)
{
    int i;
    int r;
    struct json_parser_t p;
    struct json_tok_t tok[5];
    const char *js_p = "{\"x\": \"va\\\\ue\", \"y\": \"value y\"}";

    json_init(&p);

    for (i = 1; i <= strlen(js_p); i++) {
        r = json_parse(&p, js_p, i, tok, sizeof(tok)/sizeof(tok[0]));

        if (i == strlen(js_p)) {
            BTASSERT(r == 5);
            BTASSERT(tokeq(js_p, tok, 5,
                           JSON_OBJECT, -1, -1, 2,
                           JSON_STRING, "x", 1,
                           JSON_STRING, "va\\\\ue", 0,
                           JSON_STRING, "y", 1,
                           JSON_STRING, "value y", 0));
        } else {
            BTASSERT(r == JSON_ERROR_PART);
        }
    }

    return (0);
}

int test_partial_array(struct harness_t *harness_p)
{
#ifdef JSON_STRICT
    int r;
    int i;
    json_parser p;
    jsontok_t tok[10];
    const char *js_p;

    js_p = "[ 1, true, [123, \"hello\"]]";

    json_init(&p);
   
    for (i = 1; i <= strlen(js); i++) {
        r = json_parse(&p, js, i, tok, sizeof(tok)/sizeof(tok[0]));
    
        if (i == strlen(js)) {
            BTASSERT(r == 6);
            BTASSERT(tokeq(js, tok, 6,
                           JSON_ARRAY, -1, -1, 3,
                           JSON_PRIMITIVE, "1",
                           JSON_PRIMITIVE, "true",
                           JSON_ARRAY, -1, -1, 2,
                           JSON_PRIMITIVE, "123",
                           JSON_STRING, "hello", 0));
        } else {
            BTASSERT(r == JSON_ERROR_PART);
        }
    }
#endif

    return (0);
}

int test_array_nomem(struct harness_t *harness_p)
{
    int i;
    int r;
    struct json_parser_t p;
    struct json_tok_t toksmall[10], toklarge[10];
    const char *js_p;

    js_p = "  [ 1, true, [123, \"hello\"]]";

    for (i = 0; i < 6; i++) {
        json_init(&p);
        memset(toksmall, 0, sizeof(toksmall));
        memset(toklarge, 0, sizeof(toklarge));
        r = json_parse(&p, js_p, strlen(js_p), toksmall, i);
        BTASSERT(r == JSON_ERROR_NOMEM);

        memcpy(toklarge, toksmall, sizeof(toksmall));

        r = json_parse(&p, js_p, strlen(js_p), toklarge, 10);
        BTASSERT(r >= 0);
        BTASSERT(tokeq(js_p, toklarge, 4,
                       JSON_ARRAY, -1, -1, 3,
                       JSON_PRIMITIVE, "1",
                       JSON_PRIMITIVE, "true",
                       JSON_ARRAY, -1, -1, 2,
                       JSON_PRIMITIVE, "123",
                       JSON_STRING, "hello", 0));
    }

    return (0);
}

int test_unquoted_keys(struct harness_t *harness_p)
{
#ifndef JSON_STRICT
    int r;
    struct json_parser_t p;
    struct json_tok_t tok[10];
    const char *js_p;

    js_p = "key1: \"value\"\nkey2 : 123";

    json_init(&p);
    r = json_parse(&p, js_p, strlen(js_p), tok, 10);

    BTASSERT(r >= 0);
    BTASSERT(tokeq(js_p, tok, 4,
                   JSON_PRIMITIVE, "key1",
                   JSON_STRING, "value", 0,
                   JSON_PRIMITIVE, "key2",
                   JSON_PRIMITIVE, "123"));
#endif

    return (0);
}

int test_issue_22(struct harness_t *harness_p)
{
    int r;
    struct json_parser_t p;
    struct json_tok_t tokens[128];
    const char *js_p;

    js_p =
        "{ \"height\":10, \"layers\":[ { \"data\":[6,6], \"height\":10, "
        "\"name\":\"Calque de Tile 1\", \"opacity\":1, \"type\":\"tilelayer\", "
        "\"visible\":true, \"width\":10, \"x\":0, \"y\":0 }], "
        "\"orientation\":\"orthogonal\", \"properties\": { }, \"tileheight\":32, "
        "\"tilesets\":[ { \"firstgid\":1, \"image\":\"..\\/images\\/tiles.png\", "
        "\"imageheight\":64, \"imagewidth\":160, \"margin\":0, \"name\":\"Tiles\", "
        "\"properties\":{}, \"spacing\":0, \"tileheight\":32, \"tilewidth\":32 }], "
        "\"tilewidth\":32, \"version\":1, \"width\":10 }";

    json_init(&p);
    r = json_parse(&p, js_p, strlen(js_p), tokens, 128);

    BTASSERT(r >= 0);

    return (0);
}

int test_issue_27(struct harness_t *harness_p)
{
    const char *js_p;

    js_p = "{ \"name\" : \"Jack\", \"age\" : 27 } { \"name\" : \"Anna\", ";

    BTASSERT(parse(js_p, JSON_ERROR_PART, 8));

    return (0);
}

int test_input_length(struct harness_t *harness_p)
{
    const char *js_p;
    int r;
    struct json_parser_t p;
    struct json_tok_t tokens[10];

    js_p = "{\"a\": 0}garbage";

    json_init(&p);
    r = json_parse(&p, js_p, 8, tokens, 10);
    BTASSERT(r == 3);
    BTASSERT(tokeq(js_p, tokens, 3,
                   JSON_OBJECT, -1, -1, 1,
                   JSON_STRING, "a", 1,
                   JSON_PRIMITIVE, "0"));
    return (0);
}

int test_count(struct harness_t *harness_p)
{
    struct json_parser_t p;
    const char *js_p;

    js_p = "{}";
    json_init(&p);
    BTASSERT(json_parse(&p, js_p, strlen(js_p), NULL, 0) == 1);

    js_p = "[]";
    json_init(&p);
    BTASSERT(json_parse(&p, js_p, strlen(js_p), NULL, 0) == 1);

    js_p = "[[]]";
    json_init(&p);
    BTASSERT(json_parse(&p, js_p, strlen(js_p), NULL, 0) == 2);

    js_p = "[[], []]";
    json_init(&p);
    BTASSERT(json_parse(&p, js_p, strlen(js_p), NULL, 0) == 3);

    js_p = "[{}, {}]";
    json_init(&p);
    BTASSERT(json_parse(&p, js_p, strlen(js_p), NULL, 0) == 3);

    js_p = "[[], [[]], [[], []]]";
    json_init(&p);
    BTASSERT(json_parse(&p, js_p, strlen(js_p), NULL, 0) == 7);

    js_p = "[\"a\", [[], []]]";
    json_init(&p);
    BTASSERT(json_parse(&p, js_p, strlen(js_p), NULL, 0) == 5);

    js_p = "[[], \"[], [[]]\", [[]]]";
    json_init(&p);
    BTASSERT(json_parse(&p, js_p, strlen(js_p), NULL, 0) == 5);

    js_p = "[1, 2, 3]";
    json_init(&p);
    BTASSERT(json_parse(&p, js_p, strlen(js_p), NULL, 0) == 4);

    js_p = "[1, 2, [3, \"a\"], null]";
    json_init(&p);
    BTASSERT(json_parse(&p, js_p, strlen(js_p), NULL, 0) == 7);

    return (0);
}


int test_nonstrict(struct harness_t *harness_p)
{
#ifndef JSON_STRICT
    const char *js_p;

    js_p = "a: 0garbage";

    BTASSERT(parse(js_p, 2, 2,
                   JSON_PRIMITIVE, "a",
                   JSON_PRIMITIVE, "0garbage"));

    js_p = "Day : 26\nMonth : Sep\n\nYear: 12";
    BTASSERT(parse(js_p, 6, 6,
                   JSON_PRIMITIVE, "Day",
                   JSON_PRIMITIVE, "26",
                   JSON_PRIMITIVE, "Month",
                   JSON_PRIMITIVE, "Sep",
                   JSON_PRIMITIVE, "Year",
                   JSON_PRIMITIVE, "12"));
#endif

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_empty, "test_empty" },
        { test_object, "test_object" },
        { test_array, "test_array" },
        { test_primitive, "test_primitive" },
        { test_string, "test_string" },
        { test_partial_string, "test_partial_string" },
        { test_partial_array, "test_partial_array" },
        { test_array_nomem, "test_array_nomem" },
        { test_unquoted_keys, "test_unquoted_keys" },
        { test_input_length, "test_input_length" },
        { test_issue_22, "test_issue_22" },
        { test_issue_27, "test_issue_27" },
        { test_count, "test_count" },
        { test_nonstrict, "test_nonstrict" },
        { NULL, NULL }
    };

    sys_start();
    uart_module_init();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
