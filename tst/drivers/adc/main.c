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

struct pin_driver_t pin;
struct adc_driver_t adc;

static int test_init(struct harness_t *harness_p)
{
    BTASSERT(adc_init(&adc,
                      &adc_0_dev,
                      &pin_a0_dev,
                      ADC_REFERENCE_VCC,
                      10) == 0);

    BTASSERT(pin_init(&pin, &pin_d2_dev, PIN_OUTPUT) == 0);
    BTASSERT(pin_write(&pin, 1) == 0);

    return (0);
}

static int test_convert(struct harness_t *harness_p)
{
    int i;
    uint16_t samples[16];

    memset(samples, 0, sizeof(samples));

    BTASSERT(adc_convert(&adc, samples, membersof(samples)) == 0);

    for (i = 0; i < membersof(samples); i++) {
        std_printf(FSTR("[%d]: %d\r\n"), i, samples[i]);
        BTASSERT(samples[i] > 500);
    }

    return (0);
}

static int test_convert_isr(struct harness_t *harness_p)
{
    uint16_t sample;

    /* Low value. */
    sample = 0xffff;
    BTASSERT(pin_write(&pin, 0) == 0);
    time_busy_wait_us(10);
    sys_lock();
    
    if (adc_convert_isr(&adc, &sample) != 0) {
        sys_unlock();
        
        return (-1);
    }
    
    sys_unlock();
    std_printf(FSTR("low sample = %d\r\n"), sample);
    BTASSERT(sample < 300);

    /* High value. */
    sample = 0x0000;
    BTASSERT(pin_write(&pin, 1) == 0);
    time_busy_wait_us(10);
    sys_lock();
    
    if (adc_convert_isr(&adc, &sample) != 0) {
        sys_unlock();
        
        return (-1);
    }
    
    sys_unlock();
    std_printf(FSTR("high sample = %d\r\n"), sample);
    BTASSERT(sample > 500);
    
    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_init, "test_init" },
        { test_convert, "test_convert" },
        { test_convert_isr, "test_convert_isr" },
        { NULL, NULL }
    };

    sys_start();
    adc_module_init();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
