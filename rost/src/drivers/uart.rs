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

pub fn init()
{
    unsafe {
        ::uart_module_init();
    }
}

pub type UartDriver = ::Struct_uart_driver_t;

impl UartDriver {
    
    pub fn init(&mut self,
                dev: i32,
                baudrate: i32,
                rxbuf: &mut [u8])
    {
        unsafe {
            ::uart_init(self,
                        match dev {
                            0 => &mut ::uart_device[0],
                            1 => &mut ::uart_device[1],
                            2 => &mut ::uart_device[2],
                            3 => &mut ::uart_device[3],
                            _ => &mut ::uart_device[0]
                        },
                        baudrate,
                        rxbuf as *mut _ as *mut ::std::os::raw::c_void,
                        rxbuf.len() as u32);
        }
    }

    pub fn start(&mut self)
    {
        unsafe {
            ::uart_start(self);
        }
    }

    pub fn stop(&mut self)
    {
        unsafe {
            ::uart_stop(self);
        }
    }
}
