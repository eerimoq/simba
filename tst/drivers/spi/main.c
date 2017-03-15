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

static struct spi_driver_t spi;
static uint8_t txbuf[128];
static uint8_t rxbuf[128];

#if defined(ARCH_ESP32)
#    define PIN_SS_DEV pin_gpio17_dev
#    define SPI_DEVICE spi_v_dev
#else
#    define PIN_SS_DEV pin_d5_dev
#    define SPI_DEVICE spi_device[0]
#endif

static int test_init(struct harness_t *harness_p)
{
    BTASSERT(spi_module_init() == 0);

    BTASSERT(spi_init(&spi,
                      &SPI_DEVICE,
                      &PIN_SS_DEV,
                      SPI_MODE_MASTER,
                      SPI_SPEED_125KBPS,
                      0,
                      0) == 0);

    BTASSERT(spi_start(&spi) == 0);

    return (0);
}

static int test_transfer(struct harness_t *harness_p)
{
    int i;

    for (i = 0; i < membersof(txbuf); i++) {
        txbuf[i] = i;
        rxbuf[i] = 1;
    }

    BTASSERT(spi_take_bus(&spi) == 0);
    BTASSERT(spi_select(&spi) == 0);

    BTASSERT(spi_transfer(&spi,
                          &rxbuf[4],
                          &txbuf[4],
                          sizeof(rxbuf) - 7) == sizeof(rxbuf) - 7);

    BTASSERT(spi_deselect(&spi) == 0);
    BTASSERT(spi_give_bus(&spi) == 0);

    for (i = 0; i < membersof(txbuf); i++) {
        std_printf(FSTR("[%d]: tx = 0x%x, rx = 0x%x\r\n"),
                   i,
                   txbuf[i],
                   rxbuf[i]);
    }

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_init, "test_init" },
        { test_transfer, "test_transfer" },
        { NULL, NULL }
    };

    sys_start();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
