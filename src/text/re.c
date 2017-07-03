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

/**
 * Compiled pattern examples:
 *
 * pattern = "foo"
 * code = [
 *     OP_CODE_TEXT, 'f',
 *     OP_CODE_TEXT, 'o',
 *     OP_CODE_TEXT, 'o',
 *     OP_CODE_RETURN
 * ]
 *
 * pattern = "^foo$"
 * code = [
 *     OP_CODE_BEGIN,
 *     OP_CODE_TEXT, 'f',
 *     OP_CODE_TEXT, 'o',
 *     OP_CODE_TEXT, 'o',
 *     OP_CODE_END,
 *     OP_CODE_RETURN
 * ]
 *
 * pattern = "a|bc|d"
 * code = [
 *     OP_CODE_ALTERNATIVES, 3, 2, 4, 2,
 *     OP_CODE_TEXT, 'a',
 *     OP_CODE_TEXT, 'b',
 *     OP_CODE_TEXT, 'c',
 *     OP_CODE_TEXT, 'd',
 *     OP_CODE_RETURN
 * ]
 *
 * pattern = "(a|bc)+"
 * code = [
 *     OP_CODE_ONE_OR_MORE, 0, 11,
 *     OP_CODE_GROUP,
 *     OP_CODE_ALTERNATIVES, 2, 2, 4,
 *     OP_CODE_TEXT, 'a',
 *     OP_CODE_TEXT, 'b',
 *     OP_CODE_TEXT, 'c',
 *     OP_CODE_RETURN
 * ]
 *
 * pattern = "(a|bc)*d(e?)"
 * code = [
 *     OP_CODE_ZERO_OR_MORE, 0, 12,
 *     OP_CODE_GROUP,
 *     OP_CODE_ALTERNATIVES, 2, 2, 4,
 *     OP_CODE_TEXT, 'a',
 *     OP_CODE_TEXT, 'b',
 *     OP_CODE_TEXT, 'c',
 *     OP_CODE_RETURN,
 *     OP_CODE_TEXT, 'd',
 *     OP_CODE_GROUP,
 *     OP_CODE_ZERO_OR_ONE, 0, 2,
 *     OP_CODE_TEXT, 'e',
 *     OP_CODE_RETURN
 * ]
 *
 * pattern = "ab.*cd"
 * code = [
 *     OP_CODE_TEXT, 'a',
 *     OP_CODE_TEXT, 'b',
 *     OP_CODE_ZERO_OR_MORE, 0, 2,
 *     OP_CODE_DOT,
 *     OP_CODE_RETURN,
 *     OP_CODE_TEXT, 'c',
 *     OP_CODE_TEXT, 'd',
 *     OP_CODE_RETURN
 * ]
 *
 * pattern = "[a]"
 * code = [
 *     OP_CODE_SET, 3,
 *     OP_CODE_SET_SINGLE, OP_CODE_TEXT, 'a',
 *     OP_CODE_RETURN
 * ]
 *
 * pattern = "[\wc-d]{3}"
 * code = [
 *     OP_CODE_MEMBERS, 0, 7, 0, 3,
 *     OP_CODE_SET, 7,
 *     OP_CODE_SET_SINGLE, OP_CODE_ALPHANUMERIC,
 *     OP_CODE_SET_RANGE, OP_CODE_TEXT, 'c', OP_CODE_TEXT, 'd',
 *     OP_CODE_RETURN
 * ]
 *
 * pattern = "\x03\n\\n"
 * code = [
 *     OP_CODE_TEXT, '\x03',
 *     OP_CODE_TEXT, '\n',
 *     OP_CODE_TEXT, '\',
 *     OP_CODE_TEXT, 'n',
 *     OP_CODE_RETURN
 * ]
 *
 * pattern = "a|(b|c)"
 * code = [
 *     OP_CODE_ALTERNATIVES, 2, 2, 9,
 *     OP_CODE_TEXT, 'a',
 *     OP_CODE_GROUP,
 *     OP_CODE_ALTERNATIVES, 2, 2, 2,
 *     OP_CODE_TEXT, 'b',
 *     OP_CODE_TEXT, 'c',
 *     OP_CODE_RETURN
 * ]
 *
 */

#include "simba.h"

/* Parser operation codes. */
enum op_code_t {
    OP_CODE_BEGIN = 0,
    OP_CODE_END,
    OP_CODE_TEXT,
    OP_CODE_DOT,
    OP_CODE_WHITESPACE,
    OP_CODE_DECIMAL_DIGIT,
    OP_CODE_ALPHANUMERIC,
    OP_CODE_ZERO_OR_ONE,
    OP_CODE_ZERO_OR_MORE,
    OP_CODE_ONE_OR_MORE,
    OP_CODE_ZERO_OR_ONE_NON_GREEDY,
    OP_CODE_ZERO_OR_MORE_NON_GREEDY,
    OP_CODE_ONE_OR_MORE_NON_GREEDY,
    OP_CODE_MEMBERS,
    OP_CODE_ALTERNATIVES,
    OP_CODE_SET,
    OP_CODE_SET_SINGLE,
    OP_CODE_SET_RANGE,
    OP_CODE_GROUP,
    OP_CODE_RETURN
};

#define NON_GREEDY_OFFSET                 3

struct compile_t {
    char *compiled_p;
    const char *pattern_p;
    char *compiled_begin_p;
    size_t compiled_left;
    char *current_repetition_op_code_p;
};

struct match_t {
    const char *compiled_p;
    char flags;
    const char *buf_p;
    size_t buf_left;
    struct re_group_t *groups_p;
    size_t *number_of_groups_p;
};

struct module_t {
    int initialized;
#if CONFIG_RE_DEBUG_LOG_MASK > -1
    struct log_object_t log;
#endif
};

static struct module_t module;

#if CONFIG_RE_DEBUG_LOG_MASK > -1
#    define DLOG(level, fmt, ...) \
    log_object_print(&module.log, LOG_ ## level, OSTR(fmt), __VA_ARGS__)

/**
 * Debug strings.
 */
static char *op_code_str[] = {
    "BEGIN",
    "END",
    "TEXT",
    "DOT",
    "WHITESPACE",
    "DECIMAL_DIGIT",
    "ALPHANUMERIC",
    "ZERO_OR_ONE",
    "ZERO_OR_MORE",
    "ONE_OR_MORE",
    "ZERO_OR_ONE_NON_GREEDY",
    "ZERO_OR_MORE_NON_GREEDY",
    "ONE_OR_MORE_NON_GREEDY",
    "MEMBERS",
    "ALTERNATIVES",
    "SET",
    "SET_SINGLE",
    "SET_RANGE",
    "GROUP",
    "RETURN"
};

#else
#    define DLOG(level, fmt, ...)
#endif

static int compile_escape_char(struct compile_t *self_p,
                               char escaped_char)
{
    switch (escaped_char) {

    case '\0':
        return (-1);

    case 's':
        *self_p->compiled_p++ = OP_CODE_WHITESPACE;
        break;

    case 'd':
        *self_p->compiled_p++ = OP_CODE_DECIMAL_DIGIT;
        break;

    case 'w':
        *self_p->compiled_p++ = OP_CODE_ALPHANUMERIC;
        break;

    default:
        if (self_p->compiled_left < 1) {
            return (-1);
        }

        *self_p->compiled_p++ = OP_CODE_TEXT;
        *self_p->compiled_p++ = escaped_char;
        self_p->compiled_left--;

        return (2);
    }

    return (1);
}

static int compile_repetition(struct compile_t *self_p,
                              int op_code)
{
    int code_size;

    if (self_p->current_repetition_op_code_p == NULL) {
        return (-1);
    }

    if (self_p->pattern_p[1] == '?') {
        op_code += NON_GREEDY_OFFSET;
        self_p->pattern_p++;
    }

    if (self_p->compiled_left < 3) {
        return (-1);
    }

    /* Insert before last op, group or set. */
    code_size = (self_p->compiled_p - self_p->current_repetition_op_code_p);
    memmove(&self_p->current_repetition_op_code_p[3],
            self_p->current_repetition_op_code_p,
            code_size);
    code_size++;
    self_p->current_repetition_op_code_p[0] = op_code;
    self_p->current_repetition_op_code_p[1] = ((code_size >> 8) & 0xff);
    self_p->current_repetition_op_code_p[2] = (code_size & 0xff);
    self_p->compiled_p[3] = OP_CODE_RETURN;
    self_p->compiled_p += 4;
    self_p->compiled_left -= 3;

    return (0);
}

static int compile_dot(struct compile_t *self_p)
{
    self_p->current_repetition_op_code_p = self_p->compiled_p;
    *self_p->compiled_p++ = OP_CODE_DOT;

    return (0);
}

static int compile_begin(struct compile_t *self_p)
{
    *self_p->compiled_p++ = OP_CODE_BEGIN;

    return (0);
}

static int compile_end(struct compile_t *self_p)
{
    *self_p->compiled_p++ = OP_CODE_END;

    return (0);
}

static int compile_zero_or_one(struct compile_t *self_p)
{
    int op_code = OP_CODE_ZERO_OR_ONE;
    int code_size;

    if (self_p->current_repetition_op_code_p == NULL) {
        return (-1);
    }

    if (self_p->compiled_left < 2) {
        return (-1);
    }

    if (self_p->pattern_p[1] == '?') {
        op_code += NON_GREEDY_OFFSET;
        self_p->pattern_p++;
    }

    /* Insert before last op, group or set. */
    code_size = (self_p->compiled_p - self_p->current_repetition_op_code_p);
    memmove(&self_p->current_repetition_op_code_p[3],
            self_p->current_repetition_op_code_p,
            code_size);
    self_p->current_repetition_op_code_p[0] = op_code;
    self_p->current_repetition_op_code_p[1] = ((code_size >> 8) & 0xff);
    self_p->current_repetition_op_code_p[2] = (code_size & 0xff);
    self_p->compiled_p += 3;
    self_p->compiled_left -= 2;

    return (0);
}

static int compile_members(struct compile_t *self_p)
{
    int number_of_members = 0;
    int code_size;

    if (self_p->current_repetition_op_code_p == NULL) {
        return (-1);
    }

    if (self_p->compiled_left < 5) {
        return (-1);
    }

    self_p->pattern_p++;

    while (isdigit((int)*self_p->pattern_p)) {
        number_of_members *= 10;
        number_of_members += (*self_p->pattern_p++ - '0');
    }

    code_size = (self_p->compiled_p - self_p->current_repetition_op_code_p);
    memmove(&self_p->current_repetition_op_code_p[5],
            self_p->current_repetition_op_code_p,
            code_size);
    code_size++;
    self_p->current_repetition_op_code_p[0] = OP_CODE_MEMBERS;
    self_p->current_repetition_op_code_p[1] = ((code_size >> 8) & 0xff);
    self_p->current_repetition_op_code_p[2] = (code_size & 0xff);
    self_p->current_repetition_op_code_p[3] = ((number_of_members >> 8)
                                               & 0xff);
    self_p->current_repetition_op_code_p[4] = (number_of_members & 0xff);
    self_p->compiled_p[5] = OP_CODE_RETURN;
    self_p->compiled_p += 6;
    self_p->compiled_left -= 5;

    return (0);
}

static int compile_escape(struct compile_t *self_p)
{
    self_p->pattern_p++;
    self_p->current_repetition_op_code_p = self_p->compiled_p;

    return (compile_escape_char(self_p, *self_p->pattern_p));
}

static int compile_set(struct compile_t *self_p)
{
    char pattern_char;
    char *compiled_op_code_p;
    int code_size;
    int res;

    self_p->pattern_p++;
    self_p->current_repetition_op_code_p = self_p->compiled_p;

    /* Check that space is available for the size field. */
    if (self_p->compiled_left < 2) {
        return (-1);
    }

    *self_p->compiled_p++ = OP_CODE_SET;
    code_size = 0;

    /* Make room for the size. */
    self_p->compiled_p += 2;

    /* Accept unescaped ']' and '-' as the first character. */

    /* Make sure a range entry fits in the compile buffer as it is
       bigger than a single entry. */
    if (self_p->compiled_left < 3) {
        return (-1);
    }

    pattern_char = *self_p->pattern_p;

    if ((pattern_char == ']') || (pattern_char == '-')) {
        *self_p->compiled_p++ = OP_CODE_SET_SINGLE;
        *self_p->compiled_p++ = OP_CODE_TEXT;
        *self_p->compiled_p++ = pattern_char;
        pattern_char = *self_p->pattern_p++;
        self_p->compiled_left -= 3;
        code_size += 3;
    }

    while (1) {
        /* Make sure a range entry fits in the compile buffer as it is
           bigger than a single entry. */
        if (self_p->compiled_left < 5) {
            return (-1);
        }

        /* Parse one operation; single or range. */
        pattern_char = *self_p->pattern_p++;

        /* Break when the closing bracket is found. */
        if (pattern_char == ']') {
            self_p->pattern_p--;
            break;
        }

        /* Make room for single/range op code. */
        compiled_op_code_p = self_p->compiled_p;
        self_p->compiled_p++;
        self_p->compiled_left--;
        code_size++;

        /* Handle the first character for this single/range entry. */
        if (pattern_char == '\0') {
            return (-1);
        } else if (pattern_char == '\\') {
            pattern_char = *self_p->pattern_p++;
            res = compile_escape_char(self_p, pattern_char);

            /* Special escape characters are not allowed in a
               range. */
            if ((res < 0)
                || ((res < 2)
                    && (self_p->pattern_p[0] == '-')
                    && (self_p->pattern_p[1] != ']'))) {
                return (-1);
            }

            code_size += res;
        } else if (pattern_char == '-') {
            if (*self_p->pattern_p == ']') {
                *self_p->compiled_p++ = OP_CODE_TEXT;
                *self_p->compiled_p++ = pattern_char;
                self_p->compiled_left -= 2;
                code_size += 2;
            } else {
                return (-1);
            }
        } else {
            *self_p->compiled_p++ = OP_CODE_TEXT;
            *self_p->compiled_p++ = pattern_char;
            self_p->compiled_left -= 2;
            code_size += 2;
        }

        /* Find out if it's a range entry. */
        pattern_char = *self_p->pattern_p;

        /* Create a single entry and break if the closing bracket is
           found. */
        if (pattern_char == ']') {
            *compiled_op_code_p = OP_CODE_SET_SINGLE;
            break;
        }

        /* Create a single entry and continue unless the range
           operater is found at the end of the set. */
        if ((pattern_char != '-') || (self_p->pattern_p[1] == ']')) {
            *compiled_op_code_p = OP_CODE_SET_SINGLE;
            continue;
        }

        /* It's a range entry. */
        *compiled_op_code_p = OP_CODE_SET_RANGE;

        /* Skip the range operator. */
        self_p->pattern_p++;
        pattern_char = *self_p->pattern_p++;

        /* Handle the second character for this range entry. */
        if (pattern_char == '\0') {
            return (-1);
        } else if (pattern_char == '\\') {
            pattern_char = *self_p->pattern_p++;
            res = compile_escape_char(self_p, pattern_char);

            /* Do not allow special escape characters in a range. */
            if (res < 2) {
                return (-1);
            }

            code_size += res;
        } else if (pattern_char == '-') {
            return (-1);
        } else {
            *self_p->compiled_p++ = OP_CODE_TEXT;
            *self_p->compiled_p++ = pattern_char;
            self_p->compiled_left -= 2;
            code_size += 2;
        }
    }

    self_p->current_repetition_op_code_p[1] = ((code_size >> 8) & 0xff);
    self_p->current_repetition_op_code_p[2] = (code_size & 0xff);

    return (0);
}

static int compile_alternatives(struct compile_t *self_p)
{
    return (-1);
}

static int compile_group(struct compile_t *self_p)
{
    return (-1);
}

static int compile_return(struct compile_t *self_p)
{
    *self_p->compiled_p++ = OP_CODE_RETURN;

    return (0);
}

static int compile_text(struct compile_t *self_p)
{
    if (self_p->compiled_left < 1) {
        return (-1);
    }

    self_p->current_repetition_op_code_p = self_p->compiled_p;
    *self_p->compiled_p++ = OP_CODE_TEXT;
    *self_p->compiled_p++ = *self_p->pattern_p;
    self_p->compiled_left--;

    return (0);
}

static ssize_t match(struct match_t *self_p);

static ssize_t match_repetition(struct match_t *self_p,
                                int matched_size,
                                int greedy,
                                int repetitions_min,
                                int repetitions_max)
{
    int res, matched_size_repetition, code_size;
    int longest_matched_size_repetition = -1;
    struct match_t state;

    matched_size_repetition = 0;
    code_size = ((self_p->compiled_p[0] << 8) | self_p->compiled_p[1]);
    self_p->compiled_p += 2;

    while (1) {
        /* Initialize the match state. */
        state.compiled_p = self_p->compiled_p + code_size;
        state.flags = self_p->flags;
        state.buf_p = self_p->buf_p + matched_size_repetition;
        state.buf_left = self_p->buf_left - matched_size_repetition;
        state.groups_p = NULL;
        state.number_of_groups_p = NULL;

        res = match(&state);

        if (res >= 0) {
            longest_matched_size_repetition = matched_size_repetition;

            if (greedy == 0) {
                if ((repetitions_min == 0)
                    || (matched_size_repetition > 0)) {
                    break;
                }
            }
        }

        /* Match the repetition pattern. */
        state.compiled_p = self_p->compiled_p;
        state.flags = self_p->flags;
        state.buf_p = self_p->buf_p + matched_size_repetition;
        state.buf_left = self_p->buf_left - matched_size_repetition;

        res = match(&state);

        if (res < 0) {
            break;
        }

        matched_size_repetition += res;
    }

    if (longest_matched_size_repetition >= repetitions_min) {
        res = 0;

        while (res < longest_matched_size_repetition) {
            state = *self_p;
            state.buf_p += res;
            state.buf_left -= res;

            res += match(&state);
        }

        /* Match the rest of the pattern. */
        state = *self_p;
        state.compiled_p += code_size;
        state.buf_p += longest_matched_size_repetition;
        state.buf_left -= longest_matched_size_repetition;

        res += match(&state);

        return (matched_size + res);
    }

    return (-1);
}

static int match_begin(struct match_t *self_p)
{
    return (-1);
}

static int match_end(struct match_t *self_p)
{
    return (-1);
}

static int match_text(struct match_t *self_p)
{
    int value, reference;

    if (self_p->buf_left < 1) {
        return (-1);
    }

    value = (int)*self_p->buf_p;
    reference = *self_p->compiled_p;

    DLOG(DEBUG, "Comparing '%c' and '%c'.\r\n", value, reference);

    if (self_p->flags & RE_IGNORECASE) {
        value = tolower(value);
        reference = tolower(reference);
    }

    if (value != reference) {

        return (-1);
    }

    self_p->buf_p++;
    self_p->compiled_p++;
    self_p->buf_left--;

    return (1);
}

static int match_text_range(struct match_t *self_p,
                            int lower,
                            int upper)
{
    int value;

    if (self_p->buf_left < 1) {
        return (-1);
    }

    value = (int)*self_p->buf_p++;

    DLOG(DEBUG,
         "Comparing '%c' with range '%c' to '%c'.\r\n",
         value,
         (int)lower,
         (int)upper);

    if (self_p->flags & RE_IGNORECASE) {
        value = tolower(value);
        lower = tolower(lower);
        upper = tolower(upper);
    }

    if ((value < lower) || (value > upper)) {
        self_p->buf_p--;

        return (-1);
    }

    self_p->buf_left--;

    return (1);
}

static int match_dot(struct match_t *self_p)
{
    if (self_p->buf_left < 1) {
        return (-1);
    }

    if (!(self_p->flags & RE_DOTALL)) {
        if (*self_p->buf_p == '\n') {
            return (-1);
        }
    }

    self_p->buf_p++;
    self_p->buf_left--;

    return (1);
}

static int match_whitespace(struct match_t *self_p)
{
    if (self_p->buf_left < 1) {
        return (-1);
    }

    if (!isspace((int)*self_p->buf_p)) {
        return (-1);
    }

    self_p->buf_p++;
    self_p->buf_left--;

    return (1);
}

static int match_decimal_digit(struct match_t *self_p)
{
    if (self_p->buf_left < 1) {
        return (-1);
    }

    if (!isdigit((int)*self_p->buf_p)) {
        return (-1);
    }

    self_p->buf_p++;
    self_p->buf_left--;

    return (1);
}

static int match_alphanumeric(struct match_t *self_p)
{
    if (self_p->buf_left < 1) {
        return (-1);
    }

    if ((!isalnum((int)*self_p->buf_p)) && (*self_p->buf_p != '_')) {
        return (-1);
    }

    self_p->buf_p++;
    self_p->buf_left--;

    return (1);
}

static int match_zero_or_one(struct match_t *self_p,
                             int matched_size)
{
    int res;
    int code_size;
    struct match_t state;

    /* Greedy match - try one. */
    state = *self_p;
    state.compiled_p += 2;

    res = match(&state);

    if (res >= 0) {
        return (matched_size + res);
    }

    /* Try zero. */
    code_size = ((self_p->compiled_p[0] << 8) | self_p->compiled_p[1]);
    state = *self_p;
    state.compiled_p += (2 + code_size);

    res = match(&state);

    if (res >= 0) {
        return (matched_size + res);
    }

    return (res);
}

static int match_zero_or_more(struct match_t *self_p,
                              int matched_size)
{
    return (match_repetition(self_p, matched_size, 1, 0, -1));
}

static int match_one_or_more(struct match_t *self_p,
                             int matched_size)
{
    return (match_repetition(self_p, matched_size, 1, 1, -1));
}

static int match_zero_or_one_non_greedy(struct match_t *self_p,
                                        int matched_size)
{
    int res;
    int code_size;
    struct match_t state;

    /* Non-greedy match - try zero. */
    code_size = ((self_p->compiled_p[0] << 8)
                 | self_p->compiled_p[1]);
    self_p->compiled_p += 2;
    state = *self_p;
    state.compiled_p += code_size;

    res = match(&state);

    if (res >= 0) {
        return (matched_size + res);
    }

    /* Try one. */
    res = match(self_p);

    if (res >= 0) {
        return (matched_size + res);
    }

    return (-1);
}

static int match_zero_or_more_non_greedy(struct match_t *self_p,
                                         int matched_size)
{
    return (match_repetition(self_p, matched_size, 0, 0, -1));
}

static int match_one_or_more_non_greedy(struct match_t *self_p,
                                        int matched_size)
{
    return (match_repetition(self_p, matched_size, 0, 1, -1));
}

static int match_members(struct match_t *self_p)
{
    int res, number_of_members, i, code_size, matched_size;
    struct match_t state;

    code_size = ((self_p->compiled_p[0] << 8) | self_p->compiled_p[1]);
    number_of_members = ((self_p->compiled_p[2] << 8)
                         | self_p->compiled_p[3]);
    self_p->compiled_p += 4;
    matched_size = 0;

    for (i = 0; i < number_of_members; i++) {
        state = *self_p;
        res = match(&state);

        if (res < 0) {
            return (res);
        }

        self_p->buf_p += res;
        self_p->buf_left -= res;
        matched_size += res;
    }

    self_p->compiled_p += code_size;

    return (matched_size);
}

static int match_set(struct match_t *self_p)
{
    int res;
    char lower, upper;
    int code_size;
    const char *compiled_end_p;

    code_size = ((self_p->compiled_p[0] << 8) | self_p->compiled_p[1]);
    self_p->compiled_p += 2;
    compiled_end_p = (self_p->compiled_p + code_size);

    while (*self_p->compiled_p != OP_CODE_RETURN) {
        if (*self_p->compiled_p++ == OP_CODE_SET_SINGLE) {
            switch (*self_p->compiled_p++) {

            case OP_CODE_WHITESPACE:
                res = match_whitespace(self_p);
                break;

            case OP_CODE_DECIMAL_DIGIT:
                res = match_decimal_digit(self_p);
                break;

            case OP_CODE_ALPHANUMERIC:
                res = match_alphanumeric(self_p);
                break;

            case OP_CODE_TEXT:
                res = match_text(self_p);

                if (res < 0) {
                    self_p->compiled_p++;
                }
                break;

            default:
                return (-1);
            }
        } else {
            lower = self_p->compiled_p[1];
            upper = self_p->compiled_p[3];
            self_p->compiled_p += 4;

            res = match_text_range(self_p, lower, upper);
        }

        /* Return when a match is found. */
        if (res >= 0) {
            self_p->compiled_p = compiled_end_p;
            return (res);
        }
    }

    return (-1);
}

/**
 * Recursive match using given compiled pattern.
 *
 * @param[in] self_p The match state.
 *
 * @return Number of matched bytes or negative error code.
 */
static ssize_t match(struct match_t *self_p)
{
    int res;
    int op_code;
    size_t matched_size = 0;

    while (1) {
        op_code = *self_p->compiled_p++;

        DLOG(DEBUG,
             "op = %s (%d)\r\n",
             op_code_str[op_code],
             op_code);

        switch (op_code) {

        case OP_CODE_BEGIN:
            res = match_begin(self_p);
            break;

        case OP_CODE_END:
            res = match_end(self_p);
            break;

        case OP_CODE_TEXT:
            res = match_text(self_p);
            break;

        case OP_CODE_DOT:
            res = match_dot(self_p);
            break;

        case OP_CODE_WHITESPACE:
            res = match_whitespace(self_p);
            break;

        case OP_CODE_DECIMAL_DIGIT:
            res = match_decimal_digit(self_p);
            break;

        case OP_CODE_ALPHANUMERIC:
            res = match_alphanumeric(self_p);
            break;

        case OP_CODE_ZERO_OR_ONE:
            return (match_zero_or_one(self_p, matched_size));

        case OP_CODE_ZERO_OR_MORE:
            return (match_zero_or_more(self_p, matched_size));

        case OP_CODE_ONE_OR_MORE:
            return (match_one_or_more(self_p, matched_size));

        case OP_CODE_ZERO_OR_ONE_NON_GREEDY:
            return (match_zero_or_one_non_greedy(self_p, matched_size));

        case OP_CODE_ZERO_OR_MORE_NON_GREEDY:
            return (match_zero_or_more_non_greedy(self_p, matched_size));

        case OP_CODE_ONE_OR_MORE_NON_GREEDY:
            return (match_one_or_more_non_greedy(self_p, matched_size));

        case OP_CODE_MEMBERS:
            res = match_members(self_p);
            break;

        case OP_CODE_ALTERNATIVES:
            return (-1);

        case OP_CODE_SET:
            res = match_set(self_p);
            break;

        case OP_CODE_GROUP:
            return (-1);

        case OP_CODE_RETURN:
            return (matched_size);

        default:
            DLOG(ERROR, "Bad op code %d.\r\n", op_code);
            res = -1;
            break;
        }

        /* No match found. */
        if (res < 0) {
            return (res);
        }

        matched_size += res;
    }
}

int re_module_init()
{
    if (module.initialized == 1) {
        return (0);
    }

    module.initialized = 1;

#if CONFIG_RE_DEBUG_LOG_MASK > -1
    log_module_init();
    log_object_init(&module.log, "re", CONFIG_RE_DEBUG_LOG_MASK);
#endif

    return (0);
}

char *re_compile(char *compiled_p,
                 const char *pattern_p,
                 char flags,
                 size_t size)
{
    int res;
    struct compile_t state;

    /* Initialize the compile self_p-> */
    state.compiled_p = compiled_p;
    state.pattern_p = pattern_p;
    state.compiled_begin_p = compiled_p;
    state.compiled_left = size;
    state.current_repetition_op_code_p = NULL;

    *state.compiled_p++ = flags;
    state.compiled_left--;
    res = 0;

    while (res >= 0) {
        DLOG(DEBUG,
             "pattern = '%c', compiled_left = %d\r\n",
             *state.pattern_p,
             state.compiled_left);

        /* Check that the op code fits in the compilation buffer. */
        if (state.compiled_left < 1) {
            return (NULL);
        }

        /* Decrement once for the op code. */
        state.compiled_left--;

        switch (*state.pattern_p) {

        case '.':
            res = compile_dot(&state);
            break;

        case '^':
            res = compile_begin(&state);
            break;

        case '$':
            res = compile_end(&state);
            break;

        case '*':
            res = compile_repetition(&state, OP_CODE_ZERO_OR_MORE);
            break;

        case '+':
            res = compile_repetition(&state, OP_CODE_ONE_OR_MORE);
            break;

        case '?':
            res = compile_zero_or_one(&state);
            break;

        case '{':
            res = compile_members(&state);
            break;

        case '\\':
            res = compile_escape(&state);
            break;

        case '[':
            res = compile_set(&state);
            break;

        case '|':
            res = compile_alternatives(&state);
            break;

        case '(':
            res = compile_group(&state);
            break;

        case '\0':
            compile_return(&state);
            return (state.compiled_begin_p);

        default:
            res = compile_text(&state);
            break;
        }

        state.pattern_p++;
    }

    return (NULL);
}

ssize_t re_match(const char *compiled_p,
                 const char *buf_p,
                 size_t size,
                 struct re_group_t *groups_p,
                 size_t *number_of_groups_p)
{
    struct match_t state;

    /* Initialize the match state. */
    state.compiled_p = &compiled_p[1];
    state.flags = compiled_p[0];
    state.buf_p = buf_p;
    state.buf_left = size;
    state.groups_p = groups_p;
    state.number_of_groups_p = number_of_groups_p;

    return (match(&state));
}
