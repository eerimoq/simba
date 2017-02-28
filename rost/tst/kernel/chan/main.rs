#![no_std]
#![feature(concat_idents)]
#![feature(collections)]

extern crate collections;

use collections::vec::Vec;

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
