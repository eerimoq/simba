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

#ifndef __BOARD_H__
#define __BOARD_H__

#define pin_pa0_dev pin_device[]
#define pin_pa1_dev pin_device[]
#define pin_pa2_dev pin_device[]
#define pin_pa3_dev pin_device[]
#define pin_pa4_dev pin_device[]
#define pin_pa5_dev pin_device[]
#define pin_pa6_dev pin_device[]
#define pin_pa7_dev pin_device[]
#define pin_pa8_dev pin_device[]
#define pin_pa9_dev pin_device[]
#define pin_pa10_dev pin_device[]
#define pin_pa11_dev pin_device[]
#define pin_pa12_dev pin_device[]
#define pin_pa13_dev pin_device[]
#define pin_pa14_dev pin_device[]
#define pin_pa15_dev pin_device[]

#define pin_pb0_dev pin_device[]
#define pin_pb1_dev pin_device[]
#define pin_pb2_dev pin_device[]
#define pin_pb3_dev pin_device[]
#define pin_pb4_dev pin_device[]
#define pin_pb5_dev pin_device[]
#define pin_pb6_dev pin_device[]
#define pin_pb7_dev pin_device[]
#define pin_pb8_dev pin_device[]
#define pin_pb9_dev pin_device[]
#define pin_pb10_dev pin_device[]
#define pin_pb11_dev pin_device[]
#define pin_pb12_dev pin_device[]
#define pin_pb13_dev pin_device[]
#define pin_pb14_dev pin_device[]
#define pin_pb15_dev pin_device[]

#define pin_pc0_dev pin_device[]
#define pin_pc1_dev pin_device[]
#define pin_pc2_dev pin_device[]
#define pin_pc3_dev pin_device[]
#define pin_pc4_dev pin_device[]
#define pin_pc5_dev pin_device[]
#define pin_pc6_dev pin_device[]
#define pin_pc7_dev pin_device[]
#define pin_pc8_dev pin_device[]
#define pin_pc9_dev pin_device[]
#define pin_pc10_dev pin_device[]
#define pin_pc11_dev pin_device[]
#define pin_pc12_dev pin_device[]
#define pin_pc13_dev pin_device[]
#define pin_pc14_dev pin_device[]
#define pin_pc15_dev pin_device[]

#define pin_pd0_dev pin_device[]
#define pin_pd1_dev pin_device[]
#define pin_pd2_dev pin_device[]
#define pin_pd3_dev pin_device[]
#define pin_pd4_dev pin_device[]
#define pin_pd5_dev pin_device[]
#define pin_pd6_dev pin_device[]
#define pin_pd7_dev pin_device[]
#define pin_pd8_dev pin_device[]
#define pin_pd9_dev pin_device[]
#define pin_pd10_dev pin_device[]
#define pin_pd11_dev pin_device[]
#define pin_pd12_dev pin_device[]
#define pin_pd13_dev pin_device[]
#define pin_pd14_dev pin_device[]
#define pin_pd15_dev pin_device[]

#define pin_pe0_dev pin_device[]
#define pin_pe1_dev pin_device[]
#define pin_pe2_dev pin_device[]
#define pin_pe3_dev pin_device[]
#define pin_pe4_dev pin_device[]
#define pin_pe5_dev pin_device[]
#define pin_pe6_dev pin_device[]
#define pin_pe7_dev pin_device[]
#define pin_pe8_dev pin_device[]
#define pin_pe9_dev pin_device[]
#define pin_pe10_dev pin_device[]
#define pin_pe11_dev pin_device[]
#define pin_pe12_dev pin_device[]
#define pin_pe13_dev pin_device[]
#define pin_pe14_dev pin_device[]
#define pin_pe15_dev pin_device[]

#define uart_0_dev uart_device[0]
#define uart_1_dev uart_device[1]
#define uart_2_dev uart_device[2]

#define spi_0_dev spi_device[0]
#define spi_1_dev spi_device[1]
#define spi_2_dev spi_device[2]

#define i2c_0_dev i2c_device[0]
#define i2c_1_dev i2c_device[1]

#define can_0_dev can_device[0]

#define flash_0_dev flash_device[0]

/**
 * Convert given pin string to the pin number.
 *
 * @param[in] str_p Pin as a string.
 *
 * @return Pin number or negative error code.
 */
int board_pin_string_to_device_index(const char *str_p);

#endif
