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

#ifndef __DRIVERS_TYPES_H__
#define __DRIVERS_TYPES_H__

/**
 * Prologue of the software PWM interrupt handler.
 */
#define PWM_SOFT_ISR_PROLOGUE                                           \
    struct pwm_soft_driver_t *elem_p;                                   \
                                                                        \
    /* Set the next timeout timer count early for better accuracy. */   \
    elem_p = module.current_p->next_p;                                  \
                                                                        \
    while (elem_p->delta == 0) {                                        \
        elem_p = elem_p->next_p;                                        \
    }

/**
 * Epilogue of the software PWM interrupt handler.
 */
#define PWM_SOFT_ISR_EPILOGUE                                   \
    /* Update pin values. */                                    \
    elem_p = module.current_p;                                  \
                                                                \
    if (elem_p != &module.tail_pwm_soft) {                      \
        do {                                                    \
            pin_device_write_low(elem_p->pin_dev_p);            \
            elem_p = elem_p->next_p;                            \
        } while (elem_p->delta == 0);                           \
    } else {                                                    \
        /* One period has elapsed. Set all PWM:s high. */       \
        elem_p = module.head_p;                                 \
                                                                \
        while (elem_p != &module.tail_pwm_soft) {               \
            pin_device_write_high(elem_p->pin_dev_p);           \
            elem_p = elem_p->next_p;                            \
        }                                                       \
                                                                \
        elem_p = module.tail_pwm_soft.next_p;                   \
    }                                                           \
                                                                \
    /* Unlink stopped elements. */                              \
    if (module.current_p->thrd_p != NULL) {                     \
        unlink_pwm_soft_isr(module.current_p);                  \
        thrd_resume_isr(module.current_p->thrd_p, 0);           \
        module.current_p->thrd_p = NULL;                        \
    }                                                           \
                                                                \
    /* Save current element. */                                 \
    module.current_p = elem_p;

#endif
