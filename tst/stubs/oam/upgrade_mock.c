/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2017, Erik Moqvist
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
#include "upgrade_mock.h"

int mock_write_upgrade_module_init(int res)
{
    harness_mock_write("upgrade_module_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(upgrade_module_init)()
{
    int res;

    harness_mock_read("upgrade_module_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_upgrade_bootloader_enter(int res)
{
    harness_mock_write("upgrade_bootloader_enter(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(upgrade_bootloader_enter)()
{
    int res;

    harness_mock_read("upgrade_bootloader_enter(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_upgrade_bootloader_stay_set(int res)
{
    harness_mock_write("upgrade_bootloader_stay_set(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(upgrade_bootloader_stay_set)()
{
    int res;

    harness_mock_read("upgrade_bootloader_stay_set(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_upgrade_bootloader_stay_clear(int res)
{
    harness_mock_write("upgrade_bootloader_stay_clear(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(upgrade_bootloader_stay_clear)()
{
    int res;

    harness_mock_read("upgrade_bootloader_stay_clear(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_upgrade_bootloader_stay_get(int res)
{
    harness_mock_write("upgrade_bootloader_stay_get(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(upgrade_bootloader_stay_get)()
{
    int res;

    harness_mock_read("upgrade_bootloader_stay_get(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_upgrade_application_enter(int res)
{
    harness_mock_write("upgrade_application_enter(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(upgrade_application_enter)()
{
    int res;

    harness_mock_read("upgrade_application_enter(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_upgrade_application_erase(int res)
{
    harness_mock_write("upgrade_application_erase(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(upgrade_application_erase)()
{
    int res;

    harness_mock_read("upgrade_application_erase(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_upgrade_application_is_valid(int quick,
                                            int res)
{
    harness_mock_write("upgrade_application_is_valid(quick)",
                       &quick,
                       sizeof(quick));

    harness_mock_write("upgrade_application_is_valid(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(upgrade_application_is_valid)(int quick)
{
    int res;

    harness_mock_assert("upgrade_application_is_valid(quick)",
                        &quick);

    harness_mock_read("upgrade_application_is_valid(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_upgrade_binary_upload_begin(int res)
{
    harness_mock_write("upgrade_binary_upload_begin(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(upgrade_binary_upload_begin)()
{
    int res;

    harness_mock_read("upgrade_binary_upload_begin(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_upgrade_binary_upload(const void *buf_p,
                                     size_t size,
                                     int res)
{
    harness_mock_write("upgrade_binary_upload(buf_p)",
                       buf_p,
                       size);

    harness_mock_write("upgrade_binary_upload(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("upgrade_binary_upload(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(upgrade_binary_upload)(const void *buf_p,
                                                       size_t size)
{
    int res;

    harness_mock_assert("upgrade_binary_upload(buf_p)",
                        buf_p);

    harness_mock_assert("upgrade_binary_upload(size)",
                        &size);

    harness_mock_read("upgrade_binary_upload(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_upgrade_binary_upload_end(int res)
{
    harness_mock_write("upgrade_binary_upload_end(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(upgrade_binary_upload_end)()
{
    int res;

    harness_mock_read("upgrade_binary_upload_end(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
