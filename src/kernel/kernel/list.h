/**
 * @file kernel/list.h
 * @version 0.1.0
 *
 * @section License
 * Copyright (C) 2014-2015, Erik Moqvist
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

#ifndef __KERNEL_LIST_H__
#define __KERNEL_LIST_H__

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

/* Initialize a singly linked list. */
#define LIST_SL_INIT(list_p)                    \
    do {                                        \
        (list_p)->head_p = NULL;                \
        (list_p)->tail_p = NULL;                \
    } while (0);

#define LIST_SL_INIT_STRUCT                     \
    { .head_p = NULL, .tail_p = NULL }

#define LIST_SL_PEEK_HEAD(list_p, element_pp)   \
    do {                                        \
        *(element_pp) = (list_p)->head_p;       \
    } while (0);

#define LIST_SL_ADD_HEAD(list_p, element_p)     \
    do {                                        \
        (element_p)->next_p = (list_p)->head_p; \
        (list_p)->head_p = element_p;           \
        if ((list_p)->tail_p == NULL) {         \
            (list_p)->tail_p = (element_p);     \
        }                                       \
    } while (0);

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

#define LIST_SL_ITERATOR_INIT(iterator_p, list_p)       \
    do {                                                \
        (iterator_p)->next_p = (list_p)->head_p;        \
    } while (0)

#define LIST_SL_ITERATOR_NEXT(iterator_p, element_pp)                   \
    do {                                                                \
        *(element_pp) = (iterator_p)->next_p;                           \
        if ((iterator_p)->next_p != NULL) {                             \
            (iterator_p)->next_p =                                      \
                ((struct list_sl_iterator_t *)(iterator_p)->next_p)->next_p; \
        }                                                               \
    } while (0)

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
