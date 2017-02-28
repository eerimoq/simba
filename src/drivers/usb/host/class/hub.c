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

#include "simba.h"

int usb_class_hub_enumerate(struct usb_host_device_t *device_p)
{
    /* Setup the interrupt endpoint. */

    return (0);
}

ssize_t usb_class_hub_read(struct usb_class_hub_t *hub_p,
                           uint8_t *report_p,
                           size_t size)
{
    /* Get an interrupt from the HUB. */

    /* Use GET_STATUS to get the status of all USB hub ports. The status
     * PORT_CONNECTION means a device has been attached. */

    /* CLEAR_FEATURE C_PORT_CONNECTION on the port with the connection. */

    /* Wait for an interrupt that tells us what?? */

    /* Use SET_FEATURE to set the device reset on the device connected
     * to a USB hub port. */

    /* Use CLEAR_FEATURE to clear the device reset. */

    return (-1);
}
