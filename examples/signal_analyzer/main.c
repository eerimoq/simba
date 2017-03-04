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

#define SAMPLE_TIMEOUT_IN_MILLISECONDS                      1
#define TIMEOUTS_PER_REPORT                               100

/**
 * Report of a measurement period.
 */
struct report_t {
    uint32_t high_count;
    uint32_t low_count;
    uint32_t rising_count;
};

/**
 * PWM signal measurement pin.
 */
struct pwm_pin_t {
    struct pin_device_t *pin_device_p;
    int previous_value;
    struct report_t report;
};

struct module_t {
    struct fs_command_t cmd_pwm_measure;
    uint32_t timeout_count;
    struct queue_t queue;
    uint8_t buf[128];
    struct pwm_pin_t pwm_pins[8];
};

static struct module_t module = {
    .pwm_pins = {
        { .pin_device_p = &pin_d2_dev },
        { .pin_device_p = &pin_d3_dev },
        { .pin_device_p = &pin_d4_dev },
        { .pin_device_p = &pin_d5_dev },
        { .pin_device_p = &pin_d6_dev },
        { .pin_device_p = &pin_d7_dev },
        { .pin_device_p = &pin_d8_dev },
        { .pin_device_p = &pin_d9_dev }
    }
};

/**
 * PWM measure timer callback. Reads input pin values, stores the data
 * in a report, and writes the report to a queue once the report
 * period is over.
 */
static void sample_timeout(void *arg_p)
{
    int i;
    int value;

    /* Read the pin value and update the report for each pin. */
    for (i = 0; i < membersof(module.pwm_pins); i++) {
        value = pin_device_read(module.pwm_pins[i].pin_device_p);

        if (value == 1) {
            module.pwm_pins[i].report.high_count++;
        } else {
            module.pwm_pins[i].report.low_count++;
        }

        if ((value == 1) && (module.pwm_pins[i].previous_value == 0)) {
            module.pwm_pins[i].report.rising_count++;
        }

        module.pwm_pins[i].previous_value = value;
    }

    module.timeout_count++;

    /* Write the reports to the report queue when the report period is
       over. */
    if ((module.timeout_count % TIMEOUTS_PER_REPORT) == 0) {
        queue_write_isr(&module.queue,
                        &module.timeout_count,
                        sizeof(module.timeout_count));

        for (i = 0; i < membersof(module.pwm_pins); i++) {
            queue_write_isr(&module.queue,
                            &module.pwm_pins[i].report,
                            sizeof(module.pwm_pins[0].report));

            /* Reset for next report period. */
            module.pwm_pins[i].report.high_count = 0;
            module.pwm_pins[i].report.low_count = 0;
            module.pwm_pins[i].report.rising_count = 0;
        }
    }
}

/**
 * File system command to measure duty cycle and frequency of up to
 * eight PWM signals.
 */
static int cmd_pwm_measure_cb(int argc,
                              const char *argv[],
                              void *chout_p,
                              void *chin_p,
                              void *arg_p,
                              void *call_arg_p)
{
    int i, j;
    struct time_t timeout;
    char *delim_p;
    struct report_t reports[8];
    uint32_t time;
    int duty_cycle;
    int frequency;
    long iterations;
    struct timer_t timer;

    if (argc > 2) {
        std_fprintf(chout_p, OSTR("Usage: %s [iterations]\r\n"), argv[0]);

        return (-1);
    }

    /* Iterations argument. */
    if (argc == 2) {
        if (std_strtol(argv[1], &iterations) == NULL) {
            return (-1);
        }
    } else {
        iterations = 1;
    }

    /* Initialization. */
    queue_init(&module.queue, &module.buf[0], sizeof(module.buf));
    module.timeout_count = 0;

    for (i = 0; i < membersof(module.pwm_pins); i++) {
        module.pwm_pins[i].report.high_count = 0;
        module.pwm_pins[i].report.low_count = 0;
        module.pwm_pins[i].report.rising_count = 0;
    }

    timeout.seconds = 0;
    timeout.nanoseconds = 1000000L * SAMPLE_TIMEOUT_IN_MILLISECONDS;

    timer_init(&timer,
               &timeout,
               sample_timeout,
               NULL,
               TIMER_PERIODIC);
    timer_start(&timer);

    /* Wait for reports from the timer callback. */
    for (i = 0; i < iterations; i++) {
        queue_read(&module.queue, &time, sizeof(time));
        queue_read(&module.queue, &reports[0], sizeof(reports));

        std_fprintf(chout_p, OSTR("%lu: ["), time);
        delim_p = "";

        for (j = 0; j < membersof(reports); j++, delim_p = ",") {
            duty_cycle = ((100 * reports[j].high_count) / TIMEOUTS_PER_REPORT);
            frequency = ((1000 * reports[j].rising_count)
                         / (TIMEOUTS_PER_REPORT * SAMPLE_TIMEOUT_IN_MILLISECONDS));
            std_fprintf(chout_p,
                        OSTR("%s(%d,%d)"),
                        delim_p,
                        duty_cycle,
                        frequency);
        }

        std_fprintf(chout_p, OSTR("]\r\n"));
    }

    /* Measurement complete, stop the timer. */
    timer_stop(&timer);

    return (0);
}

int main()
{
    int i;

    sys_start();

    std_printf(sys_get_info());

    /* Initialize the pins as inputs. */
    for (i = 0; i < membersof(module.pwm_pins); i++) {
        pin_device_set_mode(module.pwm_pins[i].pin_device_p, PIN_INPUT);
    }

    fs_command_init(&module.cmd_pwm_measure,
                    CSTR("/pwm/measure"),
                    cmd_pwm_measure_cb,
                    NULL);
    fs_command_register(&module.cmd_pwm_measure);

    thrd_suspend(NULL);

    return (0);
}
