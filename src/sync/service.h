/**
 * @file oam/service.h
 *
 * @section License
 * Copyright (C) 2014-2016, Erik Moqvist
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

#ifndef __OAM_SERVICE_H__
#define __OAM_SERVICE_H__

#include "simba.h"

struct service_t;
struct service_server_t;

typedef int (*service_callback_t)(struct service_t *self_p,
                                  struct service_server_t *server_p,
                                  void *chan_p);

struct service_t {
    const char *name_p;
    service_callback_t callback;
    void **channels_p;
};

/**
 * The service server wait for events to occur on a list of channels
 * and calls the service callback.
 */
struct service_server_t {
    const char *name_p;
    struct service_t *services_p;
    struct chan_list_t list;
    struct service_server_t *next_p;
};

/**
 * Initialize the service module. This function must be called before
 * calling any other function in this module.
 *
 * The module will only be initialized once even if this function is
 * called multiple times.
 *
 * @return zero(0) or negative error code.
 */
int service_module_init(void);

/**
 * Initialize a service server.
 */
int service_server_init(struct service_server_t *self_p,
                        const char *name_p,
                        struct service_t *services_p,
                        void *workspace_p,
                        size_t workspace_size);

/**
 * The service main function that listens for events on all registered
 * channels.
 *
 * @return Never returns.
 */
void *service_server_main(void *arg_p);

#endif
