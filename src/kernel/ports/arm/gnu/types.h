/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2017, Erik Moqvist
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

#ifndef __KERNEL_PORTS_TYPES_H__
#define __KERNEL_PORTS_TYPES_H__

/* System control block. */
struct arm_system_control_block_t {
    uint32_t ACTLR;
    uint32_t reserved1[829];
    uint32_t CPUID;
    uint32_t ICSR;
    uint32_t VTOR;
    uint32_t AIRCR;
    uint32_t SCR;
    uint32_t CCR;
    uint32_t SHPR1;
    uint32_t SHPR2;
    uint32_t SHPR3;
    uint32_t SHCSR;
    uint32_t CFSR;
    uint32_t HFSR;
    uint32_t MMAR;
    uint32_t BFAR;
};

/* Auxiliary Control Register */
#define SCB_ACTLR_DISMCYCINT            BIT(0)
#define SCB_ACTLR_DISDEFWBUF            BIT(1)
#define SCB_ACTLR_DISFOLD               BIT(2)

/* CPUID Base Register */
#define SCB_CPUID_REVISION_POS          (0)
#define SCB_CPUID_REVISION_MASK         (0xf << SCB_CPUID_REVISION_POS)
#define SCB_CPUID_REVISION(value)       BITFIELD_SET(SCB_CPUID_REVISION, value)
#define SCB_CPUID_PARTNO_POS            (4)
#define SCB_CPUID_PARTNO_MASK           (0xfff << SCB_CPUID_PARTNO_POS)
#define SCB_CPUID_PARTNO(value)         BITFIELD_SET(SCB_CPUID_PARTNO, value)
#define SCB_CPUID_CONSTANT_POS          (16)
#define SCB_CPUID_CONSTANT_MASK         (0xf << SCB_CPUID_CONSTANT_POS)
#define SCB_CPUID_CONSTANT(value)       BITFIELD_SET(SCB_CPUID_CONSTANT, value)
#define SCB_CPUID_VARIANT_POS           (20)
#define SCB_CPUID_VARIANT_MASK          (0xf << SCB_CPUID_VARIANT_POS)
#define SCB_CPUID_VARIANT(value)        BITFIELD_SET(SCB_CPUID_VARIANT, value)
#define SCB_CPUID_IMPLEMENTER_POS       (20)
#define SCB_CPUID_IMPLEMENTER_MASK      (0xf << SCB_CPUID_IMPLEMENTER_POS)
#define SCB_CPUID_IMPLEMENTER(value)    BITFIELD_SET(SCB_CPUID_IMPLEMENTER, value)

/* Interrupt Control and State Register */
#define SCB_ICSR_VECTACTIVE_POS         (0)
#define SCB_ICSR_VECTACTIVE_MASK        (0x1ff << SCB_ICSR_VECTACTIVE_POS)
#define SCB_ICSR_VECTACTIVE(value)      BITFIELD_SET(SCB_ICSR_VECTACTIVE, value)
#define SCB_ICSR_RETTOBASE              BIT(11)
#define SCB_ICSR_VECTPENDING_POS        (12)
#define SCB_ICSR_VECTPENDING_MASK       (0x3ff << SCB_ICSR_VECTPENDING_POS)
#define SCB_ICSR_VECTPENDING(value)     BITFIELD_SET(SCB_ICSR_VECTPENDING, value)
#define SCB_ICSR_ISRPENDING             BIT(22)
#define SCB_ICSR_PENDSTCLR              BIT(25)
#define SCB_ICSR_PENDSTSET              BIT(26)
#define SCB_ICSR_PENDSVCLR              BIT(27)
#define SCB_ICSR_PENDSVSET              BIT(28)

/* Vector Table Offset Register */
#define SCB_VTOR_TBLOFF_POS             (7)
#define SCB_VTOR_TBLOFF_MASK            (0x7fffff << SCB_VTOR_TBLOFF_POS)
#define SCB_VTOR_TBLOFF(value)          BITFIELD_SET(SCB_VTOR_TBLOFF, value)

/* Application Interrupt and Reset Control Register */
#define SCB_AIRCR_VECTRESET             BIT(0)
#define SCB_AIRCR_VECTCLRACTIVE         BIT(1)
#define SCB_AIRCR_SYSRESETREQ           BIT(2)
#define SCB_AIRCR_PRIGROUP_POS          (8)
#define SCB_AIRCR_PRIGROUP_MASK         (0x7 << SCB_AIRCR_PRIGROUP_POS)
#define SCB_AIRCR_PRIGROUP(value)       BITFIELD_SET(SCB_AIRCR_PRIGROUP, value)
#define SCB_AIRCR_ENDIANESS             BIT(15)
#define SCB_AIRCR_VECTKEY_POS           (16)
#define SCB_AIRCR_VECTKEY_MASK          (0xffff << SCB_AIRCR_VECTKEY_POS)
#define SCB_AIRCR_VECTKEY(value)        BITFIELD_SET(SCB_AIRCR_VECTKEY, value)

/* System Control Register */
#define SCB_SCR_SLEEONEXIT              BIT(1)
#define SCB_SCR_SLEEPDEEP               BIT(2)
#define SCB_SCR_SEVONPEND               BIT(4)

/* Configuration and Control Register */
#define SCB_CCR_NONEBASETHRDENA         BIT(0)
#define SCB_CCR_USERSETMPEND            BIT(1)
#define SCB_CCR_UNALIGN_TRP             BIT(3)
#define SCB_CCR_DIV_0_TRP               BIT(4)
#define SCB_CCR_BFHFNMIGN               BIT(8)
#define SCB_CCR_STKALIGN                BIT(9)

/* System Handler Priority Register 1 */
#define SCB_SHPR1_PRI_4_POS             (0)
#define SCB_SHPR1_PRI_4_MASK            (0xff <<  SCB_SHPR1_PRI_4_POS)
#define SCB_SHPR1_PRI_4(value)          BITFIELD_SET(SHPR1_PRI_4, value)
#define SCB_SHPR1_PRI_5_POS             (8)
#define SCB_SHPR1_PRI_5_MASK            (0xff << SCB_SHPR1_PRI_5_POS)
#define SCB_SHPR1_PRI_5(value)          BITFIELD_SET(SHPR1_PRI_5, value)
#define SCB_SHPR1_PRI_6_POS             (16)
#define SCB_SHPR1_PRI_6_MASK            (0xff << SCB_SHPR1_PRI_6_POS)
#define SCB_SHPR1_PRI_6(value)          BITFIELD_SET(SHPR1_PRI_6, value)

/* System Handler Priority Register 2 */
#define SCB_SHPR2_PRI_11_POS            (24)
#define SCB_SHPR2_PRI_11_MASK           (0xff << SCB_SHPR2_PRI_11_POS)
#define SCB_SHPR2_PRI_11(value)         BITFIELD_SET(SCB_SHPR2_PRI_11, value)

/* System Handler Priority Register 3 */
#define SCB_SHPR3_PRI_14_POS            (16)
#define SCB_SHPR3_PRI_14_MASK           (0xff << SCB_SHPR3_PRI_14_POS)
#define SCB_SHPR3_PRI_14(value)         BITFIELD_SET(SCB_SHPR3_PRI_14, value)
#define SCB_SHPR3_PRI_15_POS            (24)
#define SCB_SHPR3_PRI_15_MASK           (0xff << SCB_SHPR3_PRI_15_POS)
#define SCB_SHPR3_PRI_15(value)         BITFIELD_SET(SCB_SHPR3_PRI_15, value)

/* System Handler Control and State Register */
#define SCB_SHCSR_MEMFAULTACT           BIT(0)
#define SCB_SHCSR_BUSFAULTACT           BIT(1)
#define SCB_SHCSR_USGFAULTACT           BIT(3)
#define SCB_SHCSR_SVCALLAVCT            BIT(7)
#define SCB_SHCSR_MONITORACT            BIT(8)
#define SCB_SHCSR_PENDSVACT             BIT(10)
#define SCB_SHCSR_SYSTICKACT            BIT(11)
#define SCB_SHCSR_USGFAULTPENDED        BIT(12)
#define SCB_SHCSR_MEMFAULTPENDED        BIT(13)
#define SCB_SHCSR_BUSFAULTPENDED        BIT(14)
#define SCB_SHCSR_SVCALLPENDED          BIT(15)
#define SCB_SHCSR_MEMFAULTENA           BIT(16)
#define SCB_SHCSR_BUSFAULTENA           BIT(17)
#define SCB_SHCSR_USGFAULTENA           BIT(18)

/* Configurable Fault Status Register */
#define SCB_CFSR

/* Hard Fault Status Register */
#define SCB_HFSR

/* Memory Management Fault Address Register */
#define SCB_MMAR

/* Bus Fault Address Register */
#define SCB_BFAR

/* System timer. */
struct arm_system_timer_t {
    uint32_t CTRL;
    uint32_t LOAD;
    uint32_t VAL;
    uint32_t CALIB;
};

/* SysTick Control and Status Register */
#define SYSTEM_TIMER_CTRL_ENABLE        BIT(0)
#define SYSTEM_TIMER_CTRL_TICKINT       BIT(1)
#define SYSTEM_TIMER_CTRL_CLKSOURCE     BIT(2)
#define SYSTEM_TIMER_CTRL_COUNTFLAG     BIT(16)

/* SysTick Reload Value Register */
#define SYSTEM_TIMER_LOAD_RELOAD_POS    (0)
#define SYSTEM_TIMER_LOAD_RELOAD_MASK   (0xffffff << SYSTEM_TIMER_LOAD_RELOAD_POS)
#define SYSTEM_TIMER_LOAD_RELOAD(value) BITFIELD_SET(SYSTEM_TIMER_LOAD_RELOAD, value)

/* SysTick Current Value Register */
#define SYSTEM_TIMER_VAL_CURRENT_POS    (0)
#define SYSTEM_TIMER_VAL_CURRENT_MASK   (0xffffff << SYSTEM_TIMER_VAL_CURRENT_POS)
#define SYSTEM_TIMER_VAL_CURRENT(value) BITFIELD_SET(SYSTEM_TIMER_VAL_CURRENT, value)

    /* SysTick Calibration Value Register */
#define SYSTEM_TIMER_CALIB_TENMS_POS    (0)
#define SYSTEM_TIMER_CALIB_TENMS_MASK   (0xffffff << SYSTEM_TIMER_CALIB_TENMS_POS)
#define SYSTEM_TIMER_CALIB_TENMS(value) BITFIELD_SET(SYSTEM_TIMER_CALIB_TENMS, value)
#define SYSTEM_TIMER_CALIB_SKEW         BIT(30)
#define SYSTEM_TIMER_CALIB_NOREF        BIT(31)

/* System nested vectored interrupt controller. */
struct arm_nvic_t {
    uint32_t ISE[2];
    uint32_t reserved1[30];
    uint32_t ICE[2];
    uint32_t reserved2[30];
    uint32_t ISP[2];
    uint32_t reserved3[30];
    uint32_t ICP[2];
    uint32_t reserved4[30];
    uint32_t IAB[2];
    uint32_t reserved5[62];
    uint32_t IP[8];
    uint32_t reserved6[697];
    uint32_t STIR;
};

#define NVIC_ISER_SET(id)     (ARM_NVIC->ISE[(id) / 32] |= (1 << ((id) % 32)))
#define NVIC_ISER_GET(id)     ((ARM_NVIC->ISE[(id) / 32] >> ((id) % 32)) & 0x1)

#define NVIC_ICER_SET(id)     (ARM_NVIC->ICE[(id) / 32] |= (1 << ((id) % 32)))
#define NVIC_ICER_GET(id)     ((ARM_NVIC->ICE[(id) / 32] >> ((id) % 32)) & 0x1)

#define NVIC_ISPR_SET(id)     (ARM_NVIC->ISP[(id) / 32] |= (1 << ((id) % 32)))
#define NVIC_ISPR_GET(id)     ((ARM_NVIC->ISP[(id) / 32] >> ((id) % 32)) & 0x1)

#define NVIC_ICPR_SET(id)     (ARM_NVIC->ICP[(id) / 32] |= (1 << ((id) % 32)))
#define NVIC_ICPR_GET(id)     ((ARM_NVIC->ICP[(id) / 32] >> ((id) % 32)) & 0x1)

#define NVIC_IABR_GET(id)     ((ARM_NVIC->IAB[(id) / 32] >> ((id) % 32)) & 0x1)

#define NVIC_IP_SET(id, prio) (ARM_NVIC->IP[id] |= (prio))
#define NVIC_IP_GET(id)       (ARM_NVIC->IP[id])

#define NVIC_IP_SET(id, prio) (ARM_NVIC->IP[id] |= (prio))
#define NVIC_IP_GET(id)       (ARM_NVIC->IP[id])

#define NVIC_STIR_INTID(id)   (id)

/* Base addresses of private peripherals. */
#define ARM_SCB        ((volatile struct arm_system_control_block_t *)0xe000e008u)
#define ARM_ST         ((volatile struct arm_system_timer_t         *)0xe000e010u)
#define ARM_NVIC       ((volatile struct arm_nvic_t                 *)0xe000e100u)

static inline void nvic_enable_interrupt(int id)
{
    ARM_NVIC->ICP[id / 32] = (1 << (id % 32));
    ARM_NVIC->ISE[id / 32] = (1 << (id % 32));
}

static inline void nvic_disable_interrupt(int id)
{
    ARM_NVIC->ICE[id / 32] = (1 << (id % 32));
    ARM_NVIC->ICP[id / 32] = (1 << (id % 32));
}

#endif
