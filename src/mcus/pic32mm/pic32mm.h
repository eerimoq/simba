/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2018, Erik Moqvist
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

#ifndef __PIC32MM_H__
#define __PIC32MM_H__

struct pic32mm_reg_t {
    uint32_t VAL;
    uint32_t CLR;
    uint32_t SET;
    uint32_t INV;
};

/* 5. Flash. */
struct pic32mm_flash_t {
    struct pic32mm_reg_t NVMCON;
    struct pic32mm_reg_t NVMKEY;
    struct pic32mm_reg_t NVMADDR;
    struct pic32mm_reg_t NVMDATA0;
    struct pic32mm_reg_t NVMDATA1;
    struct pic32mm_reg_t NVMSRCADDR;
    struct pic32mm_reg_t NVMPWP;
    struct pic32mm_reg_t NVMBWP;
};

#define PIC32MM_FLASH_NVMCON_WR                              BIT(15)
#define PIC32MM_FLASH_NVMCON_WREN                            BIT(14)
#define PIC32MM_FLASH_NVMCON_WRERR                           BIT(13)
#define PIC32MM_FLASH_NVMCON_LVDERR                          BIT(12)
#define PIC32MM_FLASH_NVMCON_NVMOP_POS                       (0)
#define PIC32MM_FLASH_NVMCON_NVMOP_MASK         \
    (0xf << PIC32MM_FLASH_NVMCON_NVMOP_POS)
#define PIC32MM_FLASH_NVMCON_NVMOP(value)               \
    BITFIELD_SET(PIC32MM_FLASH_NVMCON_NVMOP, value)
#define PIC32MM_FLASH_NVMCON_NVMOP_PROGRAM_ERASE        \
    PIC32MM_FLASH_NVMCON_NVMOP(7)
#define PIC32MM_FLASH_NVMCON_NVMOP_PAGE_ERASE   \
    PIC32MM_FLASH_NVMCON_NVMOP(4)
#define PIC32MM_FLASH_NVMCON_NVMOP_ROW_PROGRAM  \
    PIC32MM_FLASH_NVMCON_NVMOP(3)
#define PIC32MM_FLASH_NVMCON_NVMOP_DWORD_PROGRAM        \
    PIC32MM_FLASH_NVMCON_NVMOP(2)
#define PIC32MM_FLASH_NVMCON_NVMOP_NOP          \
    PIC32MM_FLASH_NVMCON_NVMOP(0)

/* 8. Interrupts. */
struct pic32mm_int_t {
    struct pic32mm_reg_t INTCON;
    struct pic32mm_reg_t PRISS;
    struct pic32mm_reg_t INTSTAT;
    struct pic32mm_reg_t IPTMR;
    struct pic32mm_reg_t IFS[4];
    uint32_t RESERVED0[16];
    struct pic32mm_reg_t IEC[4];
    uint32_t RESERVED1[16];
    struct pic32mm_reg_t IPC[26];
};

/* 9. Oscillator. */
struct pic32mm_osc_t {
    struct pic32mm_reg_t OSCCON;
    uint32_t RESERVED0[4];
    struct pic32mm_reg_t SPLLCON;
    uint32_t RESERVED1[24];
    struct pic32mm_reg_t REFO1CON;
    struct pic32mm_reg_t REFO1TRIM;
    uint32_t RESERVED2[12];
    struct pic32mm_reg_t CLKSTAT;
    struct pic32mm_reg_t OSCTUN;
};

#define PIC32MM_OSC_OSCCON_NOSC_POS                          (8)
#define PIC32MM_OSC_OSCCON_NOSC_MASK            \
    (0x7 << PIC32MM_OSC_OSCCON_NOSC_POS)
#define PIC32MM_OSC_OSCCON_NOSC(value)                  \
    BITFIELD_SET(PIC32MM_OSC_OSCCON_NOSC, value)
#define PIC32MM_OSC_OSCCON_NOSC_LPRC            \
    PIC32MM_OSC_OSCCON_NOSC(5)
#define PIC32MM_OSC_OSCCON_NOSC_SOSC            \
    PIC32MM_OSC_OSCCON_NOSC(4)
#define PIC32MM_OSC_OSCCON_NOSC_POSC            \
    PIC32MM_OSC_OSCCON_NOSC(2)
#define PIC32MM_OSC_OSCCON_NOSC_SPLL            \
    PIC32MM_OSC_OSCCON_NOSC(1)
#define PIC32MM_OSC_OSCCON_NOSC_FRC             \
    PIC32MM_OSC_OSCCON_NOSC(0)
#define PIC32MM_OSC_OSCCON_OSWEN                             BIT(0)

#define PIC32MM_OSC_SPLLCON_PLLODIV_POS                      (24)
#define PIC32MM_OSC_SPLLCON_PLLODIV_MASK        \
    (0x7 << PIC32MM_OSC_SPLLCON_PLLODIV_POS)
#define PIC32MM_OSC_SPLLCON_PLLODIV(value)              \
    BITFIELD_SET(PIC32MM_OSC_SPLLCON_PLLODIV, value)
#define PIC32MM_OSC_SPLLCON_PLLMULT_POS                      (16)
#define PIC32MM_OSC_SPLLCON_PLLMULT_MASK        \
    (0x7f << PIC32MM_OSC_SPLLCON_PLLMULT_POS)
#define PIC32MM_OSC_SPLLCON_PLLMULT(value)              \
    BITFIELD_SET(PIC32MM_OSC_SPLLCON_PLLMULT, value)
#define PIC32MM_OSC_SPLLCON_PLLMULT_3           \
    PIC32MM_OSC_SPLLCON_PLLMULT(1)
#define PIC32MM_OSC_SPLLCON_PLLICLK                          BIT(7)
    
/* 10. IO. */
struct pic32mm_port_t {
    struct pic32mm_reg_t ANSEL;
    struct pic32mm_reg_t TRIS;
    struct pic32mm_reg_t PORT;
    struct pic32mm_reg_t LAT;
    struct pic32mm_reg_t ODC;
    struct pic32mm_reg_t CNPU;
    struct pic32mm_reg_t CNPD;
    struct pic32mm_reg_t CNCON;
    struct pic32mm_reg_t CNEN0;
    struct pic32mm_reg_t CNSTAT;
    struct pic32mm_reg_t CNEN1;
    struct pic32mm_reg_t CNF;
};

struct pic32mm_pinsel_t {
    uint32_t RPCON;
    uint32_t RESERVED0[7];
    struct pic32mm_reg_t RPINR[12];
    uint32_t RESERVED1[12];
    struct pic32mm_reg_t RPOR[6];
};

/* 17. UART. */
struct pic32mm_uart_t {
    struct pic32mm_reg_t MODE;
    struct pic32mm_reg_t STA;
    struct pic32mm_reg_t TXREG;
    struct pic32mm_reg_t RXREG;
    struct pic32mm_reg_t BRG;
};

#define PIC32MM_UART_MODE_SPLEN                              BIT(23)
#define PIC32MM_UART_MODE_ACTIVE                             BIT(22)
#define PIC32MM_UART_MODE_CLKSEL_POS                         (17)
#define PIC32MM_UART_MODE_CLKSEL_MASK           \
    (0x3 << PIC32MM_UART_MODE_CLKSEL_POS)
#define PIC32MM_UART_MODE_CLKSEL(value)                 \
    BITFIELD_SET(PIC32MM_UART_MODE_CLKSEL, value)
#define PIC32MM_UART_MODE_CLKSEL_PBCLK          \
    PIC32MM_UART_MODE_CLKSEL_PBCLK(0)
#define PIC32MM_UART_MODE_CLKSEL_SYSCLK         \
    PIC32MM_UART_MODE_CLKSEL_PBCLK(1)
#define PIC32MM_UART_MODE_CLKSEL_FRC            \
    PIC32MM_UART_MODE_CLKSEL_PBCLK(2)
#define PIC32MM_UART_MODE_CLKSEL_REFO1          \
    PIC32MM_UART_MODE_CLKSEL_PBCLK(3)
#define PIC32MM_UART_MODE_OVFDIS                             BIT(16)
#define PIC32MM_UART_MODE_ON                                 BIT(15)
#define PIC32MM_UART_MODE_SIDL                               BIT(13)
#define PIC32MM_UART_MODE_IREN                               BIT(12)
#define PIC32MM_UART_MODE_RTSMD                              BIT(11)
#define PIC32MM_UART_MODE_UEN_POS                            (8)
#define PIC32MM_UART_MODE_UEN_MASK              \
    (0x3 << PIC32MM_UART_MODE_UEN_POS)
#define PIC32MM_UART_MODE_UEN(value)            \
    BITFIELD_SET(PIC32MM_UART_MODE_UEN, value)
#define PIC32MM_UART_MODE_WAKE                               BIT(7)
#define PIC32MM_UART_MODE_LPBACK                             BIT(6)
#define PIC32MM_UART_MODE_ABAUD                              BIT(5)
#define PIC32MM_UART_MODE_RXINV                              BIT(4)
#define PIC32MM_UART_MODE_BRGH                               BIT(3)
#define PIC32MM_UART_MODE_PDSEL_POS                          (1)
#define PIC32MM_UART_MODE_PDSEL_MASK            \
    (0x3 << PIC32MM_UART_MODE_PDSEL_POS)
#define PIC32MM_UART_MODE_PDSEL(value)                  \
    BITFIELD_SET(PIC32MM_UART_MODE_PDSEL, value)
#define PIC32MM_UART_MODE_STSEL                              BIT(0)

#define PIC32MM_UART_STA_MASK_POS                            (24)
#define PIC32MM_UART_STA_MASK_MASK              \
    (0xff << PIC32MM_UART_STA_MASK_POS)
#define PIC32MM_UART_STA_MASK(value)            \
    BITFIELD_SET(PIC32MM_UART_STA_MASK, value)
#define PIC32MM_UART_STA_ADDR_POS                            (16)
#define PIC32MM_UART_STA_ADDR_MASK              \
    (0xff << PIC32MM_UART_STA_ADDR_POS)
#define PIC32MM_UART_STA_ADDR(value)            \
    BITFIELD_SET(PIC32MM_UART_STA_ADDR, value)
#define PIC32MM_UART_STA_UTXISEL_POS                         (14)
#define PIC32MM_UART_STA_UTXISEL_MASK           \
    (0x3 << PIC32MM_UART_STA_UTXISEL_POS)
#define PIC32MM_UART_STA_UTXISEL(value)                 \
    BITFIELD_SET(PIC32MM_UART_STA_UTXISEL, value)
#define PIC32MM_UART_STA_UTXINV                              BIT(13)
#define PIC32MM_UART_STA_URXEN                               BIT(12)
#define PIC32MM_UART_STA_UTXBRK                              BIT(11)
#define PIC32MM_UART_STA_UTXEN                               BIT(10)
#define PIC32MM_UART_STA_UTXBF                               BIT(9)
#define PIC32MM_UART_STA_TRMT                                BIT(8)
#define PIC32MM_UART_STA_URXISEL_POS                         (6)
#define PIC32MM_UART_STA_URXISEL_MASK           \
    (0x3 << PIC32MM_UART_STA_URXISEL_POS)
#define PIC32MM_UART_STA_URXISEL(value)                 \
    BITFIELD_SET(PIC32MM_UART_STA_URXISEL, value)
#define PIC32MM_UART_STA_ADDEN                               BIT(5)
#define PIC32MM_UART_STA_RIDLE                               BIT(4)
#define PIC32MM_UART_STA_PERR                                BIT(3)
#define PIC32MM_UART_STA_FERR                                BIT(2)
#define PIC32MM_UART_STA_OERR                                BIT(1)
#define PIC32MM_UART_STA_URXDA                               BIT(0)

/* 26. Special features. */

/* Configuration bits. */
struct pic32mm_conf_t {
    uint32_t RESERVED0;
    uint32_t FDEVOPT;
    uint32_t FICD;
    uint32_t FPOR;
    uint32_t FWDT;
    uint32_t FOSCSEL;
    uint32_t FSEC;
};

#define PIC32MM_CONF_FDEVOPT_RESERVED                        0x00003fe7
#define PIC32MM_CONF_FDEVOPT_USERID_POS                      (16)
#define PIC32MM_CONF_FDEVOPT_USERID_MASK        \
    (0xffff << PIC32MM_CONF_FDEVOPT_USERID_POS)
#define PIC32MM_CONF_FDEVOPT_USERID(value)              \
    BITFIELD_SET(PIC32MM_CONF_FDEVOPT_USERID, value)
#define PIC32MM_CONF_FDEVOPT_FVBUSIO                         BIT(15)
#define PIC32MM_CONF_FDEVOPT_FUSBIDIO                        BIT(14)
#define PIC32MM_CONF_FDEVOPT_ALTI2C                          BIT(4)
#define PIC32MM_CONF_FDEVOPT_SOSCHP                          BIT(3)

#define PIC32MM_CONF_FICD_RESERVED                           0xffffffe3
#define PIC32MM_CONF_FICD_ICS_POS                            (3)
#define PIC32MM_CONF_FICD_ICS_MASK              \
    (0x3 << PIC32MM_CONF_FICD_ICS_POS)
#define PIC32MM_CONF_FICD_ICS(value)            \
    BITFIELD_SET(PIC32MM_CONF_FICD_ICS, value)
#define PIC32MM_CONF_FICD_JTAGEN                             BIT(2)

#define PIC32MM_CONF_FPOR_RESERVED                           0xfffffff0
#define PIC32MM_CONF_FPOR_LPBOREN                            BIT(3)
#define PIC32MM_CONF_FPOR_RETVR                              BIT(2)
#define PIC32MM_CONF_FPOR_BOREN_POS                          (0)
#define PIC32MM_CONF_FPOR_BOREN_MASK            \
    (0x3 << PIC32MM_CONF_FPOR_BOREN_POS)
#define PIC32MM_CONF_FPOR_BOREN(value)                  \
    BITFIELD_SET(PIC32MM_CONF_FPOR_BOREN, value)

#define PIC32MM_CONF_FWDT_RESERVED                           0xffff0000
#define PIC32MM_CONF_FWDT_FWDTEN                             BIT(15)
#define PIC32MM_CONF_FWDT_RCLKSEL_POS                        (13)
#define PIC32MM_CONF_FWDT_RCLKSEL_MASK          \
    (0x3 << PIC32MM_CONF_FWDT_RCLKSEL_POS)
#define PIC32MM_CONF_FWDT_RCLKSEL(value)                \
    BITFIELD_SET(PIC32MM_CONF_FWDT_RCLKSEL, value)
#define PIC32MM_CONF_FWDT_RWDTPS_POS                         (8)
#define PIC32MM_CONF_FWDT_RWDTPS_MASK           \
    (0x1f << PIC32MM_CONF_FWDT_RWDTPS_POS)
#define PIC32MM_CONF_FWDT_RWDTPS(value)                 \
    BITFIELD_SET(PIC32MM_CONF_FWDT_RWDTPS, value)
#define PIC32MM_CONF_FWDT_WINDIS                             BIT(7)
#define PIC32MM_CONF_FWDT_FWDTWINSZ_POS                      (5)
#define PIC32MM_CONF_FWDT_FWDTWINSZ_MASK        \
    (0x3 << PIC32MM_CONF_FWDT_FWDTWINSZ_POS)
#define PIC32MM_CONF_FWDT_FWDTWINSZ(value)              \
    BITFIELD_SET(PIC32MM_CONF_FWDT_FWDTWINSZ, value)
#define PIC32MM_CONF_FWDT_SWDTPS_POS                         (0)
#define PIC32MM_CONF_FWDT_SWDTPS_MASK           \
    (0x1f << PIC32MM_CONF_FWDT_SWDTPS_POS)
#define PIC32MM_CONF_FWDT_SWDTPS(value)                 \
    BITFIELD_SET(PIC32MM_CONF_FWDT_SWDTPS, value)

#define PIC32MM_CONF_FOSCSEL_RESERVED                        0xffff2828
#define PIC32MM_CONF_FOSCSEL_FCKSM_POS                       (14)
#define PIC32MM_CONF_FOSCSEL_FCKSM_MASK         \
    (0x3 << PIC32MM_CONF_FOSCSEL_FCKSM_POS)
#define PIC32MM_CONF_FOSCSEL_FCKSM(value)               \
    BITFIELD_SET(PIC32MM_CONF_FOSCSEL_FCKSM, value)
#define PIC32MM_CONF_FOSCSEL_SOSCSEL                         BIT(12)
#define PIC32MM_CONF_FOSCSEL_OSCIOFNC                        BIT(10)
#define PIC32MM_CONF_FOSCSEL_POSCMOD_POS                     (8)
#define PIC32MM_CONF_FOSCSEL_POSCMOD_MASK       \
    (0x3 << PIC32MM_CONF_FOSCSEL_POSCMOD_POS)
#define PIC32MM_CONF_FOSCSEL_POSCMOD(value)             \
    BITFIELD_SET(PIC32MM_CONF_FOSCSEL_POSCMOD, value)
#define PIC32MM_CONF_FOSCSEL_IESO                            BIT(7)
#define PIC32MM_CONF_FOSCSEL_SOSCEN                          BIT(6)
#define PIC32MM_CONF_FOSCSEL_PLLSRC                          BIT(4)
#define PIC32MM_CONF_FOSCSEL_FNOSC_POS                       (0)
#define PIC32MM_CONF_FOSCSEL_FNOSC_MASK         \
    (0x7 << PIC32MM_CONF_FOSCSEL_FNOSC_POS)
#define PIC32MM_CONF_FOSCSEL_FNOSC(value)               \
    BITFIELD_SET(PIC32MM_CONF_FOSCSEL_FNOSC, value)

#define PIC32MM_CONF_FSEC_RESERVED                           0x7fffffff
#define PIC32MM_CONF_FSEC_CP                                 BIT(31)

/* RAM configuration, device id and system lock registers map. */
struct pic32mm_rds_conf_t {
    struct pic32mm_reg_t CFGCON;
    uint32_t RESERVED0[4];
    struct pic32mm_reg_t DEVID;
    struct pic32mm_reg_t SYSKEY;
};

#define PIC32MM_RDS_CONF_CFGCON_BMXERRDIS                    BIT(27)
#define PIC32MM_RDS_CONF_CFGCON_BMXARB_POS                   (24)
#define PIC32MM_RDS_CONF_CFGCON_BMXARB_MASK     \
    (0x3 << PIC32MM_RDS_CONF_CFGCON_BMXARB_POS)
#define PIC32MM_RDS_CONF_CFGCON_BMXARB(value)           \
    BITFIELD_SET(PIC32MM_RDS_CONF_CFGCON_BMXARB, value)
#define PIC32MM_RDS_CONF_CFGCON_EXECADDR_POS                 (16)
#define PIC32MM_RDS_CONF_CFGCON_EXECADDR_MASK           \
    (0xff << PIC32MM_RDS_CONF_CFGCON_EXECADDR_POS)
#define PIC32MM_RDS_CONF_CFGCON_EXECADDR(value)                 \
    BITFIELD_SET(PIC32MM_RDS_CONF_CFGCON_EXECADDR, value)
#define PIC32MM_RDS_CONF_CFGCON_JTAGEN                       BIT(3)

#define PIC32MM_RDS_CONF_SYSKEY_UNLOCK_1                     0xaa996655
#define PIC32MM_RDS_CONF_SYSKEY_UNLOCK_2                     0x556699aa
#define PIC32MM_RDS_CONF_SYSKEY_LOCK                         0

/* Base addresses of peripherals. */
#define PIC32MM_UART1    ((volatile struct pic32mm_uart_t *)     0xbf801800ul)
#define PIC32MM_UART2    ((volatile struct pic32mm_uart_t *)     0xbf801900ul)
#define PIC32MM_UART3    ((volatile struct pic32mm_uart_t *)     0xbf802000ul)
#define PIC32MM_OSC      ((volatile struct pic32mm_osc_t *)      0xbf802680ul)
#define PIC32MM_FLASH    ((volatile struct pic32mm_flash_t *)    0xbf802930ul)
#define PIC32MM_PINSEL   ((volatile struct pic32mm_pinsel_t *)   0xbf802a00ul)
#define PIC32MM_PORTA    ((volatile struct pic32mm_port_t *)     0xbf802bb0ul)
#define PIC32MM_PORTB    ((volatile struct pic32mm_port_t *)     0xbf802cb0ul)
#define PIC32MM_PORTC    ((volatile struct pic32mm_port_t *)     0xbf802db0ul)
#define PIC32MM_PORTD    ((volatile struct pic32mm_port_t *)     0xbf802eb0ul)
#define PIC32MM_RDS_CONF ((volatile struct pic32mm_rds_conf_t *) 0xbf803640ul)
#define PIC32MM_TIMER1   ((volatile struct pic32mm_timer_t *)    0xbf808000ul)
#define PIC32MM_INT      ((volatile struct pic32mm_int_t *)      0xbf80f000ul)
#define PIC32MM_CONF     ((volatile struct pic32mm_conf_t *)     0xbfc017c0ul)

#define PIC32MM_RAM_ADDRESS                 0x00000000
#define PIC32MM_RAM_SIZE                    0x00004000
#define PIC32MM_RAM_END                     0x00008000
#define PIC32MM_FLASH_ADDRESS               0x1d000000
#define PIC32MM_FLASH_SIZE                  0x00040000
#define PIC32MM_FLASH_END                   0x1d040000
#define PIC32MM_SFRS_ADDRESS                0x1f800000
#define PIC32MM_SFRS_SIZE                   0x00010000
#define PIC32MM_SFRS_END                    0x1f810000
#define PIC32MM_BOOT_FLASH_ADDRESS          0x1fc00000
#define PIC32MM_BOOT_FLASH_SIZE             0x00001700
#define PIC32MM_BOOT_FLASH_END              0x1fc01700
#define PIC32MM_CONFIGURATION_BITS_ADDRESS  0x1fc01700
#define PIC32MM_CONFIGURATION_BITS_SIZE     0x00000100
#define PIC32MM_CONFIGURATION_BITS_END      0x1fc01800
#define PIC32MM_UDID_ADDRESS                0x1fc41840
#define PIC32MM_UDID_SIZE                   0x00000014
#define PIC32MM_UDID_END                    0x1fc41854

#define PIC32MM_PHYSICAL_ADDRESS(address) ((address) & 0x1fffffff)

/* Interrupt service routine. */
#define ISR(vector)                             \
    void isr_ ## vector(int vector_number)

/**
 * Read given register.
 */
#define pic32mm_reg_read(reg_p)                 \
    ({                                          \
        uint32_t UNIQUE(value);                 \
        asm volatile("lw %0, 0(%1)" :           \
                     "=r" (UNIQUE(value)) :     \
                     "r" (reg_p));              \
        UNIQUE(value);                          \
    })


/**
 * Write given value to given register.
 */
#define pic32mm_reg_write(reg_p, value)                        \
    asm volatile ("sw %0, 0(%1)" : : "r" (value), "r" (reg_p));

/**
 * Clear given bits in given register.
 */
#define pic32mm_reg_clr(reg_p, value)                                   \
    asm volatile ("sw %0, 0(%1)" : : "r" (value), "r" (&(reg_p)->CLR));

/**
 * Invert given bits in given register.
 */
#define pic32mm_reg_inv(reg_p, value)                                   \
    asm volatile ("sw %0, 0(%1)" : : "r" (value), "r" (&(reg_p)->INV));

/**
 * Set given bits in given register.
 */
#define pic32mm_reg_set(reg_p, value)                        \
    asm volatile ("sw %0, 0(%1)" : : "r" (value), "r" (&(reg_p)->SET));

/**
 * Insert given value at given offset using CLR and SET registers.
 */
#define pic32mm_reg_insert(reg_p, value, width, offset)         \
    do {                                                        \
        pic32mm_reg_clr(reg_p, bits_mask_32(width) << (offset));   \
        pic32mm_reg_set(reg_p, (value) << (offset));            \
    } while (0);

#define pic32mm_mfc0(reg, select)                       \
    ({                                                  \
        uint32_t UNIQUE(c0);                            \
        asm volatile("mfc0 %0, $" #reg ", " #select :   \
                     "=r" (UNIQUE(c0)) : :              \
                     "memory");                         \
        UNIQUE(c0);                                     \
    })

#define pic32mm_mtc0(reg, select, value)                \
    asm volatile("mtc0 %0, $" #reg ", " #select : :     \
                 "r" (value) :                          \
                 "memory")

#endif
