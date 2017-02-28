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
