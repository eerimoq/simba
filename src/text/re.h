/**
 * @section License
 *
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org/>
 *
 * This file is part of the Simba project.
 */

#ifndef __TEXT_RE_H__
#define __TEXT_RE_H__

#include "simba.h"

/**
 * Perform case-insensitive matching; expressions like ``[A-Z]`` will
 * match lowercase letters, too.
 */
#define RE_IGNORECASE 0x01

/**
 * Make the ``'.'`` special character match any character at all,
 * including a newline; without this flag, ``'.'`` will match anything
 * except a newline.
 */
#define RE_DOTALL     0x02

/**
 * When specified, the pattern character ``'^'`` matches at the
 * beginning of the string and at the beginning of each line
 * (immediately following each newline); and the pattern character
 * ``'$'`` matches at the end of the string and at the end of each
 * line (immediately preceding each newline). By default, '^' matches
 * only at the beginning of the string, and ``'$'`` only at the end of
 * the string and immediately before the newline (if any) at the end
 * of the string.
 */
#define RE_MULTILINE  0x04

struct re_group_t {
    const char *buf_p;
    ssize_t size;
};

/**
 * Compile given pattern.
 *
 * Pattern syntax:
 *
 * - ``'.'``   - Any character.
 * - ``'^'``   - Beginning of the string (**not yet supported**).
 * - ``'$'``   - End of the string (**not yet supported**).
 * - ``'?'``   - Zero or one repetitions (greedy).
 * - ``'*'``   - Zero or more repetitions (greedy).
 * - ``'+'``   - One or more repetitions (greedy).
 * - ``'??'``  - Zero or one repetitions (non-greedy).
 * - ``*?``    - Zero or more repetitions (non-greedy).
 * - ``+?``    - One or more repetitions (non-greedy).
 * - ``{m}``   - Exactly ``m`` repetitions.
 * - ``\\``    - Escape character.
 * - ``[]``    - Set of characters.
 * - ``'|'``   - Alternatives (**not yet supported**).
 * - ``(...)`` - Groups (**not yet supported**).
 * - ``\\d``   - Decimal digits ``[0-9]``.
 * - ``\\w``   - Alphanumerical characters ``[a-ZA-Z0-9_]``.
 * - ``\\s``   - Whitespace characters ``[ \t\r\n\f\v]``.
 *
 * @param[out] compiled_p Compiled regular expression pattern.
 * @param[in] pattern_p Regular expression pattern.
 * @param[in] flags A combination of the flags ``RE_IGNORECASE``,
 *                  ``RE_DOTALL`` and ``RE_MULTILINE``
 *                  (``RE_MULTILINE`` is **not yet supported**).
 * @param[in] size Size of the compiled buffer.
 *
 * @return Compiled patten, or NULL if the compilation failed.
 */
char *re_compile(char *compiled_p,
                 const char *pattern_p,
                 char flags,
                 size_t size);

/**
 * Apply given regular expression to the beginning of given string.
 *
 * @param[in] compiled_p Compiled regular expression pattern. Compile
 *                       a pattern with `re_compile()`.
 * @param[in] buf_p Buffer to apply the compiled pattern to.
 * @param[in] size Number of bytes in the buffer.
 * @param[out] groups_p Read groups or NULL.
 * @param[in,out] number_of_groups_p Number of read groups or NULL.
 *
 * @return Number of matched bytes or negative error code.
 */
ssize_t re_match(const char *compiled_p,
                 const char *buf_p,
                 size_t size,
                 struct re_group_t *groups_p,
                 size_t *number_of_groups_p);

#endif
