#![no_std]
#![feature(concat_idents)]

/**
 * @file main.rs
 * @version 3.0.0
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
use simba::slib::harness;

testcase_define!(test_vector);
fn test_vector_impl(_: *mut harness::Harness)
                    -> simba::Res
{
    let mut vec = vec![1, 2, 3, 4, 5, 6, 7, 8];

    vec.push(9);
    vec.append(&mut vec![10]);
    assert!(vec == [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]);
    assert!(vec.pop() == Some(10));

    println!("vec = {:?}", vec);

    while let Some(top) = vec.pop() {
        println!("{}", top);
    }

    println!("vec = {:?}", vec);
    
    Ok(0)
}

#[no_mangle]
pub fn main()
{
    let mut harness: harness::Harness = Default::default();
    let mut harness_testcases = [
        testcase!(Some(test_vector), "test_vector"),
        testcase!(None, "")
    ];

    sys::start();
    uart::init();

    harness.init();
    harness.run(&mut harness_testcases);
}
