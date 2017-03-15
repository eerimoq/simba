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

    std_fprintf(chout_p, OSTR("NAME                    STATUS\r\n"));

    service_p = module.services_p;

    while (service_p != NULL) {
        std_fprintf(chout_p, OSTR("%-24s%s\r\n"),
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
        std_fprintf(chout_p, OSTR("Usage: start <service>\r\n"));
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
    
    std_fprintf(chout_p, OSTR("%s: bad service\r\n"), name_p);

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
        std_fprintf(chout_p, OSTR("Usage: stop <service>\r\n"));
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
    
    std_fprintf(chout_p, OSTR("%s: bad service\r\n"), name_p);

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
                    CSTR("/oam/service/list"),
                    cmd_list_cb,
                    NULL);
    fs_command_register(&module.cmd_list);
#endif

#if CONFIG_FS_CMD_SERVICE_START == 1
    fs_command_init(&module.cmd_start,
                    CSTR("/oam/service/start"),
                    cmd_start_cb,
                    NULL);
    fs_command_register(&module.cmd_start);
#endif

#if CONFIG_FS_CMD_SERVICE_STOP == 1
    fs_command_init(&module.cmd_stop,
                    CSTR("/oam/service/stop"),
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
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(name_p != NULL, EINVAL);

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
    ASSERTN(service_p != NULL, EINVAL);

    /* Add the service to the list of services. */
    service_p->next_p = module.services_p;
    module.services_p = service_p;

    return (0);
}

int service_deregister(struct service_t *service_p)
{
    ASSERTN(service_p != NULL, EINVAL);
    
    return (-ENOSYS);
}
