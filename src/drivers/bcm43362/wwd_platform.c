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

#include "simba.h"

#include "wwd_structures.h"
#include "network/wwd_buffer_interface.h"

void host_platform_reset_wifi(wiced_bool_t reset_asserted)
{
    ASSERTN(0, ENOSYS);
}

void host_platform_power_wifi(wiced_bool_t power_enabled)
{
    ASSERTN(0, ENOSYS);
}

wwd_result_t host_platform_init()
{
    ASSERTN(0, ENOSYS);
    
    return (WWD_SUCCESS);
}

wwd_result_t host_platform_deinit()
{
    ASSERTN(0, ENOSYS);
    
    return (WWD_SUCCESS);
}

void host_platform_clocks_needed()
{
    ASSERTN(0, ENOSYS);
}

void host_platform_clocks_not_needed()
{
    ASSERTN(0, ENOSYS);
}

wwd_result_t host_platform_get_mac_address(wiced_mac_t* mac)
{
    ASSERTN(0, ENOSYS);
    
    return (WWD_SUCCESS);
}

wwd_result_t host_platform_get_ethernet_mac_address(wiced_mac_t* mac)
{
    ASSERTN(0, ENOSYS);
    
    return (WWD_SUCCESS);
}

uint32_t host_platform_get_cycle_count()
{
    ASSERTN(0, ENOSYS);
    
    return (0);
}

wiced_bool_t host_platform_is_in_interrupt_context()
{
    ASSERTN(0, ENOSYS);
    
    return (WWD_SUCCESS);
}

wwd_result_t host_platform_init_wlan_powersave_clock()
{
    ASSERTN(0, ENOSYS);
    
    return (WWD_SUCCESS);
}

wwd_result_t host_platform_deinit_wlan_powersave_clock()
{
    ASSERTN(0, ENOSYS);
    
    return (WWD_SUCCESS);
}
