#![no_std]

/**
 * @file main.rs
 * @version 0.3.0
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
 * This file is part of the Simba project.
 */

#[macro_use] extern crate simba;

use simba::kernel::sys;
use simba::drivers::uart;

static mut UART: UartDriver = uart_driver!();

#[no_mangle]
pub fn main()
{
    let uart = uart::Uart::init(unsafe { &mut UART });

    /* Setup the system. */
    sys::start();
    uart::init();
    uart.start();
    sys::set_stdout(&uart.tx);

    /* Print "Hello World!". */
    println!("Hello World!");
}
