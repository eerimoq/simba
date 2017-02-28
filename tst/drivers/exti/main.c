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

static volatile int flag = 0;

static void isr(void *arg_p)
{
    flag++;
}

int test_exti(struct harness_t *harness_p)
{
    int i;
    struct exti_driver_t exti;
    struct pin_driver_t pin;

    pin_init(&pin, &pin_d4_dev, PIN_OUTPUT);
    pin_write(&pin, 1);
    
    BTASSERT(exti_init(&exti,
                       &exti_d3_dev,
                       EXTI_TRIGGER_FALLING_EDGE,
                       isr,
                       NULL) == 0);
    BTASSERT(exti_start(&exti) == 0);

    for (i = 0; i < 10; i++) {
        pin_write(&pin, 0);
        time_busy_wait_us(10000);
        pin_write(&pin, 1);
        time_busy_wait_us(10000);
    }

    std_printf(FSTR("flag = %d\r\n"), (int)flag);
    BTASSERT(flag == 10);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_exti, "test_exti" },
        { NULL, NULL }
    };

    sys_start();
    exti_module_init();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
