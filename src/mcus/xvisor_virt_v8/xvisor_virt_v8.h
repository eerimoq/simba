/**
 * @section License
 *
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

#ifndef __XVISOR_VIRT_H__
#define __XVISOR_VIRT_H__

#define VIRT_IRQ_VIRT_TIMER                           27
#define VIRT_IRQ_PHYS_TIMER                           30
#define VIRT_IRQ_UART                                 33

/* Generic Interrupt Controller. */
struct virt_gic_dist_t {
        uint32_t CTRL;
        uint32_t CTR;
        uint32_t RESERVED0[62];
        uint32_t ENABLE_SET[32];
        uint32_t ENABLE_CLEAR[32];
        uint32_t PENDING_SET[32];
        uint32_t PENDING_CLEAR[32];
        uint32_t ACTIVE_BIT[64];
        uint32_t PRI[256];
        uint32_t TARGET[256];
        uint32_t CONFIG[192];
        uint32_t SOFTINT[192];
};

struct virt_gic_cpu_t {
    uint32_t CTRL;
    uint32_t PRIMASK;
    uint32_t BINPOINT;
    uint32_t IA;
    uint32_t EOI;
    uint32_t RUNNINGPRI;
    uint32_t HIGHPRI;
};

/* PrimeCell UART (PL011). */
struct virt_uart_pl011_t {
    uint32_t DR;
    uint32_t RSR_ECR;
    uint32_t RESERVED0[4];
    uint32_t FR;
    uint32_t RESERVED1[1];
    uint32_t ILPR;
    uint32_t IBRD;
    uint32_t FBRD;
    uint32_t LCR_H;
    uint32_t CR;
    uint32_t IFLS;
    uint32_t IMSC;
    uint32_t RIS;
    uint32_t MIS;
    uint32_t ICR;
    uint32_t DMACR;
};

#define VIRT_UART_PL011_DR_OE         BIT(11)
#define VIRT_UART_PL011_DR_BE         BIT(10)
#define VIRT_UART_PL011_DR_PE         BIT(9)
#define VIRT_UART_PL011_DR_FE         BIT(8)

#define VIRT_UART_PL011_RSR_ECR_OE    BIT(3)
#define VIRT_UART_PL011_RSR_ECR_BE    BIT(2)
#define VIRT_UART_PL011_RSR_ECR_PE    BIT(1)
#define VIRT_UART_PL011_RSR_ECR_FE    BIT(0)

#define VIRT_UART_PL011_FR_TXFF       BIT(5)
#define VIRT_UART_PL011_FR_RXFE       BIT(4)

#define VIRT_UART_PL011_LCR_H_WLEN_8  (BIT(6) | BIT(5))
#define VIRT_UART_PL011_LCR_H_FEN     BIT(4)

#define VIRT_UART_PL011_CR_RXE        BIT(9)
#define VIRT_UART_PL011_CR_TXE        BIT(8)
#define VIRT_UART_PL011_CR_UARTEN     BIT(0)

#define VIRT_UART_PL011_IMSC_OE       BIT(10)
#define VIRT_UART_PL011_IMSC_BE       BIT(9)
#define VIRT_UART_PL011_IMSC_PE       BIT(8)
#define VIRT_UART_PL011_IMSC_FE       BIT(7)
#define VIRT_UART_PL011_IMSC_RT       BIT(6)
#define VIRT_UART_PL011_IMSC_TX       BIT(5)
#define VIRT_UART_PL011_IMSC_RX       BIT(4)
#define VIRT_UART_PL011_IMSC_DSRM     BIT(3)
#define VIRT_UART_PL011_IMSC_DCDM     BIT(2)
#define VIRT_UART_PL011_IMSC_CTSM     BIT(1)
#define VIRT_UART_PL011_IMSC_RIM      BIT(0)

#define VIRT_UART_PL011_RIS_OE        BIT(10)
#define VIRT_UART_PL011_RIS_BE        BIT(9)
#define VIRT_UART_PL011_RIS_PE        BIT(8)
#define VIRT_UART_PL011_RIS_FE        BIT(7)
#define VIRT_UART_PL011_RIS_RT        BIT(6)
#define VIRT_UART_PL011_RIS_TX        BIT(5)
#define VIRT_UART_PL011_RIS_RX        BIT(4)
#define VIRT_UART_PL011_RIS_DSRM      BIT(3)
#define VIRT_UART_PL011_RIS_DCDM      BIT(2)
#define VIRT_UART_PL011_RIS_CTSM      BIT(1)
#define VIRT_UART_PL011_RIS_RIM       BIT(0)

#define VIRT_UART_PL011_MIS_OE        BIT(10)
#define VIRT_UART_PL011_MIS_BE        BIT(9)
#define VIRT_UART_PL011_MIS_PE        BIT(8)
#define VIRT_UART_PL011_MIS_FE        BIT(7)
#define VIRT_UART_PL011_MIS_RT        BIT(6)
#define VIRT_UART_PL011_MIS_TX        BIT(5)
#define VIRT_UART_PL011_MIS_RX        BIT(4)
#define VIRT_UART_PL011_MIS_DSRM      BIT(3)
#define VIRT_UART_PL011_MIS_DCDM      BIT(2)
#define VIRT_UART_PL011_MIS_CTSM      BIT(1)
#define VIRT_UART_PL011_MIS_RIM       BIT(0)

#define VIRT_UART_PL011_ICR_OE        BIT(10)
#define VIRT_UART_PL011_ICR_BE        BIT(9)
#define VIRT_UART_PL011_ICR_PE        BIT(8)
#define VIRT_UART_PL011_ICR_FE        BIT(7)
#define VIRT_UART_PL011_ICR_RT        BIT(6)
#define VIRT_UART_PL011_ICR_TX        BIT(5)
#define VIRT_UART_PL011_ICR_RX        BIT(4)
#define VIRT_UART_PL011_ICR_DSRM      BIT(3)
#define VIRT_UART_PL011_ICR_DCDM      BIT(2)
#define VIRT_UART_PL011_ICR_CTSM      BIT(1)
#define VIRT_UART_PL011_ICR_RIM       BIT(0)

/* VM info. */
struct virt_vm_info_t {
    uint32_t MAGIC;
    uint32_t VENDOR;
    uint32_t VERSION;
    uint32_t VCPU_COUNT;
    uint32_t BOOT_DELAY;
};

/* VirtIO device. */
struct virtio_v1_t {
    uint32_t MAGIC_VALUE;
    uint32_t VERSION;
    uint32_t DEVICE_ID;
    uint32_t VENDOR_ID;
    uint32_t HOST_FEATURES;
    uint32_t HOST_FEATURES_SEL;
    uint32_t RESERVED0[2];
    uint32_t GUEST_FEATURES;
    uint32_t GUEST_FEATURES_SEL;
    uint32_t GUEST_PAGE_SIZE;
    uint32_t RESERVED1[1];
    uint32_t QUEUE_SEL;
    uint32_t QUEUE_NUM_MAX;
    uint32_t QUEUE_NUM;
    uint32_t QUEUE_ALIGN;
    uint32_t QUEUE_PFN;
    uint32_t RESERVED2[3];
    uint32_t QUEUE_NOTIFY;
    uint32_t RESERVED3[3];
    uint32_t INTERRUPT_STATUS;
    uint32_t INTERRUPT_ACK;
    uint32_t RESERVED4[2];
    uint32_t STATUS;
    uint32_t RESERVED5[35];
    /* Device specific registers may follow. */
};

#define VIRTIO_DEVICE_ID_NET                                1
#define VIRTIO_DEVICE_ID_BLOCK                              2
#define VIRTIO_DEVICE_ID_CONSOLE                            3
#define VIRTIO_DEVICE_ID_RPMSG                              4

struct virtio_net_v1_t {
    struct virtio_v1_t V1;
    uint8_t MAC[6];
    uint16_t STATUS;
    uint16_t VIRTQUEUE_PAIRS_MAX;
};

#define VIRTIO_CONSOLE_F_SIZE                          BIT(0)
#define VIRTIO_CONSOLE_F_MULTIPORT                     BIT(1)
#define VIRTIO_CONSOLE_F_EMERG_WRITE                   BIT(2)

struct virtio_console_v1_t {
    struct virtio_v1_t V1;
    uint16_t COLUMNS;
    uint16_t ROWS;
    uint32_t PORTS_MAX;
    uint32_t EMERG_WRITE;
};

#define VIRT_GIC_DIST    ((volatile struct virt_gic_dist_t *)     0x08000000)
#define VIRT_GIC_CPU     ((volatile struct virt_gic_cpu_t *)      0x08010000)
#define VIRT_UART_PL011  ((volatile struct virt_uart_pl011_t *)   0x09000000)
#define VIRT_IO_NET      ((volatile struct virtio_net_v1_t *)     0x0a000000)

/* Interrupt service routine. */
#define ISR(vector)                             \
    void isr_ ## vector(void)

/* Exception service routine. */
#define ESR(vector)                             \
    void c_esr_ ## vector(void)

static inline void mcu_configure_interrupt(int id, int prio)
{
    VIRT_GIC_DIST->PRI[id / 4] |= ((prio & 0xff) << (id % 4));
    VIRT_GIC_DIST->ENABLE_SET[id / 32] |= (1 << (id % 32));
}

#endif
