/**
 * @file can_db.c
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

#include "can_db.h"

struct cantp_config_t cantp_config[] = {
    { CAN_DB_MOTOR_ID, CAN_DB_MOTOR_TYPE, CAN_DB_MOTOR_RX_MAILBOX, CAN_DB_MOTOR_TX_MAILBOX, NULL },
    { CAN_DB_THROTTLE_ID, CAN_DB_THROTTLE_TYPE, CAN_DB_THROTTLE_RX_MAILBOX, CAN_DB_THROTTLE_TX_MAILBOX, NULL },
    { CANIF_ID_INVALID, 0, 0, 0, NULL }
};
