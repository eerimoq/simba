/**
 * @file kernel/queue.rs
 * @version 0.5.0
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
 * MERQUEUETABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * This file is part of the Simba project.
 */

use alloc::boxed::Box;

pub struct Queue {
    pub inner: ::Struct_queue_t
}

impl Queue {

    pub fn new(size: Option<usize>)
               -> Queue
    {
        let mut queue: Queue = Queue { inner: Default::default() };
        
        match size {

            Some(bytes) => {
                let mut buf_p = ::kernel::sys::__rust_allocate(bytes, 0);

                unsafe {
                    ::queue_init(&mut queue.inner,
                                 buf_p as *mut i32,
                                 bytes as u32);
                }
            },

            None => {
                unsafe {
                    ::queue_init(&mut queue.inner,
                                 0 as *mut i32,
                                 0 as u32);
                }
            }
        }

        queue
    }
    
    pub fn write(&mut self, buf: &[u8]) -> ::Res
    {
        unsafe {
            Ok(::queue_write(&mut self.inner as *mut ::Struct_queue_t,
                             buf.as_ptr() as *const i32,
                             buf.len() as u32))
        }
    }

    pub fn read(&mut self, buf: &mut [u8]) -> ::Res
    {
        unsafe {
            Ok(::queue_read(&mut self.inner as *mut ::Struct_queue_t,
                            buf.as_ptr() as *mut i32,
                            buf.len() as u32))
        }
    }
}

unsafe impl Send for Queue {}

impl ::kernel::chan::Channel for Queue {

    fn get_chan_p(&mut self) -> *mut ::std::os::raw::c_void
    {
        &mut self.inner.base as *mut _ as *mut ::std::os::raw::c_void
    }

    fn write(&mut self, buf: &[u8]) -> ::Res
    {
        unsafe {
            self.write(buf)
        }
    }

    fn read(&mut self, buf: &mut [u8]) -> ::Res
    {
        unsafe {
            self.read(buf)
        }
    }
}
