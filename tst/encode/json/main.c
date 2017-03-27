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

static char qoutbuf[64];
static QUEUE_INIT_DECL(qout, qoutbuf, sizeof(qoutbuf));

static int vtokeq(const char *s_p,
                  struct json_tok_t *t_p,
                  int numtok,
                  va_list ap)
{
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
            std_printf(FSTR("token %d type is %d, not %d\r\n"),
                       i, t_p[i].type, type);
            return (0);
        }

        if ((start != -1) && (end != -1)) {
            if ((t_p[i].buf_p - s_p) != start) {
                std_printf(FSTR("token %d start is %d, not %d\r\n"),
                           i, t_p[i].buf_p - s_p, start);
                return (0);
            }

            if ((start + t_p[i].size) != end) {
                std_printf(FSTR("token %d end is %d, not %d\r\n"),
                           i, start + t_p[i].size, end);
                return (0);
            }
        }

        if ((size != -1) && (t_p[i].num_tokens != size)) {
            std_printf(FSTR("token %d size is %d, not %d\r\n"),
                       i, t_p[i].num_tokens, size);
            return (0);
        }

        if ((s_p != NULL) && (value_p != NULL)) {
            const char *p_p = t_p[i].buf_p;

            if ((strlen(value_p) != (t_p[i].size)) ||
                (strncmp(p_p, value_p, t_p[i].size) != 0)) {
                std_printf(FSTR("token %d value is %.*s, not %s\r\n"),
                           i,
                           t_p[i].size,
                           t_p[i].buf_p,
                           value_p);
                return (0);
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

/**
 * Dump given array of tokens into r_p.
 */
static int dumps(const char *r_p,
                 int status,
                 int numtok,
                 ...)
{
    int i;
    int r;
    int ok = 1;
    va_list ap;
    char buf[512];
    struct json_tok_t t[128];
    struct json_t json;
    
    va_start(ap, numtok);

    for (i = 0; i < numtok; i++) {
        t[i].type = va_arg(ap, int);
        t[i].buf_p = va_arg(ap, char *);
        t[i].size = va_arg(ap, int);
        t[i].num_tokens = va_arg(ap, int);
    }

    va_end(ap);

    json_init(&json, t, numtok);
    r = json_dumps(&json, NULL, buf);

    if (r != status) {
        std_printf(FSTR("status is %d, not %d\r\n"), r, status);
        return (0);
    }

    if (status >= 0) {
        if (memcmp(r_p, buf, status) != 0) {
            ok = 0;

            std_printf(FSTR("encoded data is '"));

            for (i = 0; i < status; i++) {
                std_printf(FSTR("%c"), r_p[i]);
            }

            std_printf(FSTR("', not '"));

            for (i = 0; i < status; i++) {
                std_printf(FSTR("%c"), buf[i]);
            }

            std_printf(FSTR("'\r\n"));
        }
    }

    return (ok);
}

static int parse(const FAR char *s_p,
                 int status,
                 int numtok,
                 ...)
{
    int r;
    int ok = 1;
    va_list args;
    struct json_tok_t t[128];
    char buf[512];
    struct json_t json;

    if (numtok > 128) {
        return (0);
    }

    std_strcpy(buf, s_p);

    json_init(&json, t, numtok);
    r = json_parse(&json, buf, strlen(buf));

    if (r != status) {
        std_printf(FSTR("status is %d, not %d\r\n"), r, status);
        return (0);
    }

    if (status >= 0) {
        va_start(args, numtok);
        ok = vtokeq(buf, t, numtok, args);
        va_end(args);
    }

    return (ok);
}

static int parse_no_check(const FAR char *s_p)
{
    struct json_tok_t t[128];
    char buf[512];
    struct json_t json;

    std_strcpy(buf, s_p);
    
    json_init(&json, t, 128);
    
    return (json_parse(&json, buf, strlen(buf)));
}

static int test_empty(struct harness_t *harness_p)
{
    BTASSERT(parse(FSTR("{}"), 1, 1,
                   JSON_OBJECT, 0, 2, 0));
    BTASSERT(parse(FSTR("[]"), 1, 1,
                   JSON_ARRAY, 0, 2, 0));
    BTASSERT(parse(FSTR("[{},{}]"), 3, 3,
                   JSON_ARRAY, 0, 7, 2,
                   JSON_OBJECT, 1, 3, 0,
                   JSON_OBJECT, 4, 6, 0));

    BTASSERT(dumps("{}", 2, 1,
                   JSON_OBJECT, 0, 2, 0));

    return (0);
}

static int test_object(struct harness_t *harness_p)
{
    BTASSERT(parse(FSTR("{\"a\":0}"), 3, 3,
                   JSON_OBJECT, 0, 7, 1,
                   JSON_STRING, "a", 1,
                   JSON_PRIMITIVE, "0"));
    BTASSERT(parse(FSTR("{\"a\":[]}"), 3, 3,
                   JSON_OBJECT, 0, 8, 1,
                   JSON_STRING, "a", 1,
                   JSON_ARRAY, 5, 7, 0));
    BTASSERT(parse(FSTR("{\"a\":{},\"b\":{}}"), 5, 5,
                   JSON_OBJECT, -1, -1, 2,
                   JSON_STRING, "a", 1,
                   JSON_OBJECT, -1, -1, 0,
                   JSON_STRING, "b", 1,
                   JSON_OBJECT, -1, -1, 0));
    BTASSERT(parse(FSTR("{\n \"Day\":26,\n \"Month\":9,\n \"Year\":12\n }"),
                   7, 7,
                   JSON_OBJECT, -1, -1, 3,
                   JSON_STRING, "Day", 1,
                   JSON_PRIMITIVE, "26",
                   JSON_STRING, "Month", 1,
                   JSON_PRIMITIVE, "9",
                   JSON_STRING, "Year", 1,
                   JSON_PRIMITIVE, "12"));
    BTASSERT(parse(FSTR("{\"a\":0,\"b\":\"c\"}"), 5, 5,
                   JSON_OBJECT, -1, -1, 2,
                   JSON_STRING, "a", 1,
                   JSON_PRIMITIVE, "0",
                   JSON_STRING, "b", 1,
                   JSON_STRING, "c", 0));

#ifdef JSON_STRICT
    BTASSERT(parse(FSTR("{\"a\"\n0}"), JSON_ERROR_INVAL, 3));
    BTASSERT(parse(FSTR("{\"a\",0}"), JSON_ERROR_INVAL, 3));
    BTASSERT(parse(FSTR("{\"a\":{2}}"), JSON_ERROR_INVAL, 3));
    BTASSERT(parse(FSTR("{\"a\":{2:3}}"), JSON_ERROR_INVAL, 3));
    BTASSERT(parse(FSTR("{\"a\":{\"a\":2 3}}"), JSON_ERROR_INVAL, 5));
    /* FIXME */
    /*BTASSERT(parse("{\"a\"}", JSON_ERROR_INVAL, 2));*/
    /*BTASSERT(parse("{\"a\":1, \"b\"}", JSON_ERROR_INVAL, 4));*/
    /*BTASSERT(parse("{\"a\",\"b\":1}", JSON_ERROR_INVAL, 4));*/
    /*BTASSERT(parse("{\"a\":1,}", JSON_ERROR_INVAL, 4));*/
    /*BTASSERT(parse("{\"a\":\"b\":\"c\"}", JSON_ERROR_INVAL, 4));*/
    /*BTASSERT(parse("{,}", JSON_ERROR_INVAL, 4));*/
#endif

    return (0);
}

static int test_array(struct harness_t *harness_p)
{
    /* FIXME */
    /*BTASSERT(parse("[10}", JSON_ERROR_INVAL, 3));*/
    /*BTASSERT(parse("[1,,3]", JSON_ERROR_INVAL, 3)*/
    BTASSERT(parse(FSTR("[10]"), 2, 2,
                   JSON_ARRAY, -1, -1, 1,
                   JSON_PRIMITIVE, "10"));
    BTASSERT(parse(FSTR("{\"a\":1]"), JSON_ERROR_INVAL, 3));
    /* FIXME */
    /*BTASSERT(parse("[\"a\":1]", JSON_ERROR_INVAL, 3));*/

    BTASSERT(parse("[10]]", JSON_ERROR_INVAL, 3));

    return (0);
}

static int test_primitive(struct harness_t *harness_p)
{
    BTASSERT(parse(FSTR("{\"boolVar\":true }"), 3, 3,
                   JSON_OBJECT, -1, -1, 1,
                   JSON_STRING, "boolVar", 1,
                   JSON_PRIMITIVE, "true"));
    BTASSERT(parse(FSTR("{\"boolVar\":false }"), 3, 3,
                   JSON_OBJECT, -1, -1, 1,
                   JSON_STRING, "boolVar", 1,
                   JSON_PRIMITIVE, "false"));
    BTASSERT(parse(FSTR("{\"nullVar\":null }"), 3, 3,
                   JSON_OBJECT, -1, -1, 1,
                   JSON_STRING, "nullVar", 1,
                   JSON_PRIMITIVE, "null"));
    BTASSERT(parse(FSTR("{\"intVar\":12}"), 3, 3,
                   JSON_OBJECT, -1, -1, 1,
                   JSON_STRING, "intVar", 1,
                   JSON_PRIMITIVE, "12"));
    BTASSERT(parse(FSTR("{\"floatVar\":12.345}"), 3, 3,
                   JSON_OBJECT, -1, -1, 1,
                   JSON_STRING, "floatVar", 1,
                   JSON_PRIMITIVE, "12.345"));
    BTASSERT(parse(FSTR("{\"badVar\":12\x03.34}"), JSON_ERROR_INVAL, 3,
                   JSON_OBJECT));

    return (0);
}

static int test_string(struct harness_t *harness_p)
{
    BTASSERT(parse(FSTR("{\"strVar\":\"hello world\"}"), 3, 3,
                   JSON_OBJECT, -1, -1, 1,
                   JSON_STRING, "strVar", 1,
                   JSON_STRING, "hello world", 0));
    BTASSERT(parse(FSTR("{\"strVar\":\"escapes:\\/\\r\\n\\t\\b\\f\\\"\\\\\"}"),
                   3, 3,
                   JSON_OBJECT, -1, -1, 1,
                   JSON_STRING, "strVar", 1,
                   JSON_STRING, "escapes:\\/\\r\\n\\t\\b\\f\\\"\\\\", 0));
    BTASSERT(parse(FSTR("{\"strVar\":\"\"}"), 3, 3,
                   JSON_OBJECT, -1, -1, 1,
                   JSON_STRING, "strVar", 1,
                   JSON_STRING, "", 0));
    BTASSERT(parse(FSTR("{\"a\":\"\\uAbcD\"}"), 3, 3,
                   JSON_OBJECT, -1, -1, 1,
                   JSON_STRING, "a", 1,
                   JSON_STRING, "\\uAbcD", 0));
    BTASSERT(parse(FSTR("{\"a\":\"str\\u0000\"}"), 3, 3,
                   JSON_OBJECT, -1, -1, 1,
                   JSON_STRING, "a", 1,
                   JSON_STRING, "str\\u0000", 0));
    BTASSERT(parse(FSTR("{\"a\":\"\\uFFFFstr\"}"), 3, 3,
                   JSON_OBJECT, -1, -1, 1,
                   JSON_STRING, "a", 1,
                   JSON_STRING, "\\uFFFFstr", 0));
    BTASSERT(parse(FSTR("{\"a\":[\"\\u0280\"]}"), 4, 4,
                   JSON_OBJECT, -1, -1, 1,
                   JSON_STRING, "a", 1,
                   JSON_ARRAY, -1, -1, 1,
                   JSON_STRING, "\\u0280", 0));

    BTASSERT(parse(FSTR("{\"a\":\"str\\uFFGFstr\"}"), JSON_ERROR_INVAL, 3));
    BTASSERT(parse(FSTR("{\"a\":\"str\\u@FfF\"}"), JSON_ERROR_INVAL, 3));
    BTASSERT(parse(FSTR("{{\"a\":[\"\\u028\"]}"), JSON_ERROR_INVAL, 4));
    BTASSERT(parse(FSTR("{{\"a\":[\"\\X\"]}"), JSON_ERROR_INVAL, 4));

    return (0);
}

static int test_partial_string(struct harness_t *harness_p)
{
    int i;
    int r;
    struct json_tok_t tok[5];
    const char *js_p = "{\"x\":\"va\\\\ue\",\"y\":\"value y\"}";
    struct json_t json;

    for (i = 1; i <= strlen(js_p); i++) {
        json_init(&json, tok, sizeof(tok)/sizeof(tok[0]));
        r = json_parse(&json, js_p, i);

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

static int test_partial_array(struct harness_t *harness_p)
{
#ifdef JSON_STRICT
    int r;
    int i;
    json_parser p;
    jsontok_t tok[10];
    const char *js_p;

    js_p = "[ 1,true,[123,\"hello\"]]";

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

static int test_array_nomem(struct harness_t *harness_p)
{
    int i;
    int r;
    struct json_tok_t toksmall[10], toklarge[10];
    const char *js_p;
    struct json_t json;

    js_p = "  [ 1,true,[123,\"hello\"]]";

    for (i = 0; i < 6; i++) {
        memset(toksmall, 0, sizeof(toksmall));
        memset(toklarge, 0, sizeof(toklarge));
        json_init(&json, toksmall, i);
        r = json_parse(&json, js_p, strlen(js_p));
        BTASSERT(r == JSON_ERROR_NOMEM);

        memcpy(toklarge, toksmall, sizeof(toksmall));

        json_init(&json, toklarge, 10);
        r = json_parse(&json, js_p, strlen(js_p));
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

static int test_unquoted_keys(struct harness_t *harness_p)
{
#ifndef JSON_STRICT
    int r;
    struct json_tok_t tok[10];
    const char *js_p;
    struct json_t json;

    js_p = "key1:\"value\"\nkey2:123";

    json_init(&json, tok, 10);
    r = json_parse(&json, js_p, strlen(js_p));

    BTASSERT(r >= 0);
    BTASSERT(tokeq(js_p, tok, 4,
                   JSON_PRIMITIVE, "key1",
                   JSON_STRING, "value", 0,
                   JSON_PRIMITIVE, "key2",
                   JSON_PRIMITIVE, "123"));
#endif

    return (0);
}

static int test_issue_22(struct harness_t *harness_p)
{
    const FAR char *js_p;

    js_p =
        FSTR("{ \"height\":10,\"layers\":[ { \"data\":[6,6],\"height\":10,"
             "\"name\":\"Calque de Tile 1\",\"opacity\":1,\"type\":\"tilelayer\","
             "\"visible\":true,\"width\":10,\"x\":0,\"y\":0 }],"
             "\"orientation\":\"orthogonal\",\"properties\":{ },\"tileheight\":32,"
             "\"tilesets\":[ { \"firstgid\":1,\"image\":\"..\\/images\\/tiles.png\","
             "\"imageheight\":64,\"imagewidth\":160,\"margin\":0,\"name\":\"Tiles\","
             "\"properties\":{},\"spacing\":0,\"tileheight\":32,\"tilewidth\":32 }],"
             "\"tilewidth\":32,\"version\":1,\"width\":10 }");

    BTASSERT(parse_no_check(js_p) >= 0);

    return (0);
}

static int test_issue_27(struct harness_t *harness_p)
{
    const FAR char *js_p;

    js_p = FSTR("{ \"name\":\"Jack\",\"age\":27 } { \"name\":\"Anna\",");

    BTASSERT(parse(js_p, JSON_ERROR_PART, 8));

    return (0);
}

static int test_input_length(struct harness_t *harness_p)
{
    const char *js_p;
    int r;
    struct json_tok_t tokens[10];
    struct json_t json;

    js_p = "{\"a\":0}garbage";

    json_init(&json, tokens, 10);
    r = json_parse(&json, js_p, 7);
    BTASSERT(r == 3);
    BTASSERT(tokeq(js_p, tokens, 3,
                   JSON_OBJECT, -1, -1, 1,
                   JSON_STRING, "a", 1,
                   JSON_PRIMITIVE, "0"));
    return (0);
}

static int test_count(struct harness_t *harness_p)
{
    BTASSERT(parse_no_check(FSTR("{}")) == 1);
    BTASSERT(parse_no_check("[]") == 1);
    BTASSERT(parse_no_check(FSTR("[[]]")) == 2);
    BTASSERT(parse_no_check(FSTR("[[],[]]")) == 3);
    BTASSERT(parse_no_check(FSTR("[{},{}]")) == 3);
    BTASSERT(parse_no_check(FSTR("[[],[[]],[[],[]]]")) == 7);
    BTASSERT(parse_no_check(FSTR("[\"a\",[[],[]]]")) == 5);
    BTASSERT(parse_no_check(FSTR("[[],\"[],[[]]\",[[]]]")) == 5);
    BTASSERT(parse_no_check(FSTR("[1,2,3]")) == 4);
    BTASSERT(parse_no_check(FSTR("[1,2,[3,\"a\"],null]")) == 7);

    return (0);
}

static int test_nonstrict(struct harness_t *harness_p)
{
#ifndef JSON_STRICT
    BTASSERT(parse(FSTR("a:0garbage"), 2, 2,
                   JSON_PRIMITIVE, "a",
                   JSON_PRIMITIVE, "0garbage"));

    BTASSERT(parse(FSTR("Day:26\nMonth:Sep\n\nYear:12"), 6, 6,
                   JSON_PRIMITIVE, "Day",
                   JSON_PRIMITIVE, "26",
                   JSON_PRIMITIVE, "Month",
                   JSON_PRIMITIVE, "Sep",
                   JSON_PRIMITIVE, "Year",
                   JSON_PRIMITIVE, "12"));
#endif

    return (0);
}

static int test_dumps(struct harness_t *harness_p)
{
    ssize_t status;
    struct json_tok_t tokens[32];
    char buf[128];
    struct json_t json;
    
    /* Empty object. */
    json_token_object(&tokens[0], 0);
    memset(buf, -1, sizeof(buf));
    json_init(&json, tokens, 1);
    status = json_dumps(&json, NULL, buf);
    BTASSERT(status == 2);
    BTASSERT(strlen(buf) == 2);
    BTASSERT(memcmp(buf, "{}", status) == 0);

    /* Test all valid types as keys. */
    json_token_object(&tokens[0], 5);
    json_token_true(&tokens[1]);
    json_token_null(&tokens[2]);
    json_token_false(&tokens[3]);
    json_token_null(&tokens[4]);
    json_token_null(&tokens[5]);
    json_token_null(&tokens[6]);
    json_token_number(&tokens[7], "12.345", 6);
    json_token_null(&tokens[8]);
    json_token_string(&tokens[9], "foo", 3);
    json_token_null(&tokens[10]);
    memset(buf, -1, sizeof(buf));
    json_init(&json, tokens, 11);
    status = json_dumps(&json, NULL, buf);
    BTASSERT(status == 55);
    BTASSERT(strlen(buf) == 55);
    BTASSERT(std_strcmp(buf,
                        FSTR("{"
                             "true:null,"
                             "false:null,"
                             "null:null,"
                             "12.345:null,"
                             "\"foo\":null"
                             "}")) == 0);
    json_init(&json, NULL, 0);
    BTASSERT(json_parse(&json, buf, status) == 11);

    /* Test an array as top object token. */
    json_token_array(&tokens[0], 1);
    json_token_true(&tokens[1]);
    memset(buf, -1, sizeof(buf));
    json_init(&json, tokens, 2);
    status = json_dumps(&json, NULL, buf);
    BTASSERT(status == 6);
    BTASSERT(strlen(buf) == 6);
    BTASSERT(std_strcmp(buf, FSTR("[true]")) == 0);

    /* Empty array. */
    json_token_array(&tokens[0], 0);
    memset(buf, -1, sizeof(buf));
    json_init(&json, tokens, 1);
    status = json_dumps(&json, NULL, buf);
    BTASSERT(status == 2);
    BTASSERT(strlen(buf) == 2);
    BTASSERT(std_strcmp(buf, FSTR("[]")) == 0);

    /* Object in array. */
    json_token_array(&tokens[0], 2);
    json_token_object(&tokens[1], 1);
    json_token_true(&tokens[2]);
    json_token_false(&tokens[3]);
    json_token_null(&tokens[4]);
    memset(buf, -1, sizeof(buf));
    json_init(&json, tokens, 5);
    status = json_dumps(&json, NULL, buf);
    BTASSERT(status == 19);
    BTASSERT(strlen(buf) == 19);
    BTASSERT(std_strcmp(buf, FSTR("[{true:false},null]")) == 0);

    return (0);
}

static int test_dumps_fail(struct harness_t *harness_p)
{
    struct json_tok_t tokens[32];
    char buf[128];
    struct json_t json;

    /* Test all invalid types as keys in an object. */
    json_token_object(&tokens[0], 1);
    json_token_array(&tokens[1], 0);
    json_init(&json, tokens, 2);
    BTASSERT(json_dumps(&json, NULL, buf) == -1);

    /* Test all invalid types as keys in an object. */
    json_token_object(&tokens[0], 1);
    json_token_object(&tokens[1], 0);
    json_init(&json, tokens, 2);
    BTASSERT(json_dumps(&json, NULL, buf) == -1);

    /* Only array and object are allowed as the top level token. */
    json_token_null(&tokens[0]);
    json_init(&json, tokens, 1);
    BTASSERT(json_dumps(&json, NULL, buf) == -1);
    json_token_true(&tokens[0]);
    json_init(&json, tokens, 1);
    BTASSERT(json_dumps(&json, NULL, buf) == -1);
    json_token_false(&tokens[0]);
    json_init(&json, tokens, 1);
    BTASSERT(json_dumps(&json, NULL, buf) == -1);
    json_token_number(&tokens[0], "1", 1);
    json_init(&json, tokens, 1);
    BTASSERT(json_dumps(&json, NULL, buf) == -1);
    json_token_string(&tokens[0], "a", 1);
    json_init(&json, tokens, 1);
    BTASSERT(json_dumps(&json, NULL, buf) == -1);

    /* Too few tokens for object. Both key and value missing. */
    json_token_object(&tokens[0], 1);
    json_init(&json, tokens, 1);
    BTASSERT(json_dumps(&json, NULL, buf) == -1);

    /* Too few tokens for object. Key given, but value is missing. */
    json_token_object(&tokens[0], 1);
    json_token_string(&tokens[1], "foo", 3);
    json_init(&json, tokens, 2);
    BTASSERT(json_dumps(&json, NULL, buf) == -1);

    /* Too few tokens for array. */
    json_token_array(&tokens[0], 1);
    json_init(&json, tokens, 1);
    BTASSERT(json_dumps(&json, NULL, buf) == -1);

    /* Bad token type. */
    json_token_array(&tokens[0], 1);
    tokens[1].type = 99;
    BTASSERT(json_init(&json, tokens, 2) == 0);
    BTASSERT(json_dumps(&json, NULL, buf) == -1);
    
    /* Bad token in object key. */
    json_token_object(&tokens[0], 1);
    json_token_string(&tokens[1], "\x01", 1);
    BTASSERT(json_init(&json, tokens, 2) == 0);
    BTASSERT(json_dumps(&json, NULL, buf) == -1);

    return (0);
}

static int test_dump(struct harness_t *harness_p)
{
    ssize_t status;
    struct json_tok_t tokens[32];
    char buf[128];
    struct json_t json;

    /* Empty object. */
    json_token_object(&tokens[0], 0);
    memset(buf, -1, sizeof(buf));
    json_init(&json, tokens, 1);
    status = json_dump(&json, NULL, &qout);
    BTASSERT(status == 2);
    BTASSERT(queue_size(&qout) == 2);
    BTASSERT(queue_read(&qout, buf, 2) == 2);
    BTASSERT(memcmp(buf, "{}", 2) == 0);

    return (0);
}

static int test_get(struct harness_t *harness_p)
{
    struct json_t json;
    struct json_tok_t tokens[64];
    struct json_tok_t *foo_p, *ten_p, *fie_p, *true_p, *one_p;
    char js_p[] = "{"
        "\"foo\":[10, {\"fie\":null}],"
        "\"true\":null,"
        "true:null,"
        "1:null"
        "}";
    
    BTASSERT(json_init(&json, tokens, membersof(tokens)) == 0);
    BTASSERT(json_parse(&json, js_p, strlen(js_p)) == 13);

    /* Get from an object. */
    foo_p = json_object_get(&json, "foo", json_root(&json));
    BTASSERT(foo_p == &tokens[2]);

    true_p = json_object_get(&json, "true", json_root(&json));
    BTASSERT(true_p == &tokens[8]);

    true_p = json_object_get_primitive(&json, "true", json_root(&json));
    BTASSERT(true_p == &tokens[10]);

    one_p = json_object_get_primitive(&json, "1", json_root(&json));
    BTASSERT(one_p == &tokens[12]);

    BTASSERT(json_object_get(&json, "1", json_root(&json)) == NULL);
    BTASSERT(json_object_get(&json, "fum", json_root(&json)) == NULL);

    /* Get from an array. */
    ten_p = json_array_get(&json, 0, foo_p);
    BTASSERT(ten_p != &tokens[4]);

    fie_p = json_object_get(&json, "fie", json_array_get(&json, 1, foo_p));
    BTASSERT(fie_p != &tokens[7]);
    
    /* Get outside the array should fail. */
    BTASSERT(json_array_get(&json, 2, foo_p) == NULL);

    /* Fail to get from NULL token. */
    BTASSERT(json_array_get(&json, 0, NULL) == NULL);
    BTASSERT(json_object_get(&json, "foo", NULL) == NULL);

    /* Using the object get function on a non-object token should
       fail. */
    BTASSERT(json_object_get(&json, "foo", ten_p) == NULL);
    
    /* Using the array get function on a non-array token should
       fail. */
    BTASSERT(json_array_get(&json, 0, json_root(&json)) == NULL);
    
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
        { test_dumps, "test_dumps" },
        { test_dumps_fail, "test_dumps_fail" },
        { test_dump, "test_dump" },
        { test_get, "test_get" },
        { NULL, NULL }
    };

    sys_start();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
