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

/**
 * Get the address to given endpoint fifo in DPRAM.
 */
static uint8_t *get_fifo(int endpoint)
{
    uint8_t *dpram_p;

    dpram_p = (uint8_t *)SAM_UOTGHS_DPRAM_ADDRESS;
    dpram_p += (SAM_UOTGHS_DPRAM_PIPE_SIZE * endpoint);

    return (dpram_p);
}

/**
 * Start a transmission on given endpoint.
 */
static void tx_start(volatile struct sam_uotghs_t *regs_p,
                     int endpoint)
{
    regs_p->DEVICE.EPTICR[endpoint] = SAM_UOTGHS_DEVICE_EPTICR_TXINIC;
}

/**
 * Busy wait for current transmission for given endpoint to end.
 */
static void tx_wait(volatile struct sam_uotghs_t *regs_p,
                    int endpoint)
{
    while ((regs_p->DEVICE.EPTISR[endpoint] & SAM_UOTGHS_DEVICE_EPTISR_TXINI) == 0);
}

/**
 * Busy wait for current reception for given endpoint to end.
 */
static void rx_wait(volatile struct sam_uotghs_t *regs_p,
                    int endpoint)
{
    while ((regs_p->DEVICE.EPTISR[endpoint] & SAM_UOTGHS_DEVICE_EPTISR_RXOUTI) == 0);
}

/**
 * Start a control transmission.
 */
static void control_tx_start(volatile struct sam_uotghs_t *regs_p)
{
    tx_start(regs_p, 0);
}

/**
 * Busy wait for current control transmission to end.
 */
static void control_tx_wait(volatile struct sam_uotghs_t *regs_p)
{
    tx_wait(regs_p, 0);
}

/**
 * Busy wait for current control reception to end.
 */
static void control_rx_wait(volatile struct sam_uotghs_t *regs_p)
{
    rx_wait(regs_p, 0);
}

static void control_rx_release(volatile struct sam_uotghs_t *regs_p)
{
    regs_p->DEVICE.EPTICR[0] = SAM_UOTGHS_DEVICE_EPTICR_RXOUTIC;
}

/**
 * Intialize given endpoint.
 */
static void endpoint_init(volatile struct sam_uotghs_t *regs_p,
                          uint8_t index,
                          uint8_t type,
                          uint8_t size)
{
    regs_p->DEVICE.EPTCFG[index] = (size
                                    | type
                                    | SAM_UOTGHS_DEVICE_EPTCFG_EPBK_1_BANK
                                    | SAM_UOTGHS_DEVICE_EPTCFG_NBTRANS(1)
                                    | SAM_UOTGHS_DEVICE_EPTCFG_ALLOC);
    regs_p->DEVICE.EPT = BIT(index);
    regs_p->DEVICE.EPTIER[index] = SAM_UOTGHS_DEVICE_EPTIER_RXSTPES_UNDERFES;
}

/**
 * Initialize all endpoints.
 */
static void endpoints_init(volatile struct sam_uotghs_t *regs_p)
{
    regs_p->DEVICE.EPTCFG[2] = (SAM_UOTGHS_DEVICE_EPTCFG_EPTYPE_BLK
                                | SAM_UOTGHS_DEVICE_EPTCFG_EPSIZE_512_BYTES
                                | SAM_UOTGHS_DEVICE_EPTCFG_EPBK_1_BANK
                                | SAM_UOTGHS_DEVICE_EPTCFG_NBTRANS(1)
                                | SAM_UOTGHS_DEVICE_EPTCFG_ALLOC);
    regs_p->DEVICE.EPT |= BIT(2);
    regs_p->DEVICE.IER |= SAM_UOTGHS_DEVICE_IER_PEP_2;
    regs_p->DEVICE.EPTIER[2] = SAM_UOTGHS_DEVICE_EPTIER_RXOUTES;

    regs_p->DEVICE.EPTCFG[3] = (SAM_UOTGHS_DEVICE_EPTCFG_EPTYPE_BLK
                                | SAM_UOTGHS_DEVICE_EPTCFG_EPSIZE_512_BYTES
                                | SAM_UOTGHS_DEVICE_EPTCFG_EPBK_1_BANK
                                | SAM_UOTGHS_DEVICE_EPTCFG_NBTRANS(1)
                                | SAM_UOTGHS_DEVICE_EPTCFG_EPDIR
                                | SAM_UOTGHS_DEVICE_EPTCFG_ALLOC);
    regs_p->DEVICE.EPT |= BIT(3);
}

/**
 * Read data from given endpoint.
 */
static void endpoint_read(int endpoint, void* buf_p, uint8_t size)
{
    uint8_t *b_p;
    uint8_t *fifo_p;

    b_p = buf_p;
    fifo_p = get_fifo(endpoint);

    while (size--) {
        *b_p++ = *fifo_p++;
    }
}

/**
 * Read data from the control endpoint.
 */
static void control_endpoint_read(void* buf_p, uint8_t size)
{
    endpoint_read(0, buf_p, size);
}

/**
 * Transmit given control packet.
 */
static void control_write(volatile struct sam_uotghs_t *regs_p,
                          const void* buf_p,
                          size_t size,
                          size_t offset)
{
    const uint8_t *b_p;
    uint8_t *fifo_p;

    b_p = buf_p;
    fifo_p = get_fifo(0);

    while (size--) {
        *fifo_p++ = *b_p++;
    }

    control_tx_start(regs_p);
}

/**
 * Write given list of descriptors to the host.
 */
static int control_write_descriptors(volatile struct sam_uotghs_t *regs_p,
                                     const union usb_descriptor_t **descs_pp,
                                     size_t size)
{
    size_t desc_size;
    size_t fifo_offset;
    const union usb_descriptor_t* desc_p;
    uint8_t *fifo_p;
    const uint8_t *buf_p;

    fifo_p = get_fifo(0);
    fifo_offset = 0;

    while (size > 0) {
        desc_p = *descs_pp++;

        /* Abort if the host requested too many bytes. */
        if (desc_p == NULL) {
            return (-EPROTO);
        }

        buf_p = (const uint8_t *)desc_p;
        desc_size = desc_p->header.length;

        if (desc_size > size) {
            desc_size = size;
        }

        size -= desc_size;

        while (desc_size > 0) {
            while ((fifo_offset < 64) && (desc_size > 0)) {
                *fifo_p++ = *buf_p++;
                fifo_offset++;
                desc_size--;
            }

            if (fifo_offset == 64) {
                fifo_p = get_fifo(0);
                fifo_offset = 0;
                control_tx_start(regs_p);
                control_tx_wait(regs_p);
            }
        }
    }

    if (fifo_offset > 0) {
        control_tx_start(regs_p);
        control_tx_wait(regs_p);
    }

    return (0);
}

static void handle_set_address(volatile struct sam_uotghs_t *regs_p,
                               uint16_t address)
{
    /* Send an empty status packet before changing the address. */
    control_tx_start(regs_p);
    control_tx_wait(regs_p);

    regs_p->DEVICE.CTRL = bits_insert_32(regs_p->DEVICE.CTRL,
                                         SAM_UOTGHS_DEVICE_CTRL_UADD_POS,
                                         SAM_UOTGHS_DEVICE_CTRL_UADD_WIDTH,
                                         address);
}

static void handle_get_descriptor(struct usb_device_driver_t *self_p,
                                  uint8_t index,
                                  uint8_t type,
                                  uint16_t length)
{
    switch (type) {

    case DESCRIPTOR_TYPE_CONFIGURATION:
        control_write_descriptors(self_p->dev_p->regs_p,
                                  &self_p->descriptors_pp[1],
                                  length);
        break;

    case DESCRIPTOR_TYPE_DEVICE:
        control_write(self_p->dev_p->regs_p,
                      self_p->descriptors_pp[0],
                      length,
                      0);
        break;

    case DESCRIPTOR_TYPE_STRING:
        while (1);
        break;

    default:
        while (1);
        break;
    }
}

static void handle_set_configuration(struct usb_device_driver_t *self_p,
                                     struct usb_setup_t *setup_p)
{
    uint32_t mask;

    mask = (setup_p->request_type & REQUEST_TYPE_RECIPIENT_MASK);

    if (mask == REQUEST_TYPE_RECIPIENT_DEVICE) {
        endpoints_init(self_p->dev_p->regs_p);
        self_p->configuration = setup_p->u.set_configuration.configuration_value;
    }

    control_tx_start(self_p->dev_p->regs_p);
}

static void handle_type_standard(struct usb_device_driver_t *self_p,
                                 struct usb_setup_t *setup_p)
{
    switch (setup_p->request) {

    case REQUEST_GET_STATUS:
        while (1);
        break;

    case REQUEST_SET_ADDRESS:
        handle_set_address(self_p->dev_p->regs_p,
                           setup_p->u.set_address.device_address);
        break;

    case REQUEST_GET_DESCRIPTOR:
        handle_get_descriptor(self_p,
                              setup_p->u.get_descriptor.descriptor_index,
                              setup_p->u.get_descriptor.descriptor_type,
                              setup_p->length);
        break;

    case REQUEST_SET_CONFIGURATION:
        handle_set_configuration(self_p, setup_p);
        break;

    default:
        while (1);
        break;
    }
}

static void handle_type_class(struct usb_device_driver_t *self_p,
                              struct usb_setup_t *setup_p)
{
    int i;
    struct usb_device_driver_base_t *driver_p;

    for (i = 0; i < self_p->drivers_max; i++) {
        driver_p = self_p->drivers_pp[i];

        if (driver_p->setup_isr(driver_p, setup_p) == 0) {
            return;
        }
    }
}

static void handle_setup(struct usb_device_driver_t *self_p,
                         volatile struct sam_uotghs_t *regs_p)
{
    struct usb_setup_t setup;

    /* Read the setup packet. */
    control_endpoint_read(&setup, sizeof(setup));

    /* regs_p->DEVICE.IDR = (SAM_UOTGHS_DEVICE_ISR_PEP_0 << i); */
    control_rx_release(self_p->dev_p->regs_p);
    regs_p->DEVICE.EPTICR[0] = SAM_UOTGHS_DEVICE_EPTICR_RXSTPIC_UNDERFIC;

    /* Always wait for any ongoing transmission to end. */
    control_tx_wait(regs_p);

    switch (setup.request_type & REQUEST_TYPE_TYPE_MASK) {

    case REQUEST_TYPE_TYPE_STANDARD:
        handle_type_standard(self_p, &setup);
        break;

    case REQUEST_TYPE_TYPE_CLASS:
        handle_type_class(self_p, &setup);
        break;

    default:
        while (1);
        break;
    }
}

extern struct usb_device_class_cdc_driver_t cdc;

static void endpoint_isr(struct usb_device_driver_t *self_p,
                         volatile struct sam_uotghs_t *regs_p,
                         int endpoint)
{
    uint32_t isr;
    struct usb_device_driver_base_t *drv_p;

    drv_p = self_p->endpoints[endpoint];

    if (drv_p == NULL) {
        return;
    }

    isr = regs_p->DEVICE.EPTISR[endpoint];

    /* Received packet from the host. */
    if (isr & SAM_UOTGHS_DEVICE_EPTISR_RXOUTI) {
        regs_p->DEVICE.EPTICR[endpoint] = SAM_UOTGHS_DEVICE_EPTICR_RXOUTIC;
        drv_p->start_of_frame_isr(drv_p);
        regs_p->DEVICE.EPTIDR[endpoint] = SAM_UOTGHS_DEVICE_EPTIDR_FIFOCONC;
    }

    /* Transmitted packet to the host. */
    if (isr & SAM_UOTGHS_DEVICE_EPTISR_TXINI) {
    }
}

static void isr(struct usb_driver_base_t *drv_p)
{
    struct usb_device_driver_t *self_p;
    struct usb_device_t *dev_p;
    volatile struct sam_uotghs_t *regs_p;
    uint32_t isr;
    int endpoint;

    self_p = (struct usb_device_driver_t *)drv_p;
    dev_p = self_p->dev_p;
    regs_p = dev_p->regs_p;
    isr = regs_p->DEVICE.ISR;

    /* Reset interrupt. */
    if (isr & SAM_UOTGHS_DEVICE_ISR_EORST) {
        self_p->configuration = -1;
        endpoint_init(regs_p,
                      0,
                      SAM_UOTGHS_DEVICE_EPTCFG_EPTYPE_CTRL,
                      SAM_UOTGHS_DEVICE_EPTCFG_EPSIZE_64_BYTES);
        regs_p->DEVICE.CTRL |= SAM_UOTGHS_DEVICE_CTRL_ADDEN;
    }

    if (regs_p->DEVICE.EPTISR[0] & SAM_UOTGHS_DEVICE_EPTISR_RXSTPI_UNDERFI) {
        handle_setup(self_p, regs_p);
    }

    /* Endpoint interrupts. */
    for (endpoint = 1; endpoint < 10; endpoint++) {
        if (isr & (SAM_UOTGHS_DEVICE_ISR_PEP_0 << endpoint)) {
            endpoint_isr(self_p, regs_p, endpoint);
        }
    }

    /* All interrupts have been handled. */
    regs_p->DEVICE.ICR = isr;
}

static int usb_device_port_module_init(void)
{
    return (0);
}

static int usb_device_port_init(struct usb_device_driver_t *self_p,
                                struct usb_device_t *dev_p)
{
    int i;
    int index;
    int endpoint;
    struct usb_device_driver_base_t *drv_p;

    self_p->base.isr = isr;

    for (i = 0; i < self_p->drivers_max; i++) {
        drv_p = self_p->drivers_pp[i];
        index = 0;

        while (1) {
            endpoint = drv_p->get_endpoint_by_index(drv_p, index);

            if (endpoint < 0) {
                break;
            }

            self_p->endpoints[endpoint] = drv_p;
            index++;
        }
    }

    return (0);
}

static int usb_device_port_start(struct usb_device_driver_t *self_p)
{
    struct usb_device_t *dev_p;
    volatile struct sam_uotghs_t *regs_p;

    dev_p = self_p->dev_p;
    regs_p = dev_p->regs_p;
    dev_p->drv_p = (struct usb_driver_base_t *)self_p;

    /* Start the USB clock. */
    SAM_PMC->CKGR_UCKR = (PMC_CKGR_UCKR_UPLLCOUNT(3)
                          | PMC_CKGR_UCKR_UPLLEN);

    while ((SAM_PMC->SR & PMC_SR_LOCKU) == 0);

    SAM_PMC->USB = (PMC_USB_USBS | PMC_USB_USBDIV(0));
    SAM_PMC->SCER = PMC_UOTGCLK;

    pmc_peripheral_clock_enable(dev_p->id);

    /* Enable the device in the nvic interrupt subsyetem. */
    nvic_enable_interrupt(dev_p->id);

    /* Enable the USB hardware. Force device mode. */
    regs_p->CTRL = (SAM_UOTGHS_CTRL_UIMOD
                    | SAM_UOTGHS_CTRL_OTGPADE
                    | SAM_UOTGHS_CTRL_USBE
                    | SAM_UOTGHS_CTRL_VBUSPO);

    /* Wait for the USB clock to stablize. */
    while ((regs_p->SR & SAM_UOTGHS_SR_CLKUSABLE) == 0);

    /* Enable the USB hardware. Force device mode. */
    regs_p->CTRL = (SAM_UOTGHS_CTRL_UIMOD
                    | SAM_UOTGHS_CTRL_OTGPADE
                    | SAM_UOTGHS_CTRL_USBE
                    | SAM_UOTGHS_CTRL_VBUSPO
                    | SAM_UOTGHS_CTRL_VBUSHWC);

    /* Enable setup and start of frame interrupts. */
    regs_p->DEVICE.IER = (SAM_UOTGHS_DEVICE_IER_PEP_0
                          | SAM_UOTGHS_DEVICE_IER_EORSTES);

    /* Set detach bit to zero to attach to the host. */
    regs_p->DEVICE.CTRL = 0;

    return (0);
}

static int usb_device_port_stop(struct usb_device_driver_t *self_p)
{
    return (0);
}

static ssize_t usb_device_port_read_isr(struct usb_device_driver_t *self_p,
                                        int endpoint,
                                        size_t offset,
                                        void *buf_p,
                                        size_t size)
{
    volatile struct sam_uotghs_t *regs_p;
    size_t i;
    size_t fifo_size;
    uint8_t *b_p;
    uint8_t *fifo_p;

    regs_p = self_p->dev_p->regs_p;

    if (endpoint == 0) {
        control_rx_wait(regs_p);
        control_endpoint_read(buf_p, size);
        control_rx_release(regs_p);
        control_tx_start(regs_p);
        control_tx_wait(regs_p);
    } else {
        fifo_size = (regs_p->DEVICE.EPTISR[endpoint] >> 20);
        size = MIN(size, fifo_size);

        if (size > 0) {
            b_p = buf_p;
            fifo_p = get_fifo(endpoint);
            fifo_p += offset;

            for (i = 0; i < size; i++) {
                *b_p++ = *fifo_p++;
            }
        }
    }

    return (size);
}

static ssize_t usb_device_port_write_isr(struct usb_device_driver_t *self_p,
                                         int endpoint,
                                         const void *buf_p,
                                         size_t size)
{
    volatile struct sam_uotghs_t *regs_p;
    const uint8_t *b_p;
    uint8_t *fifo_p;
    size_t left;

    left = size;
    regs_p = self_p->dev_p->regs_p;
    b_p = buf_p;
    fifo_p = get_fifo(endpoint);
    tx_wait(regs_p, endpoint);

    while (left--) {
        *fifo_p++ = *b_p++;
    }

    tx_start(regs_p, endpoint);
    regs_p->DEVICE.EPTIDR[endpoint] = SAM_UOTGHS_DEVICE_EPTIDR_FIFOCONC;

    return (size);
}

static ssize_t usb_device_port_write(struct usb_device_driver_t *self_p,
                                     int endpoint,
                                     const void *buf_p,
                                     size_t size)
{
    sys_lock();
    size = usb_device_port_write_isr(self_p, endpoint, buf_p, size);
    sys_unlock();

    return (size);
}

static ssize_t usb_device_port_size_isr(struct usb_device_driver_t *self_p,
                                        int endpoint)
{
    return (self_p->dev_p->regs_p->DEVICE.EPTISR[endpoint] >> 20);
}
