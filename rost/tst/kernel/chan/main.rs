#![no_std]
#![feature(concat_idents)]
#![feature(collections)]

extern crate collections;

use collections::vec::Vec;

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
