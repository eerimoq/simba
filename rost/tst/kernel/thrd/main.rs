#![no_std]
#![feature(concat_idents)]

/**
 * @file main.rs
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
    
use simba::boxed::Box;

use simba::kernel::{sys, thrd};
use simba::drivers::uart;
use simba::slib::harness;

#[derive(Debug, PartialEq)]
struct Foo {
    a: i32,
    b: u32
}

const STACK_SIZE: u32 = 2500;
const PRIO: i32 = 0;

testcase_define!(test_spawn_join);
fn test_spawn_join_impl(_: *mut harness::Harness)
                        -> simba::Res
{
    /* Spawn a child thread... */
    let child = thrd::spawn(move || {
        println!("Child thread!");
    }, PRIO, STACK_SIZE);
    
    /* ... and wait for it to terminate. */
    try!(child.join());

    println!("Child terminated.");

    Ok(0)
}

testcase_define!(test_nested_spawn_join);
fn test_nested_spawn_join_impl(_: *mut harness::Harness)
                               -> simba::Res
{
    /* Spawn a child thread... */
    let child = thrd::spawn(move || {
        println!("Child thread!");

        /* Spawn a baby thread... */
        let baby = thrd::spawn(move || {
            println!("Baby thread!");
        }, PRIO, STACK_SIZE);
        
        /* ... and wait for it to terminate. */
        baby.join().unwrap();
        
        println!("Baby terminated.");
    }, PRIO, STACK_SIZE);
    
    /* ... and wait for it to terminate. */
    try!(child.join());

    println!("Child terminated.");

    Ok(0)
}

testcase_define!(test_large_closure);
fn test_large_closure_impl(_: *mut harness::Harness)
                           -> simba::Res
{
    let vec = vec![1, 2, 3, 4, 5, 6, 7, 8];
    let foo = Foo { a: -1, b: 2 };
    let boxed_foo = Box::new(foo);

    /* Spawn a child thread... */
    let child = thrd::spawn(move || {
        println!("vec = {:?}", vec);
        println!("boxed_foo = {:?}", boxed_foo);
        assert!(vec == [1, 2, 3, 4, 5, 6, 7, 8]);
        assert!(*boxed_foo == Foo { a: -1, b: 2 });
    }, PRIO, STACK_SIZE);
    
    /* ... and wait for it to terminate. */
    try!(child.join());

    Ok(0)
}

#[no_mangle]
pub fn main()
{
    let mut harness: harness::Harness = Default::default();
    let mut harness_testcases = [
        testcase!(Some(test_spawn_join), "test_spawn_join"),
        testcase!(Some(test_nested_spawn_join), "test_nested_spawn_join"),
        testcase!(Some(test_large_closure), "test_large_closure"),
        testcase!(None, "")
    ];

    sys::start();
    uart::init();

    harness.init();
    harness.run(&mut harness_testcases);
}
