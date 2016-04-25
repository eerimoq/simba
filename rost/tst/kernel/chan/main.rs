#![no_std]
#![feature(concat_idents)]
#![feature(collections)]

extern crate collections;

use collections::vec::Vec;

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

use simba::kernel::{chan, errno, event, queue, sys, time};
use simba::kernel::chan::Channel;
use simba::drivers::uart;
use simba::slib::harness::Harness;

testcase_define!(test_poll);
fn test_poll_impl(_: *mut Harness)
                  -> simba::Res
{
    let timeout = time::Time { seconds: 0, nanoseconds: 100 };
    let mut queue = queue::Queue::new(Some(32));
    let mut event = event::Event::new();

    println!("1. Writing to the queue channel.");
    assert!(queue.write(&[2, 1, 0]) == Ok(3));

    /* Poll the list waiting for data on any channel in the list. */
    loop {
        println!("Polling...");

        let res;

        {
            let mut l: Vec<&mut Channel> = vec![&mut queue, &mut event];
            res = chan::poll(&mut l, &Some(timeout));
        }

        match res {

            Ok(0) => {
                println!("2. Reading from the queue channel.");
                let mut buf: [u8; 3] = [0; 3];
                assert!(queue.read(&mut buf) == Ok(3));
            },

            Ok(1) => {
                println!("4. Reading from the event channel.");
                let mut mask: [u8; 4] = [0; 4];
                assert!(event.read(&mut mask) == Ok(4));
            },

            Err(errno::ETIMEDOUT) => {
                println!("3. Timeout. Writing to the event channel.");
                assert!(event.write(&[0, 0, 0, 1]) == Ok(4));
            },

            _ => {
                unreachable!();
            }
        }
    }
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
