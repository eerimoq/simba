/*
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

#ifndef __BOARD_H__
#define __BOARD_H__

#define pin_ra0_dev  pin_device[0]
#define pin_ra1_dev  pin_device[1]
#define pin_ra2_dev  pin_device[2]
#define pin_ra3_dev  pin_device[3]
#define pin_ra4_dev  pin_device[4]
#define pin_ra6_dev  pin_device[5]
#define pin_ra7_dev  pin_device[6]
#define pin_ra8_dev  pin_device[7]
#define pin_ra9_dev  pin_device[8]
#define pin_ra10_dev pin_device[9]
#define pin_ra15_dev pin_device[10]

#define pin_rb0_dev  pin_device[11]
#define pin_rb1_dev  pin_device[12]
#define pin_rb2_dev  pin_device[13]
#define pin_rb3_dev  pin_device[14]
#define pin_rb4_dev  pin_device[15]
#define pin_rb5_dev  pin_device[16]
#define pin_rb6_dev  pin_device[17]
#define pin_rb7_dev  pin_device[18]
#define pin_rb8_dev  pin_device[19]
#define pin_rb9_dev  pin_device[20]
#define pin_rb10_dev pin_device[21]
#define pin_rb11_dev pin_device[22]
#define pin_rb12_dev pin_device[23]
#define pin_rb13_dev pin_device[24]
#define pin_rb14_dev pin_device[25]
#define pin_rb15_dev pin_device[26]

#define pin_rc0_dev  pin_device[27]
#define pin_rc1_dev  pin_device[28]
#define pin_rc2_dev  pin_device[29]
#define pin_rc3_dev  pin_device[30]
#define pin_rc4_dev  pin_device[31]
#define pin_rc5_dev  pin_device[32]
#define pin_rc6_dev  pin_device[33]
#define pin_rc7_dev  pin_device[34]
#define pin_rc8_dev  pin_device[35]
#define pin_rc9_dev  pin_device[36]
#define pin_rc12_dev pin_device[37]

#define pin_rd0_dev  pin_device[38]

#define flash_dev    flash_device[0]

/**
 * Convert given pin string to the pin number.
 *
 * @param[in] str_p Pin as a string.
 *
 * @return Pin number or negative error code.
 */
int board_pin_string_to_device_index(const char *str_p);

#endif
