#![no_std]
#![feature(concat_idents)]

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
