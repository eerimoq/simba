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

#define pin_pa0_dev pin_device[0]
#define pin_pa1_dev pin_device[1]
#define pin_pa2_dev pin_device[2]
#define pin_pa3_dev pin_device[3]
#define pin_pa4_dev pin_device[4]
#define pin_pa5_dev pin_device[5]
#define pin_pa6_dev pin_device[6]
#define pin_pa7_dev pin_device[7]
#define pin_pa8_dev pin_device[8]
#define pin_pa9_dev pin_device[9]
#define pin_pa10_dev pin_device[10]
#define pin_pa11_dev pin_device[11]
#define pin_pa12_dev pin_device[12]
#define pin_pa13_dev pin_device[13]
#define pin_pa14_dev pin_device[14]
#define pin_pa15_dev pin_device[15]

#define pin_pb0_dev pin_device[16]
#define pin_pb1_dev pin_device[17]
#define pin_pb2_dev pin_device[18]
#define pin_pb3_dev pin_device[19]
#define pin_pb4_dev pin_device[20]
#define pin_pb5_dev pin_device[21]
#define pin_pb6_dev pin_device[22]
#define pin_pb7_dev pin_device[23]
#define pin_pb8_dev pin_device[24]
#define pin_pb9_dev pin_device[25]
#define pin_pb10_dev pin_device[26]
#define pin_pb11_dev pin_device[27]
#define pin_pb12_dev pin_device[28]
#define pin_pb13_dev pin_device[29]
#define pin_pb14_dev pin_device[30]
#define pin_pb15_dev pin_device[31]

#define pin_pc0_dev pin_device[32]
#define pin_pc1_dev pin_device[33]
#define pin_pc2_dev pin_device[34]
#define pin_pc3_dev pin_device[35]
#define pin_pc4_dev pin_device[36]
#define pin_pc5_dev pin_device[37]
#define pin_pc6_dev pin_device[38]
#define pin_pc7_dev pin_device[39]
#define pin_pc8_dev pin_device[40]
#define pin_pc9_dev pin_device[41]
#define pin_pc10_dev pin_device[42]

#define pin_led_dev pin_pc2_dev

/**
 * Convert given pin string to the pin number.
 *
 * @param[in] str_p Pin as a string.
 *
 * @return Pin number or negative error code.
 */
int board_pin_string_to_device_index(const char *str_p);

#endif
