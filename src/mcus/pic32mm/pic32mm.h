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

/* 5. Flash. */
struct pic32mm_flash_t {
    uint32_t NVMCON;
    uint32_t NVMCONCLR;
    uint32_t NVMCONSET;
    uint32_t NVMCONINV;
    uint32_t NVMKEY;
    uint32_t RESERVED0[3];
    uint32_t NVMADDR;
    uint32_t NVMADDRCLR;
    uint32_t NVMADDRSET;
    uint32_t NVMADDRINV;
    uint32_t NVMDATA0;
    uint32_t RESERVED1[3];
    uint32_t NVMDATA1;
    uint32_t RESERVED2[3];
    uint32_t NVMSRCADDR;
    uint32_t RESERVED3[3];
    uint32_t NVMPWP;
    uint32_t NVMPWPCLR;
    uint32_t NVMPWPSET;
    uint32_t NVMPWPINV;
    uint32_t NVMBWP;
    uint32_t NVMBWPCLR;
    uint32_t NVMBWPSET;
    uint32_t NVMBWPINV;
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
    uint32_t INTCON;
    uint32_t INTCONCLR;
    uint32_t INTCONSET;
    uint32_t INTCONINV;
    uint32_t PRISS;
    uint32_t PRISSCLR;
    uint32_t PRISSSET;
    uint32_t PRISSINV;
    uint32_t INTSTAT;
    uint32_t INTSTATCLR;
    uint32_t INTSTATSET;
    uint32_t INTSTATINV;
    uint32_t IPTMR;
    uint32_t IPTMRCLR;
    uint32_t IPTMRSET;
    uint32_t IPTMRINV;
    struct {
        uint32_t VALUE;
        uint32_t CLR;
        uint32_t SET;
        uint32_t INV;
    } IFS[4];
    uint32_t RESERVED0[16];
    struct {
        uint32_t VALUE;
        uint32_t CLR;
        uint32_t SET;
        uint32_t INV;
    } IEC[4];
    uint32_t RESERVED1[16];
    struct {
        uint32_t VALUE;
        uint32_t CLR;
        uint32_t SET;
        uint32_t INV;
    } IPC[26];
};

/* 10. IO. */
struct pic32mm_port_t {
    uint32_t ANSEL;
    uint32_t ANSELCLR;
    uint32_t ANSELSET;
    uint32_t ANSELINV;
    uint32_t TRIS;
    uint32_t TRISCLR;
    uint32_t TRISSET;
    uint32_t TRISINV;
    uint32_t PORT;
    uint32_t PORTCLR;
    uint32_t PORTSET;
    uint32_t PORTINV;
    uint32_t LAT;
    uint32_t LATCLR;
    uint32_t LATSET;
    uint32_t LATINV;
    uint32_t ODC;
    uint32_t ODCCLR;
    uint32_t ODCSET;
    uint32_t ODCINV;
    uint32_t CNPU;
    uint32_t CNPUCLR;
    uint32_t CNPUSET;
    uint32_t CNPUINV;
    uint32_t CNPD;
    uint32_t CNPDCLR;
    uint32_t CNPDSET;
    uint32_t CNPDINV;
    uint32_t CNCON;
    uint32_t CNCONCLR;
    uint32_t CNCONSET;
    uint32_t CNCONINV;
    uint32_t CNEN0;
    uint32_t CNEN0CLR;
    uint32_t CNEN0SET;
    uint32_t CNEN0INV;
    uint32_t CNSTAT;
    uint32_t CNSTATCLR;
    uint32_t CNSTATSET;
    uint32_t CNSTATINV;
    uint32_t CNEN1;
    uint32_t CNEN1CLR;
    uint32_t CNEN1SET;
    uint32_t CNEN1INV;
    uint32_t CNF;
    uint32_t CNFCLR;
    uint32_t CNFSET;
    uint32_t CNFINV;
};

struct pic32mm_pinsel_t {
    uint32_t RPCON;
    uint32_t RESERVED0[7];
    struct {
        uint32_t VALUE;
        uint32_t CLR;
        uint32_t SET;
        uint32_t INV;
    } RPINR[12];
    uint32_t RESERVED1[12];
    struct {
        uint32_t VALUE;
        uint32_t CLR;
        uint32_t SET;
        uint32_t INV;
    } RPOR[6];
};

/* 17. UART. */
struct pic32mm_uart_t {
    uint32_t MODE;
    uint32_t MODECLR;
    uint32_t MODESET;
    uint32_t MODEINV;
    uint32_t STA;
    uint32_t STACLR;
    uint32_t STASET;
    uint32_t STAINV;
    uint32_t TXREG;
    uint32_t TXREGCLR;
    uint32_t TXREGSET;
    uint32_t TXREGINV;
    uint32_t RXREG;
    uint32_t RXREGCLR;
    uint32_t RXREGSET;
    uint32_t RXREGINV;
    uint32_t BRG;
    uint32_t BRGCLR;
    uint32_t BRGSET;
    uint32_t BRGINV;
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

/* 26. Configuration bits. */
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
#define PIC32MM_CONF_FDEVOPT_USERID_POS                      16
#define PIC32MM_CONF_FDEVOPT_USERID_MASK        \
    (0xffff << PIC32MM_CONF_FDEVOPT_USERID_POS)
#define PIC32MM_CONF_FDEVOPT_USERID(value)              \
    BITFIELD_SET(PIC32MM_CONF_FDEVOPT_USERID, value)
#define PIC32MM_CONF_FDEVOPT_FVBUSIO                         BIT(15)
#define PIC32MM_CONF_FDEVOPT_FUSBIDIO                        BIT(14)
#define PIC32MM_CONF_FDEVOPT_ALTI2C                          BIT(4)
#define PIC32MM_CONF_FDEVOPT_SOSCHP                          BIT(3)

#define PIC32MM_CONF_FICD_RESERVED                           0xffffffe3
#define PIC32MM_CONF_FICD_ICS_POS                            3
#define PIC32MM_CONF_FICD_ICS_MASK              \
    (0x3 << PIC32MM_CONF_FICD_ICS_POS)
#define PIC32MM_CONF_FICD_ICS(value)            \
    BITFIELD_SET(PIC32MM_CONF_FICD_ICS, value)
#define PIC32MM_CONF_FICD_JTAGEN                             BIT(2)

#define PIC32MM_CONF_FPOR_RESERVED                           0xfffffff0
#define PIC32MM_CONF_FPOR_LPBOREN                            BIT(3)
#define PIC32MM_CONF_FPOR_RETVR                              BIT(2)
#define PIC32MM_CONF_FPOR_BOREN_POS                          0
#define PIC32MM_CONF_FPOR_BOREN_MASK            \
    (0x3 << PIC32MM_CONF_FPOR_BOREN_POS)
#define PIC32MM_CONF_FPOR_BOREN(value)                  \
    BITFIELD_SET(PIC32MM_CONF_FPOR_BOREN, value)

#define PIC32MM_CONF_FWDT_RESERVED                           0xffff0000
#define PIC32MM_CONF_FWDT_FWDTEN                             BIT(15)
#define PIC32MM_CONF_FWDT_RCLKSEL_POS                        13
#define PIC32MM_CONF_FWDT_RCLKSEL_MASK          \
    (0x3 << PIC32MM_CONF_FWDT_RCLKSEL_POS)
#define PIC32MM_CONF_FWDT_RCLKSEL(value)                \
    BITFIELD_SET(PIC32MM_CONF_FWDT_RCLKSEL, value)
#define PIC32MM_CONF_FWDT_RWDTPS_POS                         8
#define PIC32MM_CONF_FWDT_RWDTPS_MASK           \
    (0x1f << PIC32MM_CONF_FWDT_RWDTPS_POS)
#define PIC32MM_CONF_FWDT_RWDTPS(value)                 \
    BITFIELD_SET(PIC32MM_CONF_FWDT_RWDTPS, value)
#define PIC32MM_CONF_FWDT_WINDIS                             BIT(7)
#define PIC32MM_CONF_FWDT_FWDTWINSZ_POS                      5
#define PIC32MM_CONF_FWDT_FWDTWINSZ_MASK        \
    (0x3 << PIC32MM_CONF_FWDT_FWDTWINSZ_POS)
#define PIC32MM_CONF_FWDT_FWDTWINSZ(value)              \
    BITFIELD_SET(PIC32MM_CONF_FWDT_FWDTWINSZ, value)
#define PIC32MM_CONF_FWDT_SWDTPS_POS                         0
#define PIC32MM_CONF_FWDT_SWDTPS_MASK           \
    (0x1f << PIC32MM_CONF_FWDT_SWDTPS_POS)
#define PIC32MM_CONF_FWDT_SWDTPS(value)                 \
    BITFIELD_SET(PIC32MM_CONF_FWDT_SWDTPS, value)

#define PIC32MM_CONF_FOSCSEL_RESERVED                        0xffff2828
#define PIC32MM_CONF_FOSCSEL_FCKSM_POS                       14
#define PIC32MM_CONF_FOSCSEL_FCKSM_MASK         \
    (0x3 << PIC32MM_CONF_FOSCSEL_FCKSM_POS)
#define PIC32MM_CONF_FOSCSEL_FCKSM(value)               \
    BITFIELD_SET(PIC32MM_CONF_FOSCSEL_FCKSM, value)
#define PIC32MM_CONF_FOSCSEL_SOSCSEL                         BIT(12)
#define PIC32MM_CONF_FOSCSEL_OSCIOFNC                        BIT(10)
#define PIC32MM_CONF_FOSCSEL_POSCMOD_POS                     8
#define PIC32MM_CONF_FOSCSEL_POSCMOD_MASK       \
    (0x3 << PIC32MM_CONF_FOSCSEL_POSCMOD_POS)
#define PIC32MM_CONF_FOSCSEL_POSCMOD(value)             \
    BITFIELD_SET(PIC32MM_CONF_FOSCSEL_POSCMOD, value)
#define PIC32MM_CONF_FOSCSEL_IESO                            BIT(7)
#define PIC32MM_CONF_FOSCSEL_SOSCEN                          BIT(6)
#define PIC32MM_CONF_FOSCSEL_PLLSRC                          BIT(4)
#define PIC32MM_CONF_FOSCSEL_FNOSC_POS                       0
#define PIC32MM_CONF_FOSCSEL_FNOSC_MASK         \
    (0x7 << PIC32MM_CONF_FOSCSEL_FNOSC_POS)
#define PIC32MM_CONF_FOSCSEL_FNOSC(value)               \
    BITFIELD_SET(PIC32MM_CONF_FOSCSEL_FNOSC, value)

#define PIC32MM_CONF_FSEC_RESERVED                           0x7fffffff
#define PIC32MM_CONF_FSEC_CP                                 BIT(31)

/* Base addresses of peripherals. */
#define PIC32MM_UART1   ((volatile struct pic32mm_uart_t *)   0xbf801800ul)
#define PIC32MM_UART2   ((volatile struct pic32mm_uart_t *)   0xbf801900ul)
#define PIC32MM_UART3   ((volatile struct pic32mm_uart_t *)   0xbf802000ul)
#define PIC32MM_FLASH   ((volatile struct pic32mm_flash_t *)  0xbf802930ul)
#define PIC32MM_PINSEL  ((volatile struct pic32mm_pinsel_t *) 0xbf802a00ul)
#define PIC32MM_PORTA   ((volatile struct pic32mm_port_t *)   0xbf802bb0ul)
#define PIC32MM_PORTB   ((volatile struct pic32mm_port_t *)   0xbf802cb0ul)
#define PIC32MM_PORTC   ((volatile struct pic32mm_port_t *)   0xbf802db0ul)
#define PIC32MM_PORTD   ((volatile struct pic32mm_port_t *)   0xbf802eb0ul)
#define PIC32MM_TIMER1  ((volatile struct pic32mm_timer_t *)  0xbf808000ul)
#define PIC32MM_CONF    ((volatile struct pic32mm_conf_t *)   0xbfc017c0ul)
#define PIC32MM_INT     ((volatile struct pic32mm_int_t *)    0xbf80f000ul)

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

/* Interrupt service routine. */
#define ISR(vector)                             \
    void isr_ ## vector(int vector_number)

/**
 * Read given register.
 */
uint32_t pic32mm_reg_read(volatile uint32_t *reg_p);

/**
 * Write given register.
 */
void pic32mm_reg_write(volatile uint32_t *reg_p, uint32_t value);

#define pic32mm_mfc0(reg, select) ({                            \
            uint32_t UNIQUE(c0);                                \
            asm volatile("mfc0 %0, $" #reg ", " #select :       \
                         "=r" (UNIQUE(c0)) : :                  \
                         "memory");                             \
            UNIQUE(c0);                                         \
        })

#define pic32mm_mtc0(reg, select, value)                \
    asm volatile("mtc0 %0, $" #reg ", " #select : :     \
                 "r" (value) :                          \
                 "memory")


#endif
