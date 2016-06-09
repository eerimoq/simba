#![no_std]
#![feature(lang_items)]
#![feature(concat_idents)]
#![feature(shared)]
#![feature(macro_reexport)]
#![feature(allow_internal_unstable)]
#![feature(collections)]
#![feature(alloc)]

//
// @file lib.rs
// @version 0.2.0
//
// @section License
// Copyright (C) 2014-2016, Erik Moqvist
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// This file is part of the Pumba project.
//

// Types required by the generated file simba.rs.
pub mod std {
    pub mod os {
        #[allow(non_camel_case_types)]
        pub mod raw {
            pub type c_void = i32;
            pub type c_char = i8;
            pub type c_uchar = u8;
            pub type c_short = i16;
            pub type c_ushort = u16;
            pub type c_int = i32;
            pub type c_uint = u32;
            pub type c_long = i32;
            pub type c_ulong = u32;
            pub type c_longlong = u64;
            pub type c_ulonglong = u64;
            pub type c_float = f32;
            pub type c_double = f64;
        }   
    }
}

include!("simba.rs");

/* Convert RUST string to "const char *". */
#[macro_export]
macro_rules! const_char_p {
    ($str:expr) => ($str as *const _ as *const $crate::std::os::raw::c_char)
}

/* Testcase definition helper. */
#[macro_export]
macro_rules! testcase_define {
    ($callback:ident) => (
        unsafe extern "C" fn $callback(
            harness_p: *mut $crate::Struct_harness_t)
            -> $crate::std::os::raw::c_int
        {
            match concat_idents!($callback, _impl)(
                harness_p
                    as *mut $crate::slib::harness::Harness) {
                Ok(_) => 0,
                _ => 1
            }
        }
    )
}

#[macro_export]
macro_rules! testcase {
    ($callback:expr, $name:expr) => (
        $crate::slib::harness::HarnessTestcase {
            callback: $callback,
            name_p: const_char_p!(concat!($name, "\0"))
        }
    )
}

#[macro_export]
macro_rules! print {
    ($($arg:tt)*) => ({
        use core::fmt::Write;
        unsafe {
            $crate::sem_take(&mut $crate::kernel::sys::STDOUT.sem
                            as *mut $crate::Struct_sem_t,
                            0 as *mut $crate::Struct_time_t);
            $crate::kernel::sys::STDOUT.write_fmt(
                format_args!($($arg)*)).unwrap();
            $crate::sem_give(&mut $crate::kernel::sys::STDOUT.sem,
                            1);
        }
    });
}

#[macro_export]
macro_rules! println {
    ($fmt:expr) => (print!(concat!($fmt, "\r\n")));
    ($fmt:expr, $($arg:tt)*) => (print!(concat!($fmt, "\r\n"), $($arg)*));
}

#[macro_export]
macro_rules! thrd {
    ($main:expr, $arg:expr, $prio:expr, $stack:expr) => (
        $crate::kernel::thrd::Thrd {
            main: $main,
            arg: $arg,
            prio: $prio,
            stack: $stack,
            thrd: None
        }
    )
}

#[macro_export]
macro_rules! chan {
    () => (
        $crate::kernel::chan::Chan {
            read: None,
            write: None,
            size: None,
            writer_p: 0 as *mut $crate::Struct_thrd_t,
            reader_p: 0 as *mut $crate::Struct_thrd_t,
            list_p: 0 as *mut $crate::Struct_chan_list_t,
        }
    )
}

#[macro_export]
macro_rules! queue {
    () => (
        $crate::kernel::queue::Queue {
            inner: $crate::Struct_queue_t {
                base: chan!(),
                buffer: $crate::Struct_queue_buffer_t {
                    begin_p: 0 as *mut _,
                    read_p: 0 as *mut _,
                    write_p: 0 as *mut _,
                    end_p: 0 as *mut _,
                    size: 0
                },
                state: 0,
                buf_p: 0 as *mut _,
                size: 0,
                left: 0
            }
        }
    )
}

// This function is called when a panic occurs.
#[allow(unused_variables)]
#[lang="panic_fmt"]
pub extern "C" fn rust_begin_unwind(args: core::fmt::Arguments,
                                    file: &'static str,
                                    line: u32) -> !
{
    println!("{}:{}: panic", file, line);
    loop {}
}

#[macro_use]
#[macro_reexport(vec, format)]
extern crate collections as core_collections;

pub use core_collections::vec;

extern crate alloc;
pub use alloc::boxed;

pub type Res = Result<i32, i32>;

pub mod kernel;
pub mod drivers;
pub mod slib;
