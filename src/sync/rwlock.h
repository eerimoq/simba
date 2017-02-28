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

#ifndef __SYNC_RWLOCK_H__
#define __SYNC_RWLOCK_H__

#include "simba.h"

struct rwlock_t {
    int number_of_readers;
    int number_of_writers;
    volatile struct rwlock_elem_t *readers_p;
    volatile struct rwlock_elem_t *writers_p;
};

/**
 * Initialize the reader-writer lock module. This function must be
 * called before calling any other function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
 *
 * @return zero(0) or negative error code
 */
int rwlock_module_init(void);

/**
 * Initialize given reader-writer lock object.
 *
 * @param[in] self_p Reader-writer lock to initialize.
 *
 * @return zero(0) or negative error code.
 */
int rwlock_init(struct rwlock_t *self_p);

/**
 * Take given reader-writer lock. Multiple threads can have the reader
 * lock at the same time.
 *
 * @param[in] self_p Reader-writer lock to take.
 *
 * @return zero(0) or negative error code.
 */
int rwlock_reader_take(struct rwlock_t *self_p);

/**
 * Give given reader-writer lock.
 *
 * @param[in] self_p Reader-writer lock give.
 *
 * @return zero(0) or negative error code.
 */
int rwlock_reader_give(struct rwlock_t *self_p);

/**
 * Give given reader-writer lock from isr or with the system lock
 * taken.
 *
 * @param[in] self_p Reader-writer lock to give.
 *
 * @return zero(0) or negative error code.
 */
int rwlock_reader_give_isr(struct rwlock_t *self_p);

/**
 * Take given reader-writer lock as a writer. Only one thread can have
 * the lock at a time, including both readers and writers.
 *
 * @param[in] self_p Reader-writer lock to take.
 *
 * @return zero(0) or negative error code.
 */
int rwlock_writer_take(struct rwlock_t *self_p);

/**
 * Give given reader-writer lock.
 *
 * @param[in] self_p Reader-writer lock to give.
 *
 * @return zero(0) or negative error code.
 */
int rwlock_writer_give(struct rwlock_t *self_p);

/**
 * Give given reader-writer lock from isr or with the system lock
 * taken.
 *
 * @param[in] self_p Reader-writer lock to give.
 *
 * @return zero(0) or negative error code.
 */
int rwlock_writer_give_isr(struct rwlock_t *self_p);

#endif
