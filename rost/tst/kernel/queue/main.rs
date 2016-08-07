#![no_std]
#![feature(concat_idents)]

/**
 * @file main.c
 * @version 5.0.0
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

use simba::kernel::{sys, thrd, queue};
use simba::kernel::chan::ChanHandleTrait;
use simba::drivers::uart;
use simba::slib::harness::Harness;
use core::mem::transmute;

#[derive(Debug, PartialEq)]
struct Foo {
    a: i32,
    b: u32,
    c: [u8; 5]
}

const STACK_SIZE: u32 = 2048;
const PRIO: i32 = 0;

static mut QUEUE: queue::Queue = queue!();
static mut QUEUE_INBUF: [u8; 32] = [0; 32];

testcase_define!(test_read_write);
fn test_read_write_impl(_: *mut Harness)
                        -> simba::Res
{
    let mut foo = Foo { a: 0, b: 0, c: [0; 5] };
    let (tx, rx) = queue::Queue::init(unsafe { &mut QUEUE },
                                      unsafe { &mut QUEUE_INBUF });
    let tx2 = tx.clone();

    let child = thrd::spawn(move || {
        let buf: [u8; 8] = [1, 2, 3, 4, 5, 6, 7, 8];

        /* Write the array twice. */
        assert!(tx.write(&buf[0..1]) == Ok(1));
        assert!(tx.write(&buf[1..2]) == Ok(1));
        assert!(tx.write(&buf[2..4]) == Ok(2));
        assert!(tx.write(&buf[4..8]) == Ok(4));
        assert!(tx.write(&buf[0..8]) == Ok(8));
    }, PRIO, STACK_SIZE);

    /* Wait for the child to terminate. */
    assert!(child.join() == Ok(0));

    /* Read the array twice. */
    let mut buf: [u8; 8] = [0; 8];
    assert!(rx.read(&mut buf) == Ok(8));
    assert!(buf == [1, 2, 3, 4, 5, 6, 7, 8]);
    assert!(rx.read(&mut buf[0..1]) == Ok(1));
    assert!(rx.read(&mut buf[1..2]) == Ok(1));
    assert!(rx.read(&mut buf[2..4]) == Ok(2));
    assert!(rx.read(&mut buf[4..8]) == Ok(4));
    assert!(buf == [1, 2, 3, 4, 5, 6, 7, 8]);

    let child = thrd::spawn(move || {
        let foo = Foo { a: -1, b: 2, c: [5, 6, 7, 8, 9] };

        /* Write the struct foo. */
        println!("writing = {:?}", foo);
        assert!(tx2.write(unsafe {
            transmute::<&Foo, &[u8; 16]>(&foo)
        }) == Ok(16));
    }, PRIO, STACK_SIZE);

    /* Read the struct foo. */
    assert!(rx.read(unsafe {
        transmute::<&mut Foo, &mut [u8; 16]>(&mut foo)
    }) == Ok(16));
    println!("read = {:?}", foo);
    assert!(foo == Foo { a: -1, b: 2, c: [5, 6, 7, 8, 9] });

    /* Wait for the child to terminate. */
    assert!(child.join() == Ok(0));

    Ok(0)
}

#[no_mangle]
pub fn main()
{
    let mut harness: Harness = Default::default();
    let mut harness_testcases = [
        testcase!(Some(test_read_write), "test_read_write"),
        testcase!(None, "")
    ];

    sys::start();
    uart::init();

    harness.init();
    harness.run(&mut harness_testcases);
}
