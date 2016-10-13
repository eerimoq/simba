/**
 * @file service.c
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

#include "simba.h"

struct module_t {
    int initialized;
    struct service_server_t *servers_p;
#if CONFIG_FS_CMD_SERVICE_LIST == 1
    struct fs_command_t cmd_list;
#endif
};

static struct module_t module;

#if CONFIG_FS_CMD_SERVICE_LIST == 1

static int cmd_list_cb(int argc,
                       const char *argv[],
                       void *chout_p,
                       void *chin_p,
                       void *arg_p,
                       void *call_arg_p)
{
    struct service_server_t *server_p;
    struct service_t *service_p;

    std_fprintf(chout_p, FSTR("NAME\r\n"));

    server_p = module.servers_p;

    while (server_p != NULL) {
        service_p = server_p->services_p;

        while (service_p->name_p != NULL) {
            std_fprintf(chout_p, FSTR("%s\r\n"), service_p->name_p);
            service_p++;
        }

        server_p = server_p->next_p;
    }

    return (0);
}

#endif

int service_module_init(void)
{
    /* Return immediately if the module is already initialized. */
    if (module.initialized == 1) {
        return (0);
    }

    module.initialized = 1;

#if CONFIG_FS_CMD_SERVICE_LIST == 1

    fs_command_init(&module.cmd_list,
                    FSTR("/sync/service/list"),
                    cmd_list_cb,
                    NULL);
    fs_command_register(&module.cmd_list);

#endif

    return (0);
}

int service_server_init(struct service_server_t *self_p,
                        struct service_t *services_p,
                        void *workspace_p,
                        size_t workspace_size)
{
    ASSERTN(self_p != NULL, -EINVAL);
    ASSERTN(services_p != NULL, -EINVAL);

    struct service_t *service_p;
    void **chan_pp;

    self_p->services_p = services_p;

    if (chan_list_init(&self_p->list, workspace_p, workspace_size) != 0) {
        return (-1);
    }

    /* Add all channels to the list of channels to poll. */
    service_p = services_p;

    while (service_p->name_p != NULL) {
        chan_pp = service_p->channels_p;

        while (*chan_pp != NULL) {
            if (chan_list_add(&self_p->list, *chan_pp) != 0) {
                return (-1);
            }

            chan_pp++;
        }

        service_p++;
    }

    /* Prepend the server to the list of servers. */
    self_p->next_p = module.servers_p;
    module.servers_p = self_p;

    return (0);
}

void *service_server_main(void *arg_p)
{
    struct service_server_t *self_p;
    struct service_t *service_p;
    void *chan_p;
    void **chan_pp;

    self_p = arg_p;

    while (1) {
        /* Poll for channel list events.*/
        chan_p = chan_list_poll(&self_p->list, NULL);

        /* Find the channel service and call the callback. */
        service_p = self_p->services_p;

        while (service_p->name_p != NULL) {
            chan_pp = service_p->channels_p;

            while (*chan_pp != NULL) {
                if (*chan_pp == chan_p) {
                    service_p->callback(service_p, self_p, chan_p);
                    break;
                }

                chan_pp++;
            }

            if (*chan_pp != NULL) {
                break;
            }

            service_p++;
        }
    }

    return (NULL);
}
