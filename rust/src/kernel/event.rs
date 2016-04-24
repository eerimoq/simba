/**
 * @file kernel/event.rs
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * This file is part of the Simba project.
 */

pub type Event = ::Struct_event_t;

pub trait EventBuffer {
    fn buf_p(&self) -> *mut ::std::os::raw::c_void;
    fn len(&self) -> u32;
}

impl Event {

    pub fn new()
               -> Event
    {
        let mut event: Event = Default::default();

        unsafe {
            ::event_init(&mut event);
        }

        event
    }

    pub fn write<T: EventBuffer>(&mut self, value: &T)
                                 -> ::Res
    {
        unsafe {
            ::event_write(self,
                          value.buf_p(),
                          value.len());
        }

        Ok(0)
    }

    pub fn read<T: EventBuffer>(&mut self, value: &mut T)
                                -> ::Res
    {
        unsafe {
            ::event_read(self,
                         value.buf_p(),
                         value.len());
        }

        Ok(0)
    }

    pub fn size(&mut self)
                -> ::Res
    {
        unsafe {
            Ok(::event_size(self))
        }
    }
}

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
