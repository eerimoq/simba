/**
 * @file can_db.h
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

#ifndef __CAN_DB_H__
#define __CAN_DB_H__

#include "simba.h"

/* Message IDs. */
#define CAN_DB_MOTOR_ID                            0x123
#define CAN_DB_THROTTLE_ID                         0x124

/* Message types. */
#define CAN_DB_MOTOR_TYPE           cantp_type_id_normal
#define CAN_DB_THROTTLE_TYPE        cantp_type_id_normal

/* RX mailboxes. */
#define CAN_DB_MOTOR_RX_MAILBOX                      0x1
#define CAN_DB_THROTTLE_RX_MAILBOX                   0x1

/* TX mailboxes. */
#define CAN_DB_MOTOR_TX_MAILBOX                      0x2
#define CAN_DB_THROTTLE_TX_MAILBOX                   0x2

/* Message data structures. */
struct can_db_motor_t {
    struct cantp_message_header_t header;
    uint16_t rpm;
};

struct can_db_throttle_t {
    struct cantp_message_header_t header;
    int32_t diff;
};

extern struct cantp_config_t cantp_config[];

#endif
