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

#ifndef __DRIVERS_DAC_PORT_H__
#define __DRIVERS_DAC_PORT_H__

struct dac_driver_t;

struct dac_device_t {
    volatile struct sam_dacc_t *regs_p;
    int id;
    struct {
        struct dac_driver_t *head_p;
        struct dac_driver_t *tail_p;
    } jobs;
    struct {
        volatile struct sam_tc_t *regs_p;
        int channel;
        int id;
    } tc;
};

struct dac_driver_t {
    struct dac_device_t *dev_p;
    struct pin_driver_t pin_drv;
    int chxr;
    struct {
        const uint32_t *samples_p;
        size_t length;
    } next;
    struct thrd_t *thrd_p;
    int state;
    struct dac_driver_t *next_p;
};

#endif
