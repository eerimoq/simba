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

#ifndef __OAM_SERVICE_H__
#define __OAM_SERVICE_H__

#include "simba.h"

/**
 * Service start event.
 */
#define SERVICE_CONTROL_EVENT_START 0x1

/**
 * Serviece stop event.
 */
#define SERVICE_CONTROL_EVENT_STOP  0x2

enum service_status_t {
    service_status_running_t = 0,
    service_status_stopped_t = 1
};

struct service_t;

typedef enum service_status_t (*service_get_status_cb_t)(struct service_t *self_p);

/**
 * A service with name and control event channel.
 */
struct service_t {
    const char *name_p;
    struct event_t control;
    service_get_status_cb_t status_cb;
    struct service_t *next_p;
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
 * Initialize a service with given name and status callback.
 *
 * @param[in] self_p Service to initialize.
 * @param[in] name_p Name of the service.
 * @param[in] status_callback Callback function returning the service
 *                            status.
 *
 * @return zero(0) or negative error code.
 */
int service_init(struct service_t *self_p,
                 const char *name_p,
                 service_get_status_cb_t status_cb);

/**
 * Start given service.
 *
 * The event ``SERVICE_CONTROL_EVENT_START`` will be written to the
 * control channel of given service and it's up to the service to act
 * on this event. All services should act on all control events.
 *
 * @param[in] self_p Service to start.
 *
 * @return zero(0) or negative error code.
 */
int service_start(struct service_t *self_p);

/**
 * Stop given service.
 *
 * The event ``SERVICE_CONTROL_EVENT_STOP`` will be written to the
 * control channel of given service and it's up to the service to act
 * on this event. All services should act on all control events.
 *
 * @param[in] self_p Service to stop.
 *
 * @return zero(0) or negative error code.
 */
int service_stop(struct service_t *self_p);

/**
 * Register given service to the global list of services.
 *
 * @param[in] service_p Service to register.
 *
 * @return zero(0) or negative error code.
 */
int service_register(struct service_t *service_p);

/**
 * Deregister given service from the global list of services.
 *
 * @param[in] service_p Service to deregister.
 *
 * @return zero(0) or negative error code.
 */
int service_deregister(struct service_t *service_p);

#endif
