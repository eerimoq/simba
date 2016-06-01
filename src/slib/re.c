/**
 * @file re.c
 * @version 0.5.0
 *
 * @section License
 * Copyright (C) 2016, Erik Moqvist
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
 * pattern = "[ac-d]"
 * code = [
 *     OP_CODE_SET, 5,
 *     OP_CODE_SET_SINGLE, 'a',
 *     OP_CODE_SET_RANGE, 'c', 'd',
 *     OP_CODE_RETURN
 * ]
 *
 * pattern = "[ac-d]{3}"
 * code = [
 *     OP_CODE_MEMBERS, 0, 7, 0, 3,
 *     OP_CODE_SET, 5,
 *     OP_CODE_SET_SINGLE, 'a',
 *     OP_CODE_SET_RANGE, 'c', 'd',
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

//#define RE_DEBUG

#if CONFIG_DEBUG == 1 && defined(RE_DEBUG)

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

#endif

#ifdef RE_DEBUG
#    define STD_PRINTF_RE_DEBUG(...) STD_PRINTF_DEBUG(__VA_ARGS__)
#else
#    define STD_PRINTF_RE_DEBUG(...)
#endif

#define COMPILE_REPETITION(op_code_base)                                \
    {                                                                   \
        int op_code = op_code_base;                                     \
                                                                        \
        if (pattern_p[1] == '?') {                                      \
            op_code += NON_GREEDY_OFFSET;                               \
            pattern_p++;                                                \
        }                                                               \
                                                                        \
        if (compiled_left < 3) {                                        \
            return (NULL);                                              \
        }                                                               \
                                                                        \
        /* Insert before last op, group or set. */                      \
        code_size = (compiled_p - current_repetition_op_code_p);        \
        memmove(&current_repetition_op_code_p[3],                       \
                current_repetition_op_code_p,                           \
                code_size);                                             \
        code_size++;                                                    \
        current_repetition_op_code_p[0] = op_code;                      \
        current_repetition_op_code_p[1] = ((code_size >> 8) & 0xff);    \
        current_repetition_op_code_p[2] = (code_size & 0xff);           \
        compiled_p[3] = OP_CODE_RETURN;                                 \
        compiled_p += 4;                                                \
        pattern_p++;                                                    \
        compiled_left -= 3;                                             \
    }

static ssize_t match(const char *compiled_p,
                     char flags,
                     const char *buf_p,
                     size_t size,
                     struct re_group_t *groups_p,
                     size_t *number_of_groups_p);

static ssize_t match_repetition(const char *compiled_p,
                                char flags,
                                const char *buf_p,
                                size_t buf_left,
                                struct re_group_t *groups_p,
                                size_t *number_of_groups_p,
                                ssize_t matched_size,
                                int greedy,
                                int repetitions_min,
                                int repetitions_max)
{
    int res, matched_size_repetition, code_size;
    int longest_matched_size_repetition = -1;

    matched_size_repetition = 0;
    code_size = ((compiled_p[0] << 8) | compiled_p[1]);
    compiled_p += 2;

    while (1) {
        /* Match the rest of the pattern. */
        res = match(compiled_p + code_size,
                    flags,
                    buf_p + matched_size_repetition,
                    buf_left - matched_size_repetition,
                    NULL,
                    NULL);

        if (res >= 0) {
            longest_matched_size_repetition = matched_size_repetition;

            if (greedy == 0) {
                if ((repetitions_min == 0) || (matched_size_repetition > 0)) {
                    break;
                }
            }
        }

        /* Match the repetition pattern. */
        res = match(compiled_p,
                    flags,
                    buf_p + matched_size_repetition,
                    buf_left - matched_size_repetition,
                    NULL,
                    NULL);

        if (res < 0) {
            break;
        }

        matched_size_repetition += res;
    }

    if (longest_matched_size_repetition >= repetitions_min) {
        res = 0;

        while (res < longest_matched_size_repetition) {
            res += match(compiled_p,
                         flags,
                         buf_p + res,
                         buf_left - res,
                         groups_p,
                         number_of_groups_p);
        }

        res += match(compiled_p + code_size,
                     flags,
                     buf_p + longest_matched_size_repetition,
                     buf_left - longest_matched_size_repetition,
                     groups_p,
                     number_of_groups_p);

        return (matched_size + res);
    }

    return (-1);
}

/**
 * Recursive match using given compiled pattern.
 *
 * @param[in] compiled_p Compiled regular expression.
 * @param[in] buf_p Buffer to apply the pattern to.
 * @param[in] size Number of bytes in buffer.
 * @param[out] groups_p Read groups.
 * @param[in,out] number_of_groups Number of read groups.
 *
 * @return Number of matched bytes or negative error code.
 */
static ssize_t match(const char *compiled_p,
                     char flags,
                     const char *buf_p,
                     size_t size,
                     struct re_group_t *groups_p,
                     size_t *number_of_groups_p)
{
    int op_code;
    size_t matched_size = 0;
    size_t buf_left = size;
    size_t code_size;

    while (1) {
        op_code = *compiled_p++;

        STD_PRINTF_RE_DEBUG(FSTR("op = %s (%d)\r\n"),
                            op_code_str[op_code],
                            op_code);

        switch (op_code) {

        case OP_CODE_BEGIN:
            break;

        case OP_CODE_END:
            break;

        case OP_CODE_TEXT:
            if (buf_left < 1) {
                return (-1);
            }

            STD_PRINTF_RE_DEBUG(FSTR("Comparing '%c' and '%c'.\r\n"),
                                (int)*buf_p,
                                (int)*compiled_p);

            if (flags & RE_IGNORECASE) {
                if (tolower((int)*buf_p++) != tolower((int)*compiled_p++)) {
                    return (-1);
                }
            } else {
                if (*buf_p++ != *compiled_p++) {
                    return (-1);
                }
            }

            buf_left--;
            matched_size++;
            break;

        case OP_CODE_DOT:
            if (buf_left < 1) {
                return (-1);
            }

            if (!(flags & RE_DOTALL)) {
                if (*buf_p == '\n') {
                    return (-1);
                }
            }

            buf_p++;
            buf_left--;
            matched_size++;
            break;

        case OP_CODE_WHITESPACE:
            if (buf_left < 1) {
                return (-1);
            }

            if (!isspace((int)*buf_p)) {
                return (-1);
            }

            buf_p++;
            buf_left--;
            matched_size++;
            break;

        case OP_CODE_DECIMAL_DIGIT:
            if (buf_left < 1) {
                return (-1);
            }

            if (!isdigit((int)*buf_p)) {
                return (-1);
            }

            buf_p++;
            buf_left--;
            matched_size++;
            break;

        case OP_CODE_ALPHANUMERIC:
            if (buf_left < 1) {
                return (-1);
            }

            if ((!isalnum((int)*buf_p)) && (*buf_p != '_')) {
                return (-1);
            }

            buf_p++;
            buf_left--;
            matched_size++;
            break;

        case OP_CODE_ZERO_OR_ONE:
            {
                int res;

                /* Greedy match - try one. */
                res = match(compiled_p + 2,
                            flags,
                            buf_p,
                            buf_left,
                            groups_p,
                            number_of_groups_p);

                if (res >= 0) {
                    return (matched_size + res);
                }

                /* Try zero. */
                code_size = ((compiled_p[0] << 8) | compiled_p[1]);
                res = match(compiled_p + 2 + code_size,
                            flags,
                            buf_p,
                            buf_left,
                            groups_p,
                            number_of_groups_p);

                if (res >= 0) {
                    return (matched_size + res);
                }

                return (-1);
            }

        case OP_CODE_ZERO_OR_MORE:
            return (match_repetition(compiled_p,
                                     flags,
                                     buf_p,
                                     buf_left,
                                     groups_p,
                                     number_of_groups_p,
                                     matched_size,
                                     1,
                                     0,
                                     -1));

        case OP_CODE_ONE_OR_MORE:
            return (match_repetition(compiled_p,
                                     flags,
                                     buf_p,
                                     buf_left,
                                     groups_p,
                                     number_of_groups_p,
                                     matched_size,
                                     1,
                                     1,
                                     -1));

        case OP_CODE_ZERO_OR_ONE_NON_GREEDY:
            {
                int res;

                /* Non-greedy match - try zero. */
                code_size = ((compiled_p[0] << 8) | compiled_p[1]);
                compiled_p += 2;
                res = match(compiled_p + code_size,
                            flags,
                            buf_p,
                            buf_left,
                            groups_p,
                            number_of_groups_p);

                if (res >= 0) {
                    return (matched_size + res);
                }

                /* Try one. */
                res = match(compiled_p,
                            flags,
                            buf_p,
                            buf_left,
                            groups_p,
                            number_of_groups_p);

                if (res >= 0) {
                    return (matched_size + res);
                }

                return (-1);
            }

        case OP_CODE_ZERO_OR_MORE_NON_GREEDY:
            return (match_repetition(compiled_p,
                                     flags,
                                     buf_p,
                                     buf_left,
                                     groups_p,
                                     number_of_groups_p,
                                     matched_size,
                                     0,
                                     0,
                                     -1));

        case OP_CODE_ONE_OR_MORE_NON_GREEDY:
            return (match_repetition(compiled_p,
                                     flags,
                                     buf_p,
                                     buf_left,
                                     groups_p,
                                     number_of_groups_p,
                                     matched_size,
                                     0,
                                     1,
                                     -1));

        case OP_CODE_MEMBERS:
            {
                int res, number_of_members, i;

                code_size = ((compiled_p[0] << 8) | compiled_p[1]);
                number_of_members = ((compiled_p[2] << 8) | compiled_p[3]);
                compiled_p += 4;

                for (i = 0; i < number_of_members; i++) {
                    res = match(compiled_p,
                                flags,
                                buf_p,
                                buf_left,
                                groups_p,
                                number_of_groups_p);

                    if (res < 0) {
                        return (-1);
                    }

                    buf_p += res;
                    buf_left -= res;
                    matched_size += res;
                }

                compiled_p += code_size;
            }
            break;

        case OP_CODE_ALTERNATIVES:
            return (-1);

        case OP_CODE_SET:
            return (-1);

        case OP_CODE_GROUP:
            return (-1);

        case OP_CODE_RETURN:
            return (matched_size);

        default:
            return (-1);

        }
    }
}

char *re_compile(char *compiled_p,
                 const char *pattern_p,
                 char flags,
                 size_t size)
{
    char *compiled_begin_p = compiled_p;
    size_t compiled_left = size;
    char *current_repetition_op_code_p = NULL;
    size_t code_size;

    *compiled_p++ = flags;
    compiled_left--;

    while (1) {
        STD_PRINTF_RE_DEBUG(FSTR("pattern = '%c', compiled_left = %d\r\n"),
                            *pattern_p,
                            compiled_left);

        if (compiled_left < 1) {
            return (NULL);
        }

        compiled_left--;

        switch (*pattern_p) {

        case '.':
            current_repetition_op_code_p = compiled_p;
            *compiled_p++ = OP_CODE_DOT;
            pattern_p++;
            break;

        case '^':
            *compiled_p++ = OP_CODE_BEGIN;
            pattern_p++;
            return (NULL);

        case '$':
            *compiled_p++ = OP_CODE_END;
            pattern_p++;
            return (NULL);

        case '*':
            COMPILE_REPETITION(OP_CODE_ZERO_OR_MORE);
            break;

        case '+':
            COMPILE_REPETITION(OP_CODE_ONE_OR_MORE);
            break;

        case '?':
            {
                int op_code = OP_CODE_ZERO_OR_ONE;

                if (compiled_left < 2) {
                    return (NULL);
                }

                if (pattern_p[1] == '?') {
                    op_code += NON_GREEDY_OFFSET;
                    pattern_p++;
                }

                /* Insert before last op, group or set. */
                code_size = (compiled_p - current_repetition_op_code_p);
                memmove(&current_repetition_op_code_p[3],
                        current_repetition_op_code_p,
                        code_size);
                current_repetition_op_code_p[0] = op_code;
                current_repetition_op_code_p[1] = ((code_size >> 8) & 0xff);
                current_repetition_op_code_p[2] = (code_size & 0xff);
                compiled_p += 3;
                compiled_left -= 2;
                pattern_p++;
            }
            break;

        case '{':
            if (compiled_left < 5) {
                return (NULL);
            }

            /* Insert before last op, group or set. */
            int number_of_members = 0;

            pattern_p++;

            while (isdigit((int)*pattern_p)) {
                number_of_members *= 10;
                number_of_members += (*pattern_p++ - '0');
            }

            pattern_p++;

            code_size = (compiled_p - current_repetition_op_code_p);
            memmove(&current_repetition_op_code_p[5],
                    current_repetition_op_code_p,
                    code_size);
            code_size++;
            current_repetition_op_code_p[0] = OP_CODE_MEMBERS;
            current_repetition_op_code_p[1] = ((code_size >> 8) & 0xff);
            current_repetition_op_code_p[2] = (code_size & 0xff);
            current_repetition_op_code_p[3] = ((number_of_members >> 8) & 0xff);
            current_repetition_op_code_p[4] = (number_of_members & 0xff);
            compiled_p[5] = OP_CODE_RETURN;
            compiled_p += 6;
            compiled_left -= 5;
            break;

        case '\\':
            {
                pattern_p++;
                current_repetition_op_code_p = compiled_p;

                switch (*pattern_p) {

                case '\0':
                    return (NULL);

                case 's':
                    *compiled_p++ = OP_CODE_WHITESPACE;
                    break;

                case 'd':
                    *compiled_p++ = OP_CODE_DECIMAL_DIGIT;
                    break;

                case 'w':
                    *compiled_p++ = OP_CODE_ALPHANUMERIC;
                    break;

                default:
                    /* Text. */
                    if (compiled_left < 1) {
                        return (NULL);
                    }

                    *compiled_p++ = OP_CODE_TEXT;
                    *compiled_p++ = *pattern_p;
                    compiled_left--;
                    break;
                }

                pattern_p++;
            }
            break;

        case '[':
            return (NULL);

        case '|':
            /* Insert at the beginning of current group. */
            return (NULL);

        case '(':
            return (NULL);

        case '\0':
            *compiled_p++ = OP_CODE_RETURN;
            return (compiled_begin_p);

        default:
            /* Text. */
            if (compiled_left < 1) {
                return (NULL);
            }

            current_repetition_op_code_p = compiled_p;
            *compiled_p++ = OP_CODE_TEXT;
            *compiled_p++ = *pattern_p++;
            compiled_left--;
            break;
        }
    }

    return (NULL);
}

ssize_t re_match(const char *compiled_p,
                 const char *buf_p,
                 size_t size,
                 struct re_group_t *groups_p,
                 size_t *number_of_groups_p)
{
    return (match(&compiled_p[1],
                  compiled_p[0],
                  buf_p,
                  size,
                  groups_p,
                  number_of_groups_p));
}
