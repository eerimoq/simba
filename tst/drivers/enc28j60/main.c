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

#define ETH_HEADER "\1\2\3\4\5\6"

#define IP_DST "\192\168\10\1"
#define IP_SRC "\192\168\10\2"
#define IP_HEADER IP_DST IP_SRC ""

#define ICMP_HEADER "\x08\x00\x00\x00\x00\x00\x00\x00"

static struct enc28j60_driver_t enc28j60drv;
static char ping[] = ETH_HEADER IP_HEADER ICMP_HEADER "12345678";
static char pong[sizeof(ping)];
static struct thrd_t *thrd_p;
static const uint8_t macaddr[] = "\1\2\3\4\5\6";

void on_interrupt(struct enc28j60_driver_t *drv_p,
                  uint8_t status)
{
    thrd_resume_i(thrd_p, 0);
}

int test_ping(struct harness_t *harness_p)
{
    thrd_p = thrd_self();

    /* Write ping packet to device. */
    enc28j60_write_begin(&enc28j60drv);
    enc28j60_write(&enc28j60drv, ping, sizeof(ping));
    enc28j60_write_end(&enc28j60drv);

    /* Wait for pong packet from device. */
    thrd_suspend(NULL);

    /* Read pong packet from device. */
    enc28j60_read_begin(&enc28j60drv);
    enc28j60_read(&enc28j60drv, pong, sizeof(pong));
    enc28j60_read_end(&enc28j60drv);

    return (0);
}

struct harness_testcase_t harness_testcases[] = {
    { test_ping, "test_ping" },
    { NULL, NULL }
};

int main()
{
    struct harness_t harness;
    struct spi_driver_t spi;
    struct exti_driver_t exti;

    sys_start();

    uart_module_init();
    spi_module_init();

    spi_init(&spi, &spi_device[0]);
    enc28j60_init(&enc28j60drv,
                  &spi,
                  &exti,
                  macaddr,
                  on_interrupt);
    enc28j60_start(&enc28j60drv);

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
