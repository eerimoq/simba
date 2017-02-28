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
