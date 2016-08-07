/**
 * @file kernel/chan.rs
 * @version 5.0.0
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

use ::kernel::errno;
use core_collections::Vec;

pub trait Channel {

    fn get_chan_p(&mut self) -> *mut ::std::os::raw::c_void;

    fn write(&mut self, buf: &[u8]) -> ::Res;

    fn read(&mut self, buf: &mut [u8]) -> ::Res;
}

pub fn poll(list: &mut Vec<&mut Channel>,
            timeout: &Option<::kernel::time::Time>)
            -> ::Res
{
    let mut chan_list: ::Struct_chan_list_t = Default::default();
    let mut workspace: [u8; 64] = [0; 64];

    unsafe {
        ::chan_list_init(&mut chan_list as *mut ::Struct_chan_list_t,
                         workspace.as_ptr() as *mut i32,
                         64);

        for item in list {
            ::chan_list_add(&mut chan_list as *mut ::Struct_chan_list_t,
                            item.get_chan_p());
        }
    }

    Ok(0)
}
