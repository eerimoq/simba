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

#ifndef __COLLECTIONS_LIST_H__
#define __COLLECTIONS_LIST_H__

#include "simba.h"

/**
 * Singly linked list elements must have this struct as their first
 * member.
 */
struct list_elem_t {
    struct list_elem_t *next_p;
};

/**
 * A singly linked list.
 */
struct list_t {
    struct list_elem_t *head_p;
    struct list_elem_t *tail_p;
};

/**
 * A singly linked list iterator.
 */
struct list_iter_t {
    struct list_elem_t *next_p;
};

/**
 * Initialize given singly linked list object.
 *
 * Elements in the list must have a `struct list_elem_t` as their
 * first struct member.
 *
 * An element can only be part of one list at a time.
 *
 * @param[in] list_p List object to initialize.
 *
 * @return zero(0) or negative error code.
 */
int list_init(struct list_t *self_p);

/**
 * Peek at the first element in the list.
 *
 * @param[in] list_p List object.
 *
 * @return First element of the list, or NULL if the list is empty.
 */
void *list_peek_head(struct list_t *self_p);

/**
 * Add given element to the beginning of given list.
 *
 * @param[in] list_p List object.
 * @param[in] elem_p Element to add.
 *
 * @return zero(0) or negative error code.
 */
int list_add_head(struct list_t *self_p,
                  void *elem_p);

/**
 * Add given element to the end of given list.
 *
 * @param[in] list_p List object.
 * @param[in] elem_p Element to add.
 *
 * @return zero(0) or negative error code.
 */
int list_add_tail(struct list_t *self_p,
                  void *elem_p);

/**
 * Get the first element of given list and then remove it from given
 * list.
 *
 * @param[in] list_p List object.
 *
 * @return Removed element, or NULL if the list was empty.
 */
void *list_remove_head(struct list_t *self_p);

/**
 * Remove given element from given list.
 *
 * @param[in] list_p List object.
 * @param[in] elem_p Element to remove.
 *
 * @return Removed element, or NULL if the element was not found.
 */
void *list_remove(struct list_t *self_p,
                  void *elem_p);

/**
 * Initialize given iterator object.
 *
 * @param[in] iterator_p Iterator to initialize.
 * @param[in] list_p List object to iterate over.
 *
 * @return zero(0) or negative error code.
 */
int list_iter_init(struct list_iter_t *self_p,
                   struct list_t *list_p);

/**
 * Get the next element from given iterator object.
 *
 * @param[in] iterator_p Iterator object.
 *
 * @return Next element, or NULL on end of list.
 */
void *list_iter_next(struct list_iter_t *self_p);

#endif
