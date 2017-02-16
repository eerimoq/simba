#![no_std]
#![feature(concat_idents)]

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
