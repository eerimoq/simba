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
