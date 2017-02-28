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

#include <avr/interrupt.h>

#define PIN(sfr) ((sfr) + 0)
#define DDR(sfr) ((sfr) + 1)
#define PORT(sfr) ((sfr) + 2)

#define EXTI_ISR(number)                                                \
    ISR(INT ## number ## _vect)                                         \
    {                                                                   \
        struct exti_driver_t *self_p = exti_device[number].drv_p;       \
                                                                        \
        if (self_p != NULL) {                                           \
            self_p->on_interrupt(self_p->arg_p);                        \
        }                                                               \
    }

#if (EXTI_DEVICE_MAX >= 1)
EXTI_ISR(0);
#endif
#if (EXTI_DEVICE_MAX >= 2)
EXTI_ISR(1);
#endif
#if (EXTI_DEVICE_MAX >= 3)
EXTI_ISR(2);
#endif
#if (EXTI_DEVICE_MAX >= 4)
EXTI_ISR(3);
#endif
#if (EXTI_DEVICE_MAX >= 5)
EXTI_ISR(4);
#endif
#if (EXTI_DEVICE_MAX >= 6)
EXTI_ISR(5);
#endif

static int exti_port_module_init()
{
    return (0);
}

static int exti_port_start(struct exti_driver_t *self_p)
{
    struct exti_device_t *dev_p = self_p->dev_p;

    dev_p->drv_p = self_p;
    *DDR(dev_p->pin_p->sfr_p) &= ~(dev_p->pin_p->mask);

    if (dev_p->id < 3) {
        EICRA |= (self_p->trigger << (2 * dev_p->id));
    } else {
#if (EXTI_DEVICE_MAX > 2)
        EICRB |= (self_p->trigger << (2 * (dev_p->id - 4)));
#endif
    }

    EIFR = _BV(dev_p->id);
    EIMSK |= _BV(dev_p->id);

    return (0);
}

static int exti_port_stop(struct exti_driver_t *self_p)
{
    struct exti_device_t *dev_p = self_p->dev_p;

    EIMSK &= ~_BV(dev_p->id);
    self_p->dev_p = NULL;

    return (0);
}

static int exti_port_clear(struct exti_driver_t *self_p)
{
    /* Clear the interrupt flag. */
    EIFR = _BV(self_p->dev_p->id);

    return (0);
}
