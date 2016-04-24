/**
 * @file uart.rs
 * @version 0.2.0
 *
 * @section License
 * Copyright (C) 2014-2016, Erik Moqvist
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
 * This file is part of the Pumba project.
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
