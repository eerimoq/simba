/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2016, Erik Moqvist
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

#define SERVER_ADDRESS 0x12345678
#define CLIENT_ADDRESS 0x87654321

int main()
{
    struct nrf24l01_driver_t nrf24l01;
    struct pin_driver_t pin[3];
    uint8_t state[32];

    sys_start();

    nrf24l01_init(&nrf24l01,
                  &spi_device[0],
                  &pin_d10_dev,
                  &pin_d6_dev,
                  &exti_device[1],
                  SERVER_ADDRESS);
    nrf24l01_start(&nrf24l01);

    /* Initialize led pins. */
    pin_init(&pin[0], &pin_d7_dev, PIN_OUTPUT);
    pin_init(&pin[1], &pin_d8_dev, PIN_OUTPUT);
    pin_init(&pin[2], &pin_d9_dev, PIN_OUTPUT);

    pin_write(&pin[0], 0);
    pin_write(&pin[1], 0);
    pin_write(&pin[2], 0);

    while (1) {
        /* Read state from client. */
        nrf24l01_read(&nrf24l01, state, sizeof(state));
        std_printf(FSTR("state = 0x%x\r\n"), (int)state[0]);

        /* Upadte LED. */
        pin_write(&pin[0], (state[0] >> 0) & 0x1);
        pin_write(&pin[1], (state[0] >> 1) & 0x1);
        pin_write(&pin[2], (state[0] >> 2) & 0x1);
    }

    return (0);
}
