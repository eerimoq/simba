/**
 * @file main.c
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

#include "simba.h"
#include "can_db.h"

#define MOTOR_RPM         1720
#define THROTTLE_DIFF        5

union motor_control_message_t {
    struct cantp_message_header_t header;
    struct can_db_motor_t motor;
    struct can_db_throttle_t throttle;
};

struct loopback_t {
    struct canif_t base;
    int valid;
    struct thrd_t *thrd_p;
    struct canif_frame_t frame;
};

static struct loopback_t loopback;
static struct cantp_t cantp;

static int loopback_read(void *drv_p,
                         int mailbox,
                         struct canif_frame_t *frame_p)
{
    sys_lock();

    if (loopback.valid == 0) {
        loopback.thrd_p = thrd_self();
        thrd_suspend_irq(NULL);
        loopback.thrd_p = NULL;
    }

    *frame_p = loopback.frame;
    loopback.valid = 0;

    sys_unlock();

    return (0);
}

static int loopback_write(void *drv_p,
                          int mailbox,
                          const struct canif_frame_t *frame_p)
{
    ASSERT(loopback.valid == 0);

    loopback.frame = *frame_p;

    sys_lock();
    loopback.valid = 1;

    if (loopback.thrd_p != NULL) {
        thrd_resume_irq(loopback.thrd_p, 0);
    }

    sys_unlock();

    return (0);
}

static struct cantp_config_t *get_id_config(void *arg_p, uint32_t id)
{
    return (hash_map_get(arg_p, id));
}

static int id_config_hash(long key)
{
    return (0);
}

static union motor_control_message_t actuator_buf[1];
static union motor_control_message_t controller_buf[1];

int test_read_write(struct harness_t *harness_p)
{
    union motor_control_message_t message;
    int res;
    ssize_t size;
    struct cantp_chan_t actuator_chan;
    struct cantp_subscribe_t actuator_subscribe;
    struct cantp_chan_t controller_chan;
    struct cantp_subscribe_t controller_subscribe;

    /* Initialize actuator. */
    res = cantp_chan_init(&cantp, &actuator_chan, actuator_buf, sizeof(actuator_buf));
    BTASSERT(res == 0);
    actuator_subscribe.chan_p = &actuator_chan;
    res = cantp_subscribe(&cantp, CAN_DB_THROTTLE_ID, &actuator_subscribe);
    BTASSERT(res == 0);

    /* Initialize controller. */
    res = cantp_chan_init(&cantp, &controller_chan, controller_buf, sizeof(controller_buf));
    BTASSERT(res == 0);
    controller_subscribe.chan_p = &controller_chan;
    res = cantp_subscribe(&cantp, CAN_DB_MOTOR_ID, &controller_subscribe);
    BTASSERT(res == 0);

    /* Write message to controller. */
    message.header.id = CAN_DB_MOTOR_ID;
    message.motor.rpm = MOTOR_RPM;
    size = chan_write(&actuator_chan, &message, sizeof(message.motor));
    BTASSERT(size == sizeof(message.motor), "%lu %lu", size, sizeof(message.motor));

    /* Read message from actuator. */
    size = chan_read(&controller_chan, &message, sizeof(message.motor));
    BTASSERT(message.header.id == CAN_DB_MOTOR_ID);
    BTASSERT(size == sizeof(message.motor));
    std_printk(STD_LOG_NOTICE,
               FSTR("controller_main: motor(0x%x): rpm = %u"),
               (unsigned int)message.header.id,
               (unsigned int)message.motor.rpm);
    BTASSERT(message.motor.rpm == MOTOR_RPM);

    /* Write message to actuator. */
    message.header.id = CAN_DB_THROTTLE_ID;
    message.throttle.diff = THROTTLE_DIFF;
    size = chan_write(&controller_chan, &message, sizeof(message.throttle));
    BTASSERT(size == sizeof(message.throttle));

    /* Read message from controller. */
    size = chan_read(&actuator_chan, &message, sizeof(message.throttle));
    BTASSERT(message.header.id == CAN_DB_THROTTLE_ID);
    std_printk(STD_LOG_NOTICE,
               FSTR("actuator: throttle(0x%x): diff = %d"),
               (unsigned int)message.header.id,
               (int)message.throttle.diff);
    BTASSERT(message.throttle.diff == THROTTLE_DIFF);

    return (0);
}

int main()
{
    int i;
    struct hash_map_t id_config_map;
    struct hash_map_bucket_t buckets[8];
    struct hash_map_entry_t entries[8];
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_read_write, "test_read_write" },
        { NULL, NULL }
    };
    
    BTASSERT(hash_map_init(&id_config_map,
                           buckets,
                           membersof(buckets),
                           entries,
                           membersof(entries),
                           id_config_hash) == 0);

    i = 0;

    while (cantp_config[i].id != CANIF_ID_INVALID) {
        BTASSERT(hash_map_add(&id_config_map,
                              cantp_config[i].id,
                              &cantp_config[i]) == 0);
        i++;
    }

    sys_start();
    uart_module_init();

    loopback.base.read = loopback_read;
    loopback.base.write = loopback_write;
    loopback.valid = 0;
    loopback.thrd_p = NULL;
    BTASSERT(cantp_init(&cantp,
                        &loopback.base,
                        cantp_config,
                        get_id_config,
                        &id_config_map) == 0);

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
