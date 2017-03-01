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

use core::fmt;
use core::ptr;

pub static mut STDOUT: Stdout = Stdout {
    sem: ::Struct_sem_t {
        count: 0,
        count_max: 1,
        head_p: 0 as *mut ::Struct_sem_elem_t
    }
};

pub static mut HEAP: ::Struct_heap_t = ::Struct_heap_t {
    buf_p: 0 as *mut ::std::os::raw::c_void,
    size: 0,
    next_p: 0 as *mut ::std::os::raw::c_void,
    fixed: [::Struct_heap_fixed_t {
        free_p: 0 as *mut ::std::os::raw::c_void,
        size: 0,
    }; 8usize],
    dynamic: ::Struct_heap_dynamic_t {
        free_p: 0 as *mut ::std::os::raw::c_void,
    }
};

static mut HEAP_BUF: [u8; 16384] = [0; 16384];

pub struct Stdout {
    pub sem: ::Struct_sem_t
}

impl fmt::Write for Stdout {

    fn write_str(&mut self, s: &str) -> ::core::fmt::Result {
        unsafe {
            for b in s.bytes() {
                let a: [u8; 1] = [b];
                ::chan_write(::sys_get_stdout(),
                             a.as_ptr() as *const _ as *const i32,
                             a.len() as u32);
            }
        }

        Ok(())
    }
}

pub fn start()
{
    let sizes: [u32; 8] = [16, 32, 64, 128, 256, 512, 1024, 2048];

    unsafe {
        ::sem_init(&mut STDOUT.sem as *mut ::Struct_sem_t, 0, 1);
        ::heap_init(&mut HEAP as *mut ::Struct_heap_t,
                    HEAP_BUF.as_ptr() as *mut i32,
                    HEAP_BUF.len() as u32,
                    sizes.as_ptr() as *mut u32);
        ::sys_start();
    }
}

pub fn stop() {
}

pub fn set_stdout<T: ::kernel::chan::Channel>(chout: &mut T)
{
    unsafe {
        ::sys_set_stdout(chout.get_chan_p());
    }
}

#[no_mangle]
pub extern "C" fn __rust_allocate(size: usize,
                                  align: usize)
                                  -> *mut u8
{
    unsafe {
        ::heap_alloc(&mut HEAP as *mut ::Struct_heap_t,
                     size as ::size_t) as *mut u8
    }
}

#[no_mangle]
pub extern "C" fn __rust_reallocate(ptr: *mut u8,
                                    _old_size: usize,
                                    size: usize,
                                    align: usize)
                                    -> *mut u8
{
    unsafe {
        let buf_p = __rust_allocate(size, align);
        ptr::copy_nonoverlapping(ptr, buf_p, _old_size);
        __rust_deallocate(ptr, size, align);
        buf_p
    }
}

#[no_mangle]
pub extern "C" fn __rust_deallocate(ptr: *mut u8,
                                    old_size: usize,
                                    align: usize)
{
    let res;
    unsafe {
        res = ::heap_free(&mut HEAP as *mut ::Struct_heap_t,
                          ptr as *mut ::std::os::raw::c_void)
    }
}
