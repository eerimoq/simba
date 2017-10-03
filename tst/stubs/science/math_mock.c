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
#include "math_mock.h"

int mock_write_math_radians_to_degrees(float value,
                                       float res)
{
    harness_mock_write("math_radians_to_degrees(value)",
                       &value,
                       sizeof(value));

    harness_mock_write("math_radians_to_degrees(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

float __attribute__ ((weak)) STUB(math_radians_to_degrees)(float value)
{
    float res;

    harness_mock_assert("math_radians_to_degrees(value)",
                        &value,
                        sizeof(value));

    harness_mock_read("math_radians_to_degrees(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}

int mock_write_math_degrees_to_radians(float value,
                                       float res)
{
    harness_mock_write("math_degrees_to_radians(value)",
                       &value,
                       sizeof(value));

    harness_mock_write("math_degrees_to_radians(): return (res)",
                       &res,
                       sizeof(res));

    return (0);
}

float __attribute__ ((weak)) STUB(math_degrees_to_radians)(float value)
{
    float res;

    harness_mock_assert("math_degrees_to_radians(value)",
                        &value,
                        sizeof(value));

    harness_mock_read("math_degrees_to_radians(): return (res)",
                      &res,
                      sizeof(res));

    return (res);
}
