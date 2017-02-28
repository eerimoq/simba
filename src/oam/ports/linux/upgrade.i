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

struct module_port_t {
    int stay_in_bootloader;
};

static struct module_port_t module_port;

static int upgrade_port_bootloader_enter()
{
    return (-1);
}

static int upgrade_port_bootloader_stay_set()
{
    module_port.stay_in_bootloader = 1;

    return (0);
}

static int upgrade_port_bootloader_stay_clear()
{
    module_port.stay_in_bootloader = 0;

    return (0);
}

static int upgrade_port_bootloader_stay_get()
{
    return (module_port.stay_in_bootloader);
}

static int upgrade_port_application_enter()
{
    return (-1);
}

static int upgrade_port_application_erase()
{
    return (0);
}

static int upgrade_port_application_is_valid()
{
    return (0);
}

static int upgrade_port_binary_upload_begin()
{
    return (0);
}

static int upgrade_port_binary_upload(const void *buf_p,
                                      size_t size)
{
    return (0);
}

static int upgrade_port_binary_upload_end()
{
    return (0);
}
