/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2018, Erik Moqvist
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

#include <avr/interrupt.h>

/**
 * Interrupt handler for external change interrupts.
 */
#define PCINT_ISR(number, levels)               \
    ISR(PCINT ## number ## _vect)               \
    {                                           \
        isr(number, levels, PCMSK ## number);   \
    }

/**
 * Change interrupt handler.
 */
static void isr(int number,
                uint8_t levels,
                uint8_t pcmsk)
{
    int i;
    int first_device_index;
    uint8_t changed;
    uint8_t edge;
    struct pcint_driver_t *drv_p;
    
    changed = (levels ^ module.port.levels[number]);
    changed &= pcmsk;
    module.port.levels[number] = levels;
    first_device_index = (8 * number);

    /* Iterate over the 8 pins and call the callback for those with
       changed level and matching configured trigger. */
    for (i = 0; i < 8; i++) {
        if (changed & 0x1) {
            drv_p = pcint_device[first_device_index + i].drv_p;
            edge = (levels & 0x1);

            if ((drv_p->trigger == PCINT_TRIGGER_BOTH_EDGES)
                || (drv_p->trigger == edge)) {
                drv_p->on_interrupt(drv_p->arg_p);
            }
        }

        levels >>= 1;
        changed >>= 1;
    }
}

#if defined(MCU_ATMEGA328P)
PCINT_ISR(0, PIND);
PCINT_ISR(1, PINB);
PCINT_ISR(2, PINC);
#endif

#if defined(MCU_ATMEGA2560)
PCINT_ISR(0, PINB);
PCINT_ISR(1, PINJ);
PCINT_ISR(2, PINK);
#endif

static struct pin_device_t *device_index_to_pin(int index)
{
    struct pin_device_t *pin_p;
    
#if defined(MCU_ATMEGA328P)
    pin_p = &pin_device[index];
#elif defined(MCU_ATMEGA2560)
    if ((index >= 0) && (index <= 7)) {
        pin_p = &pin_device[8 - index];
    } else if ((index >= 8) && (index <= 15)) {
        pin_p = &pin_device[62 - index - 8];
    } else if ((index >= 16) && (index <= 23)) {
        pin_p = &pin_device[70 + index - 16];
    } else {
        pin_p = NULL;
    }
#else
#    error "MCU not supported."
#endif

    return (pin_p);
}

static int pcint_port_module_init()
{
    PCICR = _BV(PCIE2) | _BV(PCIE1) | _BV(PCIE0);

    return (0);
}

static int pcint_port_init(struct pcint_driver_t *self_p)
{
    struct pcint_device_t *dev_p;
    struct pin_device_t *pin_p;
    int index;

    dev_p = self_p->dev_p;
    index = indexof(dev_p, pcint_device);
    pin_p = device_index_to_pin(index);

    if (pin_p == NULL) {
        return (-ENODEV);
    }
    
    sys_lock();
    module.port.levels[index / 8] |= (*PIN(pin_p->sfr_p) & pin_p->mask);
    self_p->pcmsk_p = (volatile uint8_t *)(_SFR_MEM_ADDR(PCMSK0) + (index / 8));
    sys_unlock();
    
    return (0);
}

static int pcint_port_start(struct pcint_driver_t *self_p)
{
    struct pcint_device_t *dev_p;
    struct pin_device_t *pin_p;
    int index;
    
    dev_p = self_p->dev_p;
    index = indexof(dev_p, pcint_device);
    pin_p = device_index_to_pin(index);

    sys_lock();
    pin_device_set_mode(pin_p, PIN_INPUT);
    module.port.levels[index / 8] |= (*PIN(pin_p->sfr_p) & pin_p->mask);
    *self_p->pcmsk_p |= pin_p->mask;
    dev_p->drv_p = self_p;
    sys_unlock();

    return (0);
}

static int pcint_port_stop(struct pcint_driver_t *self_p)
{
    struct pcint_device_t *dev_p;
    struct pin_device_t *pin_p;
    int index;

    dev_p = self_p->dev_p;
    index = indexof(dev_p, pcint_device);
    pin_p = device_index_to_pin(index);

    sys_lock();
    module.port.levels[index / 8] &= ~pin_p->mask;
    *self_p->pcmsk_p &= ~pin_p->mask;
    dev_p->drv_p = NULL;
    sys_unlock();

    return (0);
}
