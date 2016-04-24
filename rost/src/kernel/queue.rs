/**
 * @file kernel/queue.rs
 * @version 0.3.0
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

use core::ptr::Shared;

pub struct Queue {
    pub inner: ::Struct_queue_t
}

impl Queue {

    pub fn init(&'static mut self, inbuf: &'static mut [u8])
                -> (QueueTx, QueueRx)
    {
        unsafe {
            ::queue_init(&mut self.inner,
                         inbuf.as_ptr() as *mut i32,
                         inbuf.len() as u32);
        }
        
        (QueueTx { chan_p: unsafe { Shared::new(self) } },
         QueueRx { chan_p: unsafe { Shared::new(self) } })
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

#[derive(Clone)]
pub struct QueueTx {
    chan_p: Shared<Queue>
}

unsafe impl Send for QueueTx {}

impl ::kernel::chan::ChanHandleTrait for QueueTx {

    fn get_chan_p(&self) -> *mut ::std::os::raw::c_void
    {
        *self.chan_p as *mut ::std::os::raw::c_void
    }

    fn write(&self, buf: &[u8]) -> ::Res
    {
        unsafe {
            (**self.chan_p).write(buf)
        }
    }

    fn read(&self, buf: &mut [u8]) -> ::Res
    {
        println!("bad read");
        panic!();
    }
}

pub struct QueueRx {
    chan_p: Shared<Queue>
}

unsafe impl Send for QueueRx {}

impl ::kernel::chan::ChanHandleTrait for QueueRx {

    fn get_chan_p(&self) -> *mut ::std::os::raw::c_void
    {
        *self.chan_p as *mut ::std::os::raw::c_void
    }

    fn write(&self, buf: &[u8]) -> ::Res
    {
        println!("bad write");
        panic!();
    }

    fn read(&self, buf: &mut [u8]) -> ::Res
    {
        unsafe {
            (**self.chan_p).read(buf)
        }
    }
}
