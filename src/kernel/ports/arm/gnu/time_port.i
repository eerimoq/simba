/**
 * @file arm/gnu/time_port.i
 * @version 3.1.1
 *
 * @section License
 * Copyright (C) 2015-2016, Erik Moqvist
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

static void time_port_busy_wait_us(long microseconds)
{
    /*
     * Based on Paul Stoffregen's implementation
     * for Teensy 3.0 (http://www.pjrc.com/)
     */
    uint32_t iterations;

    if (microseconds == 0) {
        return;
    }

    iterations = (microseconds * (F_CPU / 3000000));

    asm volatile("L_%=_time_port_busy_wait_us:"       "\n\t"
                 "subs   %0, #1"               "\n\t"
                 "bne    L_%=_time_port_busy_wait_us" "\n"
                 : "+r" (iterations) : );
}
