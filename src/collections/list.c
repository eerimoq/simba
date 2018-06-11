/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2018, Erik Moqvist
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

#include "simba.h"

int list_init(struct list_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    self_p->head_p = NULL;
    self_p->tail_p = NULL;

    return (0);
}

void *list_peek_head(struct list_t *self_p)
{
    return (self_p->head_p);
}

int list_add_head(struct list_t *self_p,
                  void *v_elem_p)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(v_elem_p != NULL, EINVAL);

    struct list_elem_t *elem_p;

    elem_p = v_elem_p;

    elem_p->next_p = self_p->head_p;
    self_p->head_p = elem_p;

    if (self_p->tail_p == NULL) {
        self_p->tail_p = elem_p;
    }

    return (0);
}

int list_add_tail(struct list_t *self_p,
                  void *v_elem_p)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(v_elem_p != NULL, EINVAL);

    struct list_elem_t *elem_p;

    elem_p = v_elem_p;
    elem_p->next_p = NULL;

    if (self_p->tail_p != NULL) {
        self_p->tail_p->next_p = elem_p;
    } else if (self_p->head_p == NULL) {
        self_p->head_p = elem_p;
    }

    self_p->tail_p = elem_p;

    return (0);
}

void *list_remove_head(struct list_t *self_p)
{
    ASSERTNRN(self_p != NULL, EINVAL);

    struct list_elem_t *elem_p;

    elem_p = self_p->head_p;

    if (elem_p != NULL) {
        self_p->head_p = elem_p->next_p;
        elem_p->next_p = NULL;

        if (self_p->tail_p == elem_p) {
            self_p->tail_p = NULL;
        }
    }

    return (elem_p);
}

int list_iter_init(struct list_iter_t *self_p,
                   struct list_t *list_p)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(list_p != NULL, EINVAL);

    self_p->next_p = list_p->head_p;

    return (0);
}

void *list_iter_next(struct list_iter_t *self_p)
{
    ASSERTNRN(self_p != NULL, EINVAL);

    struct list_elem_t *elem_p;

    elem_p = self_p->next_p;

    if (elem_p != NULL) {
        self_p->next_p = elem_p->next_p;
    }

    return (elem_p);
}

void *list_remove(struct list_t *self_p,
                  void *v_elem_p)
{
    ASSERTNRN(self_p != NULL, EINVAL);
    ASSERTNRN(v_elem_p != NULL, EINVAL);

    struct list_iter_t iter;
    struct list_elem_t *elem_p;
    struct list_elem_t *next_p;
    struct list_elem_t *prev_p;

    list_iter_init(&iter, self_p);

    elem_p = v_elem_p;
    prev_p = NULL;

    do {
        next_p = list_iter_next(&iter);

        /* Requested element? */
        if (next_p == elem_p) {
            if (prev_p != NULL) {
                prev_p->next_p = elem_p->next_p;
            } else {
                self_p->head_p = elem_p->next_p;
            }

            if (elem_p == self_p->tail_p) {
                self_p->tail_p = prev_p;
            }

            return (elem_p);
        }

        prev_p = next_p;
    } while (next_p != NULL);

    return (NULL);
}
