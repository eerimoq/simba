/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2018, Erik Moqvist
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

#include <avr/wdt.h>

int watchdog_port_start_ms(int timeout,
                           watchdog_isr_fn_t on_interrupt)
{
    uint8_t prescaler;

    /*
     * http://www.nongnu.org/avr-libc/user-manual/group__avr__watchdog.html
     *
     * Timeout value is approximated to:
     *     < 16 ms    => prescaler 0 (~15 ms)
     *     < 32 ms    => prescaler 1 (~30 ms)
     *     < 64 ms    => prescaler 2 (~60 ms)
     *     < 128 ms   => prescaler 3 (~120 ms)
     *     < 256 ms   => prescaler 4 (~250 ms)
     *     < 512 ms   => prescaler 5 (~500 ms)
     *     < 1024 ms  => prescaler 6 (~1 s)
     *     >= 1024 ms => prescaler 7 (~2 s)
     */
    timeout /= 16;
    prescaler = 0;
    while (prescaler < 7) {
        if ((1 << prescaler) > timeout) break;
        prescaler++;
    }

    wdt_enable(prescaler);

    return (0);
}

int watchdog_port_stop(void)
{
    wdt_disable();

    return (0);
}

int watchdog_port_kick(void)
{
    wdt_reset();

    return (0);
}
