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

use simba::kernel::{sys,thrd};
use simba::drivers::uart;

static mut s_timeout: i32 = 0;
static mut s_stack0: [u8; 1024] = [0; 1024];
static mut s_stack1: [u8; 1024] = [0; 1024];
static mut s_thrd0: thrd::Thrd = thrd::Thrd {
    main: foo_main,
    arg: None,
    prio: 0,
    stack: &mut [],
    thrd: None
};
static mut s_thrd1: thrd::Thrd = thrd::Thrd {
    main: foo_main,
    arg: None,
    prio: 0,
    stack: &mut [],
    thrd: None
};

fn foo_main(arg: Option<*mut i32>) -> Option<*mut i32>
{
    let mut count: i32 = 0;

    /* Default timeout is 0.5 seconds. */
    let timeout = match arg {
        Some(value) => unsafe { *value },
        _ => 500000
    };

    /* Thread main loop. */
    loop {
        println!("thread: 0x%08x, count: %3d: Hello World!",
                 simba::thrd_self() as u32,
                 count);
        count += 1;

        thrd::usleep(timeout);
    }
}

#[no_mangle]
pub fn main()
{
    let mut uart: uart::UartDriver = Default::default();

    /* Setup the system. */
    sys::start();
    uart::init();
    uart.init(0, 38400, &mut []);
    uart.start();
    sys::set_stdout(&mut uart.chout);

    /* Modifying static variables requires unsafe block. */
    unsafe {
        s_timeout = 1000000;

        s_thrd0.stack = &mut s_stack0;
        s_thrd0.arg = Some(&mut s_timeout);
        s_thrd1.stack = &mut s_stack1;

        /* Spawn two threads that prints "Hello World!" periodically. */
        s_thrd0.spawn();
        s_thrd1.spawn();
    }

    thrd::suspend(None);
}
