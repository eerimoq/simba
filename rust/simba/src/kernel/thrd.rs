/**
 * @file kernel/thrd.rs
 * @version 0.2.0
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
 * This file is part of the Pumba project.
 */

use core::mem;
use core::ptr;

pub struct Thrd {
    thrd_p: *mut ::Struct_thrd_t
}

impl Thrd {

    pub fn join(&self) -> ::Res
    {
        unsafe {
            ::thrd_join(self.thrd_p);
        }

        Ok(0)
    }
}

unsafe extern "C" fn thrd_main(arg_p: *mut ::std::os::raw::c_void)
                               -> *mut ::std::os::raw::c_void
{
    let fn_p: *const fn(*mut ::std::os::raw::c_void,
                        *mut ::std::os::raw::c_void);
    let env_p: *mut ::std::os::raw::c_void;

    fn_p = arg_p as *const fn(*mut ::std::os::raw::c_void,
                              *mut ::std::os::raw::c_void);
    env_p = (arg_p as usize + mem::size_of::<*mut usize>())
        as *mut ::std::os::raw::c_void;

    (*fn_p)(env_p, env_p);

    0 as *mut i32
}

pub fn spawn<F: FnOnce() + Send>(closure: F,
                                 prio: i32,
                                 stack_size: u32)
                                 -> Thrd
{
    let closure_size = mem::size_of::<F>() + mem::size_of::<*mut u8>();
    let thrd_p;
    let mut stack_p = unsafe {
        ::kernel::sys::__rust_allocate(stack_size as usize, 8)
    };

    /* Alignment. */
    stack_p = (((stack_p as usize) + 8) & 0xfffffff8) as *mut u8;

    fn closure_main<F: FnOnce() + Send>(env_p: *const F) -> ::Res
    {
        let closure = unsafe { ::core::ptr::read(env_p) };
        closure();
        Ok(0)
    }

    unsafe {
        /* Write the closure entry function pointer and environment to
        the target stack. */
        ptr::write(stack_p as *mut usize,
                   closure_main::<F> as usize);
        ptr::write((stack_p as usize + mem::size_of::<*mut u8>()) as *mut F,
                   closure);

        thrd_p = ::thrd_spawn(Some(thrd_main),
                              stack_p as *mut i32,
                              prio,
                              (stack_p as usize + closure_size) as *mut i32,
                              (stack_size - closure_size as u32));
    }

    Thrd {
        thrd_p: thrd_p
    }
}
