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

#include "network/wwd_buffer_interface.h"

wwd_result_t host_platform_sdio_transfer(wwd_bus_transfer_direction_t direction,
                                         sdio_command_t command,
                                         sdio_transfer_mode_t mode,
                                         sdio_block_size_t block_size,
                                         uint32_t argument,
                                         uint32_t* data,
                                         uint16_t data_size,
                                         sdio_response_needed_t response_expected,
                                         uint32_t* response)
{
    ASSERTN(0, ENOSYS);
    
    return (WWD_SUCCESS);
}

wwd_result_t host_platform_sdio_enumerate()
{
    ASSERTN(0, ENOSYS);
    
    return (WWD_SUCCESS);
}

void host_platform_enable_high_speed_sdio()
{
    ASSERTN(0, ENOSYS);
}

void sdio_irq()
{
    ASSERTN(0, ENOSYS);
}

wwd_result_t host_platform_unmask_sdio_interrupt()
{
    ASSERTN(0, ENOSYS);
    
    return (WWD_SUCCESS);
}

wwd_result_t host_enable_oob_interrupt()
{
    ASSERTN(0, ENOSYS);
    
    return (WWD_SUCCESS);
}

uint8_t host_platform_get_oob_interrupt_pin()
{
    ASSERTN(0, ENOSYS);
    
    return (0);
}
