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
    struct service_t *services_p;
#if CONFIG_FS_CMD_SERVICE_LIST == 1
    struct fs_command_t cmd_list;
#endif
#if CONFIG_FS_CMD_SERVICE_START == 1
    struct fs_command_t cmd_start;
#endif
#if CONFIG_FS_CMD_SERVICE_STOP == 1
    struct fs_command_t cmd_stop;
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
    struct service_t *service_p;

    std_fprintf(chout_p, FSTR("NAME                    STATUS\r\n"));

    service_p = module.services_p;

    while (service_p != NULL) {
        std_fprintf(chout_p, FSTR("%-24s%s\r\n"),
                    service_p->name_p,
                    (service_p->status_cb(service_p) == service_status_running_t
                     ? "running"
                     : "stopped"));
        service_p = service_p->next_p;
    }
    
    return (0);
}

#endif

#if CONFIG_FS_CMD_SERVICE_START == 1

static int cmd_start_cb(int argc,
                        const char *argv[],
                        void *chout_p,
                        void *chin_p,
                        void *arg_p,
                        void *call_arg_p)
{
    struct service_t *service_p;
    const char *name_p;
    
    if (argc != 2) {
        std_fprintf(chout_p, FSTR("Usage: %s <service>\r\n"), argv[0]);
        return (-EINVAL);
    }

    name_p = argv[1];
    service_p = module.services_p;

    while (service_p != NULL) {
        if (strcmp(name_p, service_p->name_p) == 0) {
            return (service_start(service_p));
        }

        service_p = service_p->next_p;
    }
    
    std_fprintf(chout_p, FSTR("%s: bad service\r\n"), name_p);

    return (-1);
}

#endif

#if CONFIG_FS_CMD_SERVICE_STOP == 1

static int cmd_stop_cb(int argc,
                       const char *argv[],
                       void *chout_p,
                       void *chin_p,
                       void *arg_p,
                       void *call_arg_p)
{
    struct service_t *service_p;
    const char *name_p;
    
    if (argc != 2) {
        std_fprintf(chout_p, FSTR("Usage: %s <service>\r\n"), argv[0]);
        return (-EINVAL);
    }

    name_p = argv[1];
    service_p = module.services_p;

    while (service_p != NULL) {
        if (strcmp(name_p, service_p->name_p) == 0) {
            return (service_stop(service_p));
        }

        service_p = service_p->next_p;
    }
    
    std_fprintf(chout_p, FSTR("%s: bad service\r\n"), name_p);

    return (-1);
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
                    FSTR("/oam/service/list"),
                    cmd_list_cb,
                    NULL);
    fs_command_register(&module.cmd_list);
#endif

#if CONFIG_FS_CMD_SERVICE_START == 1
    fs_command_init(&module.cmd_start,
                    FSTR("/oam/service/start"),
                    cmd_start_cb,
                    NULL);
    fs_command_register(&module.cmd_start);
#endif

#if CONFIG_FS_CMD_SERVICE_STOP == 1
    fs_command_init(&module.cmd_stop,
                    FSTR("/oam/service/stop"),
                    cmd_stop_cb,
                    NULL);
    fs_command_register(&module.cmd_stop);
#endif

    return (0);
}

int service_init(struct service_t *self_p,
                 const char *name_p,
                 service_get_status_cb_t status_cb)
{
    ASSERTN(self_p != NULL, -EINVAL);
    ASSERTN(name_p != NULL, -EINVAL);

    if (event_init(&self_p->control) != 0) {
        return (-1);
    }

    self_p->name_p = name_p;
    self_p->status_cb = status_cb;
    
    return (0);
}

int service_start(struct service_t *self_p)
{
    uint32_t mask;

    mask = SERVICE_CONTROL_EVENT_START;
    event_write(&self_p->control, &mask, sizeof(mask));
    
    return (0);
}

int service_stop(struct service_t *self_p)
{
    uint32_t mask;

    mask = SERVICE_CONTROL_EVENT_STOP;
    event_write(&self_p->control, &mask, sizeof(mask));

    return (0);
}

int service_register(struct service_t *service_p)
{
    ASSERTN(service_p != NULL, -EINVAL);

    /* Add the service to the list of services. */
    service_p->next_p = module.services_p;
    module.services_p = service_p;

    return (0);
}

int service_deregister(struct service_t *service_p)
{
    ASSERTN(service_p != NULL, -EINVAL);
    
    return (-ENOSYS);
}
