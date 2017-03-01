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
