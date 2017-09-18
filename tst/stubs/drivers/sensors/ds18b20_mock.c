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
#include "ds18b20_mock.h"

int mock_write_ds18b20_module_init(int res)
{
    harness_mock_write("ds18b20_module_init()",
                       NULL,
                       0);

    harness_mock_write("ds18b20_module_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(ds18b20_module_init)()
{
    int res;

    harness_mock_assert("ds18b20_module_init()",
                        NULL);

    harness_mock_read("ds18b20_module_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_ds18b20_init(struct owi_driver_t *owi_p,
                            int res)
{
    harness_mock_write("ds18b20_init(owi_p)",
                       owi_p,
                       sizeof(*owi_p));

    harness_mock_write("ds18b20_init(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(ds18b20_init)(struct ds18b20_driver_t *self_p,
                                              struct owi_driver_t *owi_p)
{
    int res;

    harness_mock_assert("ds18b20_init(owi_p)",
                        owi_p);

    harness_mock_read("ds18b20_init(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_ds18b20_convert(int res)
{
    harness_mock_write("ds18b20_convert(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(ds18b20_convert)(struct ds18b20_driver_t *self_p)
{
    int res;

    harness_mock_read("ds18b20_convert(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_ds18b20_read(const uint8_t *id_p,
                            float *temperature_p,
                            int res)
{
    harness_mock_write("ds18b20_read(id_p)",
                       id_p,
                       sizeof(*id_p));

    harness_mock_write("ds18b20_read(): return (temperature_p)",
                       temperature_p,
                       sizeof(*temperature_p));

    harness_mock_write("ds18b20_read(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(ds18b20_read)(struct ds18b20_driver_t *self_p,
                                              const uint8_t *id_p,
                                              float *temperature_p)
{
    int res;

    harness_mock_assert("ds18b20_read(id_p)",
                        id_p);

    harness_mock_read("ds18b20_read(): return (temperature_p)",
                      temperature_p,
                      -1);

    harness_mock_read("ds18b20_read(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_ds18b20_read_fixed_point(const uint8_t *id_p,
                                        int *temperature_p,
                                        int res)
{
    harness_mock_write("ds18b20_read_fixed_point(id_p)",
                       id_p,
                       sizeof(*id_p));

    harness_mock_write("ds18b20_read_fixed_point(): return (temperature_p)",
                       temperature_p,
                       sizeof(*temperature_p));

    harness_mock_write("ds18b20_read_fixed_point(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(ds18b20_read_fixed_point)(struct ds18b20_driver_t *self_p,
                                                          const uint8_t *id_p,
                                                          int *temperature_p)
{
    int res;

    harness_mock_assert("ds18b20_read_fixed_point(id_p)",
                        id_p);

    harness_mock_read("ds18b20_read_fixed_point(): return (temperature_p)",
                      temperature_p,
                      -1);

    harness_mock_read("ds18b20_read_fixed_point(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_ds18b20_read_string(const uint8_t *id_p,
                                   char *temperature_p,
                                   char *res)
{
    harness_mock_write("ds18b20_read_string(id_p)",
                       id_p,
                       sizeof(*id_p));

    harness_mock_write("ds18b20_read_string(): return (temperature_p)",
                       temperature_p,
                       strlen(temperature_p) + 1);

    harness_mock_write("ds18b20_read_string(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

char *__attribute__ ((weak)) STUB(ds18b20_read_string)(struct ds18b20_driver_t *self_p,
                                                       const uint8_t *id_p,
                                                       char *temperature_p)
{
    char *res;

    harness_mock_assert("ds18b20_read_string(id_p)",
                        id_p);

    harness_mock_read("ds18b20_read_string(): return (temperature_p)",
                      temperature_p,
                      -1);

    harness_mock_read("ds18b20_read_string(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_ds18b20_get_temperature(const uint8_t *id_p,
                                       int *temperature_p,
                                       int res)
{
    harness_mock_write("ds18b20_get_temperature(id_p)",
                       id_p,
                       sizeof(*id_p));

    harness_mock_write("ds18b20_get_temperature(): return (temperature_p)",
                       temperature_p,
                       sizeof(*temperature_p));

    harness_mock_write("ds18b20_get_temperature(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

int __attribute__ ((weak)) STUB(ds18b20_get_temperature)(struct ds18b20_driver_t *self_p,
                                                         const uint8_t *id_p,
                                                         int *temperature_p)
{
    int res;

    harness_mock_assert("ds18b20_get_temperature(id_p)",
                        id_p);

    harness_mock_read("ds18b20_get_temperature(): return (temperature_p)",
                      temperature_p,
                      -1);

    harness_mock_read("ds18b20_get_temperature(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_ds18b20_get_temperature_str(const uint8_t *id_p,
                                           char *temperature_p,
                                           char *res)
{
    harness_mock_write("ds18b20_get_temperature_str(id_p)",
                       id_p,
                       sizeof(*id_p));

    harness_mock_write("ds18b20_get_temperature_str(): return (temperature_p)",
                       temperature_p,
                       strlen(temperature_p) + 1);

    harness_mock_write("ds18b20_get_temperature_str(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

char *__attribute__ ((weak)) STUB(ds18b20_get_temperature_str)(struct ds18b20_driver_t *self_p,
                                                               const uint8_t *id_p,
                                                               char *temperature_p)
{
    char *res;

    harness_mock_assert("ds18b20_get_temperature_str(id_p)",
                        id_p);

    harness_mock_read("ds18b20_get_temperature_str(): return (temperature_p)",
                      temperature_p,
                      -1);

    harness_mock_read("ds18b20_get_temperature_str(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
