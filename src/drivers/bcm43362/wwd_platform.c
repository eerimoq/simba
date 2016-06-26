/**
 * @file bcm43362.c
 * @version 1.0.0
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
}

void host_platform_power_wifi(wiced_bool_t power_enabled)
{
}

wwd_result_t host_platform_init()
{
    return (WWD_SUCCESS);
}

wwd_result_t host_platform_deinit()
{
    return (WWD_SUCCESS);
}

void host_platform_clocks_needed()
{
}

void host_platform_clocks_not_needed()
{
}

wwd_result_t host_platform_get_mac_address(wiced_mac_t* mac)
{
    return (WWD_SUCCESS);
}

wwd_result_t host_platform_get_ethernet_mac_address(wiced_mac_t* mac)
{
    return (WWD_SUCCESS);
}

uint32_t host_platform_get_cycle_count()
{
    return (0);
}

wiced_bool_t host_platform_is_in_interrupt_context()
{
    return (WWD_SUCCESS);
}

wwd_result_t host_platform_init_wlan_powersave_clock()
{
    return (WWD_SUCCESS);
}

wwd_result_t host_platform_deinit_wlan_powersave_clock()
{
    return (WWD_SUCCESS);
}
