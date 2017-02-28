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

#ifndef __COLLECTIONS_LIST_H__
#define __COLLECTIONS_LIST_H__

#include "simba.h"

struct list_next_t {
    void *next_p;
};

struct list_singly_linked_t {
    void *head_p;
    void *tail_p;
};

struct list_sl_iterator_t {
    void *next_p;
};

/**
 * Initialize given singly linked list object.
 *
 * @param[in] list_p List object to initialize.
 */
#define LIST_SL_INIT(list_p)                    \
    do {                                        \
        (list_p)->head_p = NULL;                \
        (list_p)->tail_p = NULL;                \
    } while (0);

#define LIST_SL_INIT_STRUCT                     \
    { .head_p = NULL, .tail_p = NULL }

/**
 * Peek at the first element in the list.
 *
 * @param[in] list_p List object.
 * @param[out] element_pp First element of the list.
 */
#define LIST_SL_PEEK_HEAD(list_p, element_pp)   \
    do {                                        \
        *(element_pp) = (list_p)->head_p;       \
    } while (0);

/**
 * Add given element to the beginning of given list.
 *
 * @param[in] list_p List object.
 * @param[in] element_p Element to add.
 */
#define LIST_SL_ADD_HEAD(list_p, element_p)     \
    do {                                        \
        (element_p)->next_p = (list_p)->head_p; \
        (list_p)->head_p = element_p;           \
        if ((list_p)->tail_p == NULL) {         \
            (list_p)->tail_p = (element_p);     \
        }                                       \
    } while (0);

/**
 * Add given element to the end of given list.
 *
 * @param[in] list_p List object.
 * @param[in] element_p Element to add.
 */
#define LIST_SL_ADD_TAIL(list_p, element_p)                             \
    do {                                                                \
        (element_p)->next_p = NULL;                                     \
        if ((list_p)->tail_p != NULL) {                                 \
            ((struct list_next_t *)((list_p)->tail_p))->next_p = element_p; \
        } else if ((list_p)->head_p == NULL) {                          \
            (list_p)->head_p = (element_p);                             \
        }                                                               \
        (list_p)->tail_p = element_p;                                   \
    } while (0);

/**
 * Get the first element of given list and then remove it from given
 * list.
 *
 * @param[in] list_p List object.
 * @param[out] element_pp First element of the list.
 */
#define LIST_SL_REMOVE_HEAD(list_p, element_pp)         \
    do {                                                \
        *(element_pp) = (list_p)->head_p;               \
        if (*(element_pp) != NULL) {                    \
            (list_p)->head_p = (*(element_pp))->next_p; \
            (*(element_pp))->next_p = NULL;             \
            if ((list_p)->tail_p == *(element_pp)) {    \
                (list_p)->tail_p = NULL;                \
            }                                           \
        }                                               \
    } while (0)

/**
 * Initialize given iterator object.
 *
 * @param[in] iterator_p Iterator to initialize.
 * @param[in] list_p List object to iterate over.
 */
#define LIST_SL_ITERATOR_INIT(iterator_p, list_p)       \
    do {                                                \
        (iterator_p)->next_p = (list_p)->head_p;        \
    } while (0)

/**
 * Get the next element from given iterator object.
 *
 * @param[in] iterator_p Iterator object.
 * @param[out] element_pp Next element of the list.
 */
#define LIST_SL_ITERATOR_NEXT(iterator_p, element_pp)                   \
    do {                                                                \
        *(element_pp) = (iterator_p)->next_p;                           \
        if ((iterator_p)->next_p != NULL) {                             \
            (iterator_p)->next_p =                                      \
                ((struct list_sl_iterator_t *)(iterator_p)->next_p)->next_p; \
        }                                                               \
    } while (0)

/**
 * Remove given element from given list.
 *
 * @param[in] list_p List object.
 * @param[in] iterator_p Used internally.
 * @param[in] element_p Used internally.
 * @param[in] iterator_element_p Used internally.
 * @param[in] previous_element_p Used internally.
 */
#define LIST_SL_REMOVE_ELEM(list_p, iterator_p, element_p, iterator_element_p, previous_element_p) \
    LIST_SL_ITERATOR_INIT((iterator_p), (list_p));                      \
    (previous_element_p) = NULL;                                        \
    while (1) {                                                         \
        LIST_SL_ITERATOR_NEXT((iterator_p), &(iterator_element_p));     \
        if ((iterator_element_p) == NULL) {                             \
            /* Element not found.*/                                     \
            break;                                                      \
        } else if ((iterator_element_p) == (element_p)) {               \
            /* Element found. Remove it. */                             \
            if ((previous_element_p) != NULL) {                         \
                (previous_element_p)->next_p = (element_p)->next_p;     \
            } else {                                                    \
                (list_p)->head_p = NULL;                                \
            }                                                           \
            if ((element_p)->next_p == NULL) {                          \
                (list_p)->tail_p = NULL;                                \
            }                                                           \
            break;                                                      \
        }                                                               \
        (previous_element_p) = (iterator_element_p);                    \
    }

#endif
