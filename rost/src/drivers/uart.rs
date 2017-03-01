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

pub fn init()
{
    unsafe {
        ::uart_module_init();
    }
}

pub type UartDriver = ::Struct_uart_driver_t;

impl UartDriver {
    
    pub fn init(&mut self,
                dev: i32,
                baudrate: i32,
                rxbuf: &mut [u8])
    {
        unsafe {
            ::uart_init(self,
                        match dev {
                            0 => &mut ::uart_device[0],
                            1 => &mut ::uart_device[1],
                            2 => &mut ::uart_device[2],
                            3 => &mut ::uart_device[3],
                            _ => &mut ::uart_device[0]
                        },
                        baudrate,
                        rxbuf as *mut _ as *mut ::std::os::raw::c_void,
                        rxbuf.len() as u32);
        }
    }

    pub fn start(&mut self)
    {
        unsafe {
            ::uart_start(self);
        }
    }

    pub fn stop(&mut self)
    {
        unsafe {
            ::uart_stop(self);
        }
    }
}
