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

pub struct Event {
    pub inner: ::Struct_event_t
}

impl Event {

    pub fn new()
               -> Box<Event>
    {
        let mut event: Box<Event> = Box::new(Event { inner: Default::default() });

        unsafe {
            ::event_init(&mut event.inner);
        }

        event
    }
    
    pub fn write(&mut self, buf: &[u8]) -> ::Res
    {
        unsafe {
            Ok(::event_write(&mut self.inner as *mut ::Struct_event_t,
                             buf.as_ptr() as *const i32,
                             buf.len() as u32))
        }
    }

    pub fn read(&mut self, buf: &mut [u8]) -> ::Res
    {
        unsafe {
            Ok(::event_read(&mut self.inner as *mut ::Struct_event_t,
                            buf.as_ptr() as *mut i32,
                            buf.len() as u32))
        }
    }
}

//pub type Event = ::Struct_event_t;

pub trait EventBuffer {
    fn buf_p(&self) -> *mut ::std::os::raw::c_void;
    fn len(&self) -> u32;
}

//impl Event {
//
//    pub fn new()
//               -> Event
//    {
//        let mut event: Event = Default::default();
//
//        unsafe {
//            ::event_init(&mut event);
//        }
//
//        event
//    }
//
//    pub fn write(&mut self, value: u32)
//                 -> ::Res
//    {
//        unsafe {
//            ::event_write(self,
//                          &value as *const _ as *const i32,
//                          4);
//        }
//
//        Ok(0)
//    }
//
//    pub fn read(&mut self, value: &mut u32)
//                -> ::Res
//    {
//        unsafe {
//            ::event_read(self,
//                         value as *mut _ as *mut i32,
//                         4);
//        }
//
//        Ok(0)
//    }
//
//    pub fn size(&mut self)
//                -> ::Res
//    {
//        unsafe {
//            Ok(::event_size(self))
//        }
//    }
//}

impl EventBuffer for u32 {

    fn buf_p(&self) -> *mut ::std::os::raw::c_void
    {
        self as *const _ as *mut ::std::os::raw::c_void
    }

    fn len(&self) -> u32
    {
        4
    }
}

unsafe impl Send for Event {}

impl ::kernel::chan::Channel for Box<Event> {

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
