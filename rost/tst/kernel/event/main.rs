#![no_std]
#![feature(concat_idents)]

/**
 * @file main.c
 * @version 0.6.0
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

use simba::kernel::{sys, event};
use simba::drivers::uart;
use simba::slib::harness;

const EVENT_BIT_0: u32 = 0x1;
const EVENT_BIT_1: u32 = 0x2;
const EVENT_BIT_2: u32 = 0x4;

testcase_define!(test_read_write);
fn test_read_write_impl(_: *mut harness::Harness)
                        -> simba::Res
{
    let mut mask: u32;
    let mut event = event::Event::new();

    /* Write two events. */
    mask = EVENT_BIT_1 | EVENT_BIT_0;
    assert!(event.write(&mask) == Ok(0));

    assert!(event.size() == Ok(1));

    /* Read first event. */
    mask = EVENT_BIT_0;
    assert!(event.read(&mut mask) == Ok(0));
    assert!(mask == EVENT_BIT_0);

    /* Read second event. */
    mask = EVENT_BIT_1 | EVENT_BIT_0;
    assert!(event.read(&mut mask) == Ok(0));
    assert!(mask == EVENT_BIT_1);

    /* Write second and third events. */
    mask = EVENT_BIT_2 | EVENT_BIT_1;
    assert!(event.write(&mask) == Ok(0));

    /* Write first event. */
    mask = EVENT_BIT_0;
    assert!(event.write(&mask) == Ok(0));

    /* Read first and second events. */
    mask = EVENT_BIT_1 | EVENT_BIT_0;
    assert!(event.read(&mut mask) == Ok(0));
    assert!(mask == (EVENT_BIT_1 | EVENT_BIT_0));

    /* Read third event. */
    mask = EVENT_BIT_2;
    assert!(event.read(&mut mask) == Ok(0));
    assert!(mask == EVENT_BIT_2);

    assert!(event.size() == Ok(0));

    Ok(0)
}

#[no_mangle]
pub fn main()
{
    let mut harness: harness::Harness = Default::default();
    let mut harness_testcases = [
        testcase!(Some(test_read_write), "test_read_write"),
        testcase!(None, "")
    ];

    sys::start();
    uart::init();

    harness.init();
    harness.run(&mut harness_testcases);
}
