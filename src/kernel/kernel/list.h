/**
 * @file kernel/list.h
 * @version 1.0
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
#define LIST_SL_INIT(l)                         \
    do {                                        \
        (l)->head_p = NULL;                     \
        (l)->tail_p = NULL;                     \
    } while (0);

#define LIST_SL_INIT_STRUCT                     \
    { .head_p = NULL, .tail_p = NULL }

#define LIST_SL_PEEK_HEAD(l, e)                 \
    do {                                        \
        *(e) = (l)->head_p;                     \
    } while (0);

#define LIST_SL_ADD_HEAD(l, e)                  \
    do {                                        \
        (e)->next_p = (l)->head_p;              \
        (l)->head_p = e;                        \
        if ((l)->tail_p == NULL) {              \
            (l)->tail_p = (e);                  \
        }                                       \
    } while (0);

#define LIST_SL_ADD_TAIL(l, e)                                  \
    do {                                                        \
        (e)->next_p = NULL;                                     \
        if ((l)->tail_p != NULL) {                              \
            ((struct list_next_t *)((l)->tail_p))->next_p = e;  \
        } else if ((l)->head_p == NULL) {                       \
            (l)->head_p = (e);                                  \
        }                                                       \
        (l)->tail_p = e;                                        \
    } while (0);

#define LIST_SL_REMOVE_HEAD(l, e)                                       \
    do {                                                                \
        *(e) = (l)->head_p;                                             \
        if (*(e) != NULL) {                                             \
                           (l)->head_p = (*(e))->next_p;                \
                           (*(e))->next_p = NULL;                       \
                           if ((l)->tail_p == *(e)) {                   \
                                                     (l)->tail_p = NULL; \
                                                     }                  \
                           }                                            \
        } while (0)

#define LIST_SL_ITERATOR_INIT(i, l)             \
    do {                                        \
        (i)->next_p = (l)->head_p;              \
    } while (0)

#define LIST_SL_ITERATOR_NEXT(i, e)                                     \
    do {                                                                \
        *(e) = (i)->next_p;                                             \
        if ((i)->next_p != NULL) {                                      \
            (i)->next_p = ((struct list_sl_iterator_t *)(i)->next_p)->next_p; \
        }                                                               \
    } while (0)

#endif
