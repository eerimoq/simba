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

static struct pin_driver_t pin;

static int test_device_input_output(struct harness_t *harness_p)
{
    BTASSERT(pin_device_set_mode(&pin_led_dev, PIN_OUTPUT) == 0);

    /* Write high. */
    BTASSERT(pin_device_write_high(&pin_led_dev) == 0);
    BTASSERT(pin_device_read(&pin_led_dev) == 1);

    /* Write low. */
    BTASSERT(pin_device_write_low(&pin_led_dev) == 0);
    BTASSERT(pin_device_read(&pin_led_dev) == 0);

    return (0);
}

static int test_input_output(struct harness_t *harness_p)
{
    BTASSERT(pin_init(&pin, &pin_led_dev, PIN_OUTPUT) == 0);

    /* Write high. */
    BTASSERT(pin_write(&pin, 1) == 0);
    BTASSERT(pin_read(&pin) == 1);

    /* Write low. */
    BTASSERT(pin_write(&pin, 0) == 0);
    BTASSERT(pin_read(&pin) == 0);

    /* Toggle. */
    BTASSERT(pin_toggle(&pin) == 0);
    BTASSERT(pin_read(&pin) == 1);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_device_input_output, "test_device_input_output" },
        { test_input_output, "test_input_output" },
        { NULL, NULL }
    };

    sys_start();
    pin_module_init();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
