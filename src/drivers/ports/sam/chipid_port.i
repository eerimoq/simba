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

int chipid_port_read(struct chipid_t *id_p)
{
    uint32_t cidr;
    
    cidr = SAM_CHIPID->CIDR;
        
    id_p->version = CHIPID_CIDR_VERSION_GET(cidr);
    id_p->embedded_processor = CHIPID_CIDR_EPROC_GET(cidr);
    id_p->non_volatile_program_memory_size[0] = CHIPID_CIDR_NVPSIZ_GET(cidr);
    id_p->non_volatile_program_memory_size[1] = CHIPID_CIDR_NVPSIZ2_GET(cidr);
    id_p->internal_sram_size = CHIPID_CIDR_SRAMSIZ_GET(cidr);
    id_p->architecture = CHIPID_CIDR_ARCH_GET(cidr);
    id_p->non_volatile_program_memory_type = CHIPID_CIDR_NVPTYP_GET(cidr);

    return (0);
}
