#![no_std]
#![feature(concat_idents)]

/**
 * @file main.c
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

use simba::kernel::{errno, event, queue, sys, time};
use simba::kernel::chan::ChanHandleTrait;
use simba::drivers::uart;
use simba::slib::harness::Harness;

testcase_define!(test_poll);
fn test_poll_impl(_: *mut Harness)
                  -> simba::Res
{
    let timeout = time::Time { seconds: 0, nanoseconds: 100 };
    let mut queue = queue::Queue::new(Some(32));
    let mut event = event::Event::new();
    let mut list = chan::Poll::new(2);

    /* Add the channels to the poll list. */
    list.add(&mut queue);
    list.add(&mut event);

    println!("1. Writing to the queue channel.");
    queue.write(&[2, 1, 0]);

    /* Poll the list waiting for data on any channel in the list. */
    loop {
        println!("Polling the list.");

        match list.poll(&Some(timeout)) {

            Ok(0) => {
                println!("2. Reading from the queue channel.");
                assert!(queue.read(3) == Ok([2, 1, 0]));
            },

            Ok(1) => {
                println!("4. Reading from the event channel.");
                assert!(event.read(0x1) == Ok(0x1));
                break;
            },

            Err(errno::ETIMEDOUT) => {
                println!("3. Timeout. Writing to the event channel.");
                assert!(event.write(0x1) == Ok(0));
            },

            _ => unreachable!();
        }
    }

    Ok(0)
}

#[no_mangle]
pub fn main()
{
    let mut harness: Harness = Default::default();
    let mut harness_testcases = [
        testcase!(Some(test_poll), "test_poll"),
        testcase!(None, "")
    ];

    sys::start();
    uart::init();

    harness.init();
    harness.run(&mut harness_testcases);
}
