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

ISR(swt_timeout)
{
    /* Clear the interrupt flag. */
    SPC5_SWT->IR = 1;

    if (module.on_interrupt != NULL) {
        module.on_interrupt();
    } else {
        sys_panic("");
    }
}

int watchdog_port_start_ms(int timeout,
                           watchdog_isr_fn_t on_interrupt)
{
    uint32_t itr;

    /* Generate an interrupt if configured. */
    if (on_interrupt == NULL) {
        itr = 0;
    } else {
        itr =  SPC5_SWT_CR_ITR;
        SPC5_INTC->PSR[28 / 4] = 0xffffffff;
    }

    SPC5_SWT->SR = 0x0000c520;
    SPC5_SWT->SR = 0x0000d928;
    SPC5_SWT->CR = (SPC5_SWT_CR_MAP_0
                    | SPC5_SWT_CR_RIA
                    | itr
                    | SPC5_SWT_CR_CSL
                    | SPC5_SWT_CR_FRZ);
    SPC5_SWT->TO = (128 * timeout);
    SPC5_SWT->CR |= (SPC5_SWT_CR_SLK
                     | SPC5_SWT_CR_WEN);

    return (0);
}

int watchdog_port_stop(void)
{
    SPC5_SWT->SR = 0x0000c520;
    SPC5_SWT->SR = 0x0000d928;
    SPC5_SWT->CR &= ~SPC5_SWT_CR_WEN;

    return (0);
}

int watchdog_port_kick(void)
{
    SPC5_SWT->SR = 0x0000a602;
    SPC5_SWT->SR = 0x0000b480;

    return (0);
}
