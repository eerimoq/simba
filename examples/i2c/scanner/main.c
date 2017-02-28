/**
 * @section License
 *
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org/>
 *
 * This file is part of the Simba project.
 */

#include "simba.h"

struct i2c_driver_t i2c;

int main()
{
    int address;
    int number_of_slaves;
    
    sys_start();
    i2c_module_init();

    i2c_init(&i2c, &i2c_0_dev, I2C_BAUDRATE_100KBPS, -1);
    i2c_start(&i2c);
    
    std_printf(FSTR("Scanning the i2c bus for slaves...\r\n"
                    "\r\n"));

    number_of_slaves = 0;
    
    for (address = 0; address < 128; address++) {
        if (i2c_scan(&i2c, address) == 1) {
            std_printf(FSTR("Found slave with address 0x%x.\r\n"), address);
            number_of_slaves++;
        }
    }

    std_printf(FSTR("\r\n"
                    "Scan complete. Found %d slaves.\r\n"), number_of_slaves);
    
    return (0);
}
