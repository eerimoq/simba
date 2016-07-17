/**
 * @file arm/gnu/time_port.i
 * @version 1.1.0
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

static void time_port_sleep(long usec)
{
    /*
     * Based on Paul Stoffregen's implementation
     * for Teensy 3.0 (http://www.pjrc.com/)
     */
    uint32_t iterations;

    if (usec == 0) {
        return;
    }

    iterations = (usec * (F_CPU / 3000000));

    asm volatile("L_%=_time_port_sleep:"       "\n\t"
                 "subs   %0, #1"               "\n\t"
                 "bne    L_%=_time_port_sleep" "\n"
                 : "+r" (iterations) : );
}
