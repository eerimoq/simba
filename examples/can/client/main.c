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

FS_COMMAND_DEFINE("/can_frame_read", can_frame_read);
FS_COMMAND_DEFINE("/can_frame_write", can_frame_write);

struct queue_t frame_input_queue;
static char frames[128];

static char qinbuf[32];
static struct uart_driver_t uart;
static struct shell_args_t shell_args;

struct mcp2515_driver_t mcp2515;

int can_frame_read(int argc,
                   const char *argv[],
                   void *out_p,
                   void *in_p)
{
    struct mcp2515_frame_t frame;
    const char *space_p;
    int i;

    if (mcp2515_read(&mcp2515, &frame) != sizeof(frame)) {
        std_printf(FSTR("failed to read frame\r\n"));
        return (-1);
    }

    std_fprintf(out_p,
                FSTR("id = 0x%lx, length = %d, data = '"),
                frame.id,
                frame.size);

    for (i = 0, space_p = ""; i < frame.size; i++, space_p = " ") {
        std_fprintf(out_p, FSTR("%s0x%02x"),
                    space_p,
                    frame.data[i]);
    }

    std_fprintf(out_p, FSTR("'\r\n"));

    return (0);
}

int can_frame_write(int argc,
                    const char *argv[],
                    void *out_p,
                    void *in_p)
{
    struct mcp2515_frame_t frame;
    long id;
    long dlc;
    long value;
    int i;

    if ((argc < 3) || (argc > 11)) {
        std_fprintf(out_p,
                    FSTR("Usage: %s <id> <data length> [<space separated data bytes>]\r\n"),
            argv[0]);

        return (-EINVAL);
    }

    if (std_strtol(argv[1], &id) != 0) {
        std_fprintf(out_p, FSTR("%s: unable to parse integer\r\n"), argv[1]);

        return (-EINVAL);
    }

    if (std_strtol(argv[2], &dlc) != 0) {
        std_fprintf(out_p, FSTR("%s: unable to parse integer\r\n"), argv[2]);

        return (-EINVAL);
    }

    if ((dlc < 0) || (dlc > 8)) {
        std_fprintf(out_p, FSTR("%d: data length must be in the range 0 to 8\r\n"), dlc);

        return (-EINVAL);
    }

    if (argc != (3 + dlc)) {
        std_fprintf(out_p, FSTR("wrong number of data bytes\r\n"));

        return (-EINVAL);
    }

    frame.id = id;
    frame.size = dlc;
    frame.rtr = 0;

    for (i = 0; i < dlc; i++) {
        if (std_strtol(argv[3+i], &value) != 0) {
            std_fprintf(out_p, FSTR("%s: unable to parse integer\r\n"), argv[3+i]);

            return (1);
        }

        frame.data[i] = value;
    }

    if (mcp2515_write(&mcp2515, &frame) != sizeof(frame)) {
        std_printf(FSTR("failed to write frame\r\n"));
        return (-1);
    }

    return (0);
}

int main()
{
    struct pin_driver_t pin;

    sys_start();
    uart_module_init();

    uart_init(&uart, &uart_device[0], 38400, qinbuf, sizeof(qinbuf));
    uart_start(&uart);
    sys_set_stdout(&uart.chout);

    /* SPI in automatically configured as slave if this pin is input
     * on AVR MEGA 2560. */
    pin_init(&pin, &pin_d53_dev, PIN_OUTPUT);

    queue_init(&frame_input_queue, frames, sizeof(frames));

    mcp2515_init(&mcp2515,
                 &spi_device[0],
                 &pin_d10_dev,
                 &exti_d2_dev,
                 &frame_input_queue,
                 MCP2515_MODE_LOOPBACK,
                 MCP2515_SPEED_1000KBPS);

    mcp2515_start(&mcp2515);

    shell_args.chin_p = &uart.chin;
    shell_args.chout_p = &uart.chout;
    shell_args.username_p = NULL;
    shell_args.password_p = NULL;
    shell_entry(&shell_args);

    return (0);
}
