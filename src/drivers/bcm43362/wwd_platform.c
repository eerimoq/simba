/**
 * @file bcm43362.c
 * @version 1.1.0
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
