/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2017, Erik Moqvist
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
#include "xbee_client_mock.h"

int mock_write_xbee_client_module_init(int res)
{
    harness_mock_write("xbee_client_module_init()",
                       NULL,
                       0);

    harness_mock_write("xbee_client_module_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(xbee_client_module_init)()
{
    int res;

    harness_mock_assert("xbee_client_module_init()",
                        NULL,
                        0);

    harness_mock_read("xbee_client_module_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_xbee_client_init(void *chin_p,
                                void *chout_p,
                                void *buf_p,
                                size_t size,
                                int flags,
                                int res)
{
    harness_mock_write("xbee_client_init(chin_p)",
                       chin_p,
                       size);

    harness_mock_write("xbee_client_init(chout_p)",
                       chout_p,
                       size);

    harness_mock_write("xbee_client_init(buf_p)",
                       buf_p,
                       size);

    harness_mock_write("xbee_client_init(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("xbee_client_init(flags)",
                       &flags,
                       sizeof(flags));

    harness_mock_write("xbee_client_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(xbee_client_init)(struct xbee_client_t *self_p,
                                                  void *chin_p,
                                                  void *chout_p,
                                                  void *buf_p,
                                                  size_t size,
                                                  int flags)
{
    int res;

    harness_mock_assert("xbee_client_init(chin_p)",
                        chin_p,
                        size);

    harness_mock_assert("xbee_client_init(chout_p)",
                        chout_p,
                        size);

    harness_mock_assert("xbee_client_init(buf_p)",
                        buf_p,
                        size);

    harness_mock_assert("xbee_client_init(size)",
                        &size,
                        sizeof(size));

    harness_mock_assert("xbee_client_init(flags)",
                        &flags,
                        sizeof(flags));

    harness_mock_read("xbee_client_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_xbee_client_main(void *arg_p,
                                void *res)
{
    harness_mock_write("xbee_client_main(): return (arg_p)",
                       arg_p,
                       sizeof(arg_p));

    harness_mock_write("xbee_client_main(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

void *__attribute__ ((weak)) STUB(xbee_client_main)(void *arg_p)
{
    void *res;

    harness_mock_read("xbee_client_main(): return (arg_p)",
                      arg_p,
                      sizeof(*arg_p));

    harness_mock_read("xbee_client_main(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_xbee_client_read_from(void *buf_p,
                                     size_t size,
                                     struct xbee_client_address_t *address_p,
                                     ssize_t res)
{
    harness_mock_write("xbee_client_read_from(): return (buf_p)",
                       buf_p,
                       size);

    harness_mock_write("xbee_client_read_from(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("xbee_client_read_from(): return (address_p)",
                       address_p,
                       sizeof(*address_p));

    harness_mock_write("xbee_client_read_from(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(xbee_client_read_from)(struct xbee_client_t *self_p,
                                                           void *buf_p,
                                                           size_t size,
                                                           struct xbee_client_address_t *address_p)
{
    ssize_t res;

    harness_mock_read("xbee_client_read_from(): return (buf_p)",
                      buf_p,
                      size);

    harness_mock_assert("xbee_client_read_from(size)",
                        &size,
                        sizeof(size));

    harness_mock_read("xbee_client_read_from(): return (address_p)",
                      address_p,
                      sizeof(*address_p));

    harness_mock_read("xbee_client_read_from(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_xbee_client_write_to(const void *buf_p,
                                    size_t size,
                                    int flags,
                                    struct xbee_client_address_t *address_p,
                                    ssize_t res)
{
    harness_mock_write("xbee_client_write_to(buf_p)",
                       buf_p,
                       size);

    harness_mock_write("xbee_client_write_to(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("xbee_client_write_to(flags)",
                       &flags,
                       sizeof(flags));

    harness_mock_write("xbee_client_write_to(): return (address_p)",
                       address_p,
                       sizeof(*address_p));

    harness_mock_write("xbee_client_write_to(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(xbee_client_write_to)(struct xbee_client_t *self_p,
                                                          const void *buf_p,
                                                          size_t size,
                                                          int flags,
                                                          struct xbee_client_address_t *address_p)
{
    ssize_t res;

    harness_mock_assert("xbee_client_write_to(buf_p)",
                        buf_p,
                        size);

    harness_mock_assert("xbee_client_write_to(size)",
                        &size,
                        sizeof(size));

    harness_mock_assert("xbee_client_write_to(flags)",
                        &flags,
                        sizeof(flags));

    harness_mock_read("xbee_client_write_to(): return (address_p)",
                      address_p,
                      sizeof(*address_p));

    harness_mock_read("xbee_client_write_to(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_xbee_client_pin_set_mode(int pin,
                                        int mode,
                                        int res)
{
    harness_mock_write("xbee_client_pin_set_mode(pin)",
                       &pin,
                       sizeof(pin));

    harness_mock_write("xbee_client_pin_set_mode(mode)",
                       &mode,
                       sizeof(mode));

    harness_mock_write("xbee_client_pin_set_mode(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(xbee_client_pin_set_mode)(struct xbee_client_t *self_p,
                                                          int pin,
                                                          int mode)
{
    int res;

    harness_mock_assert("xbee_client_pin_set_mode(pin)",
                        &pin,
                        sizeof(pin));

    harness_mock_assert("xbee_client_pin_set_mode(mode)",
                        &mode,
                        sizeof(mode));

    harness_mock_read("xbee_client_pin_set_mode(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_xbee_client_pin_read(int pin,
                                    int res)
{
    harness_mock_write("xbee_client_pin_read(pin)",
                       &pin,
                       sizeof(pin));

    harness_mock_write("xbee_client_pin_read(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(xbee_client_pin_read)(struct xbee_client_t *self_p,
                                                      int pin)
{
    int res;

    harness_mock_assert("xbee_client_pin_read(pin)",
                        &pin,
                        sizeof(pin));

    harness_mock_read("xbee_client_pin_read(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_xbee_client_pin_write(int pin,
                                     int value,
                                     int res)
{
    harness_mock_write("xbee_client_pin_write(pin)",
                       &pin,
                       sizeof(pin));

    harness_mock_write("xbee_client_pin_write(value)",
                       &value,
                       sizeof(value));

    harness_mock_write("xbee_client_pin_write(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(xbee_client_pin_write)(struct xbee_client_t *self_p,
                                                       int pin,
                                                       int value)
{
    int res;

    harness_mock_assert("xbee_client_pin_write(pin)",
                        &pin,
                        sizeof(pin));

    harness_mock_assert("xbee_client_pin_write(value)",
                        &value,
                        sizeof(value));

    harness_mock_read("xbee_client_pin_write(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_xbee_client_pin_toggle(int pin,
                                      int res)
{
    harness_mock_write("xbee_client_pin_toggle(pin)",
                       &pin,
                       sizeof(pin));

    harness_mock_write("xbee_client_pin_toggle(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(xbee_client_pin_toggle)(struct xbee_client_t *self_p,
                                                        int pin)
{
    int res;

    harness_mock_assert("xbee_client_pin_toggle(pin)",
                        &pin,
                        sizeof(pin));

    harness_mock_read("xbee_client_pin_toggle(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_xbee_client_pin_convert(int pin,
                                       int res)
{
    harness_mock_write("xbee_client_pin_convert(pin)",
                       &pin,
                       sizeof(pin));

    harness_mock_write("xbee_client_pin_convert(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(xbee_client_pin_convert)(struct xbee_client_t *self_p,
                                                         int pin)
{
    int res;

    harness_mock_assert("xbee_client_pin_convert(pin)",
                        &pin,
                        sizeof(pin));

    harness_mock_read("xbee_client_pin_convert(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_xbee_client_at_command_read(const char *command_p,
                                           uint8_t *parameter_p,
                                           size_t size,
                                           ssize_t res)
{
    harness_mock_write("xbee_client_at_command_read(command_p)",
                       command_p,
                       strlen(command_p) + 1);

    harness_mock_write("xbee_client_at_command_read(): return (parameter_p)",
                       parameter_p,
                       sizeof(*parameter_p));

    harness_mock_write("xbee_client_at_command_read(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("xbee_client_at_command_read(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

ssize_t __attribute__ ((weak)) STUB(xbee_client_at_command_read)(struct xbee_client_t *self_p,
                                                                 const char *command_p,
                                                                 uint8_t *parameter_p,
                                                                 size_t size)
{
    ssize_t res;

    harness_mock_assert("xbee_client_at_command_read(command_p)",
                        command_p,
                        sizeof(*command_p));

    harness_mock_read("xbee_client_at_command_read(): return (parameter_p)",
                      parameter_p,
                      sizeof(*parameter_p));

    harness_mock_assert("xbee_client_at_command_read(size)",
                        &size,
                        sizeof(size));

    harness_mock_read("xbee_client_at_command_read(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_xbee_client_at_command_write(const char *command_p,
                                            const uint8_t *parameter_p,
                                            size_t size,
                                            int res)
{
    harness_mock_write("xbee_client_at_command_write(command_p)",
                       command_p,
                       strlen(command_p) + 1);

    harness_mock_write("xbee_client_at_command_write(parameter_p)",
                       parameter_p,
                       sizeof(*parameter_p));

    harness_mock_write("xbee_client_at_command_write(size)",
                       &size,
                       sizeof(size));

    harness_mock_write("xbee_client_at_command_write(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(xbee_client_at_command_write)(struct xbee_client_t *self_p,
                                                              const char *command_p,
                                                              const uint8_t *parameter_p,
                                                              size_t size)
{
    int res;

    harness_mock_assert("xbee_client_at_command_write(command_p)",
                        command_p,
                        sizeof(*command_p));

    harness_mock_assert("xbee_client_at_command_write(parameter_p)",
                        parameter_p,
                        sizeof(*parameter_p));

    harness_mock_assert("xbee_client_at_command_write(size)",
                        &size,
                        sizeof(size));

    harness_mock_read("xbee_client_at_command_write(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_xbee_client_at_command_read_u8(const char *command_p,
                                              uint8_t *parameter_p,
                                              int res)
{
    harness_mock_write("xbee_client_at_command_read_u8(command_p)",
                       command_p,
                       strlen(command_p) + 1);

    harness_mock_write("xbee_client_at_command_read_u8(): return (parameter_p)",
                       parameter_p,
                       sizeof(*parameter_p));

    harness_mock_write("xbee_client_at_command_read_u8(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(xbee_client_at_command_read_u8)(struct xbee_client_t *self_p,
                                                                const char *command_p,
                                                                uint8_t *parameter_p)
{
    int res;

    harness_mock_assert("xbee_client_at_command_read_u8(command_p)",
                        command_p,
                        sizeof(*command_p));

    harness_mock_read("xbee_client_at_command_read_u8(): return (parameter_p)",
                      parameter_p,
                      sizeof(*parameter_p));

    harness_mock_read("xbee_client_at_command_read_u8(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_xbee_client_at_command_write_u8(const char *command_p,
                                               uint8_t parameter,
                                               int res)
{
    harness_mock_write("xbee_client_at_command_write_u8(command_p)",
                       command_p,
                       strlen(command_p) + 1);

    harness_mock_write("xbee_client_at_command_write_u8(parameter)",
                       &parameter,
                       sizeof(parameter));

    harness_mock_write("xbee_client_at_command_write_u8(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(xbee_client_at_command_write_u8)(struct xbee_client_t *self_p,
                                                                 const char *command_p,
                                                                 uint8_t parameter)
{
    int res;

    harness_mock_assert("xbee_client_at_command_write_u8(command_p)",
                        command_p,
                        sizeof(*command_p));

    harness_mock_assert("xbee_client_at_command_write_u8(parameter)",
                        &parameter,
                        sizeof(parameter));

    harness_mock_read("xbee_client_at_command_write_u8(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_xbee_client_at_command_read_u16(const char *command_p,
                                               uint16_t *parameter_p,
                                               int res)
{
    harness_mock_write("xbee_client_at_command_read_u16(command_p)",
                       command_p,
                       strlen(command_p) + 1);

    harness_mock_write("xbee_client_at_command_read_u16(): return (parameter_p)",
                       parameter_p,
                       sizeof(*parameter_p));

    harness_mock_write("xbee_client_at_command_read_u16(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(xbee_client_at_command_read_u16)(struct xbee_client_t *self_p,
                                                                 const char *command_p,
                                                                 uint16_t *parameter_p)
{
    int res;

    harness_mock_assert("xbee_client_at_command_read_u16(command_p)",
                        command_p,
                        sizeof(*command_p));

    harness_mock_read("xbee_client_at_command_read_u16(): return (parameter_p)",
                      parameter_p,
                      sizeof(*parameter_p));

    harness_mock_read("xbee_client_at_command_read_u16(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_xbee_client_at_command_write_u16(const char *command_p,
                                                uint16_t parameter,
                                                int res)
{
    harness_mock_write("xbee_client_at_command_write_u16(command_p)",
                       command_p,
                       strlen(command_p) + 1);

    harness_mock_write("xbee_client_at_command_write_u16(parameter)",
                       &parameter,
                       sizeof(parameter));

    harness_mock_write("xbee_client_at_command_write_u16(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(xbee_client_at_command_write_u16)(struct xbee_client_t *self_p,
                                                                  const char *command_p,
                                                                  uint16_t parameter)
{
    int res;

    harness_mock_assert("xbee_client_at_command_write_u16(command_p)",
                        command_p,
                        sizeof(*command_p));

    harness_mock_assert("xbee_client_at_command_write_u16(parameter)",
                        &parameter,
                        sizeof(parameter));

    harness_mock_read("xbee_client_at_command_write_u16(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_xbee_client_at_command_read_u32(const char *command_p,
                                               uint32_t *parameter_p,
                                               int res)
{
    harness_mock_write("xbee_client_at_command_read_u32(command_p)",
                       command_p,
                       strlen(command_p) + 1);

    harness_mock_write("xbee_client_at_command_read_u32(): return (parameter_p)",
                       parameter_p,
                       sizeof(*parameter_p));

    harness_mock_write("xbee_client_at_command_read_u32(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(xbee_client_at_command_read_u32)(struct xbee_client_t *self_p,
                                                                 const char *command_p,
                                                                 uint32_t *parameter_p)
{
    int res;

    harness_mock_assert("xbee_client_at_command_read_u32(command_p)",
                        command_p,
                        sizeof(*command_p));

    harness_mock_read("xbee_client_at_command_read_u32(): return (parameter_p)",
                      parameter_p,
                      sizeof(*parameter_p));

    harness_mock_read("xbee_client_at_command_read_u32(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_xbee_client_at_command_write_u32(const char *command_p,
                                                uint32_t parameter,
                                                int res)
{
    harness_mock_write("xbee_client_at_command_write_u32(command_p)",
                       command_p,
                       strlen(command_p) + 1);

    harness_mock_write("xbee_client_at_command_write_u32(parameter)",
                       &parameter,
                       sizeof(parameter));

    harness_mock_write("xbee_client_at_command_write_u32(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(xbee_client_at_command_write_u32)(struct xbee_client_t *self_p,
                                                                  const char *command_p,
                                                                  uint32_t parameter)
{
    int res;

    harness_mock_assert("xbee_client_at_command_write_u32(command_p)",
                        command_p,
                        sizeof(*command_p));

    harness_mock_assert("xbee_client_at_command_write_u32(parameter)",
                        &parameter,
                        sizeof(parameter));

    harness_mock_read("xbee_client_at_command_write_u32(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_xbee_client_print_address(void *chan_p,
                                         struct xbee_client_address_t *address_p,
                                         int res)
{
    harness_mock_write("xbee_client_print_address(chan_p)",
                       chan_p,
                       sizeof(chan_p));

    harness_mock_write("xbee_client_print_address(address_p)",
                       address_p,
                       sizeof(*address_p));

    harness_mock_write("xbee_client_print_address(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(xbee_client_print_address)(void *chan_p,
                                                           struct xbee_client_address_t *address_p)
{
    int res;

    harness_mock_assert("xbee_client_print_address(chan_p)",
                        chan_p,
                        sizeof(*chan_p));

    harness_mock_assert("xbee_client_print_address(address_p)",
                        address_p,
                        sizeof(*address_p));

    harness_mock_read("xbee_client_print_address(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
