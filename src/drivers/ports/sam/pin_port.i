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

static int pin_port_module_init(void)
{
    /* PMC */
    pmc_peripheral_clock_enable(PERIPHERAL_ID_PIOA);
    pmc_peripheral_clock_enable(PERIPHERAL_ID_PIOB);
    pmc_peripheral_clock_enable(PERIPHERAL_ID_PIOC);
    pmc_peripheral_clock_enable(PERIPHERAL_ID_PIOD);
    pmc_peripheral_clock_enable(PERIPHERAL_ID_PIOE);
    pmc_peripheral_clock_enable(PERIPHERAL_ID_PIOF);

    return (0);
}

static int pin_port_init(struct pin_driver_t *self_p,
                         const struct pin_device_t *dev_p,
                         int mode)
{
    return (pin_set_mode(self_p, mode));
}

static int pin_port_read(struct pin_driver_t *self_p)
{
    return (pin_device_read(self_p->dev_p));
}

static int pin_port_write(struct pin_driver_t *self_p, int value)
{
    const struct pin_device_t *dev_p = self_p->dev_p;

    if (value == 1) {
        pin_device_write_high(dev_p);
    } else {
        pin_device_write_low(dev_p);
    }

    return (0);
}

static int pin_port_toggle(struct pin_driver_t *self_p)
{
    const struct pin_device_t *dev_p = self_p->dev_p;

    if (dev_p->pio_p->ODSR & dev_p->mask) {
        pin_device_write_low(dev_p);
    } else {
        pin_device_write_high(dev_p);
    }

    return (0);
}

static int pin_port_set_mode(struct pin_driver_t *self_p, int mode)
{
    return (pin_device_set_mode(self_p->dev_p, mode));
}
