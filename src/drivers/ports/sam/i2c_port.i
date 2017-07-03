/**
 * @section License
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2016, Erik Moqvist
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

/* SAM: TWI Address */
/* from sam3.h */
/*
#define SAM_TWI0       ((volatile struct sam_twi_t    *)0x4008c000u)
#define SAM_TWI1       ((volatile struct sam_twi_t    *)0x40090000u)
*/

/* SAM: Register Mapping */
#define SAM_TWI_CR      0x00 /* W/O */

#define SAM_TWI_MMR     0x04 /* R/W */
#define SAM_TWI_SMR     0x08 /* R/W */
#define SAM_TWI_IADR    0x0C /* R/W */
#define SAM_TWI_CWGR    0x10 /* R/W */

#define SAM_TWI_SR      0x20 /* R/O */

#define SAM_TWI_IER     0x24 /* W/O */
#define SAM_TWI_IDR     0x28 /* W/O */

#define SAM_TWI_IMR     0x2C /* R/O */
#define SAM_TWI_RHR     0x30 /* R/O */

#define SAM_TWI_THR     0x34 /* W/O */

/* TWI_CR */
#define SAM_TWI_CR_START    (1)
#define SAM_TWI_CR_STOP     (1<<1)
#define SAM_TWI_CR_MSEN     (1<<2) /* Enable TWI Master Mode (Iff MSDIS == 0)  */
#define SAM_TWI_CR_MSDIS    (1<<3)
#define SAM_TWI_CR_SVEN     (1<<4)
#define SAM_TWI_CR_SVDIS    (1<<5)
#define SAM_TWI_CR_QUICK    (1<<6)
#define SAM_TWI_CR_SWRST    (1<<7)

/* TWI_MMR */
#define SAM_TWI_MMR_MREAD       (1 << 12)
#define SAM_TWI_MMR_DADR_MASK   (0b01111111 << 16) /* Device Address Mask */
#define SAM_TWI_MMR_IADRSZ_0    (0b00000000 << 8)  /* Internal Device Address Size: None */
#define SAM_TWI_MMR_IADRSZ_1    (0b00000001 << 8)  /* Internal Device Address Size: 1 Byte */
#define SAM_TWI_MMR_IADRSZ_2    (0b00000010 << 8)  /* Internal Device Address Size: 2 Byte */
#define SAM_TWI_MMR_IADRSZ_3    (0b00000011 << 8)  /* Internal Device Address Size: 3 Byte */

/* TWI_SMR */
#define SAM_TWI_SMR_SADR_MASK   (0b01111111 << 16) /* Device Address Mask */

/* TWI_IADR */
#define SAM_TWI_IADR_MASK       (0xFF << 16) | (0xFF << 8) | (0xFF) /* 0, 1, 2 or 3 bytes depending on IADRSZ. */

/* TWI_CWGR */
#define SAM_TWI_CWGR_CKDIV_MASK       (0b111 << 16)
#define SAM_TWI_CWGR_CHDIV_MASK       (0xff  << 8)
#define SAM_TWI_CWGR_CLDIV_MASK       (0xff)

/* TWI_SR */
#define SAM_TWI_SR_TXCOMP       (1)      /* Transmission Completed (automatically set / reset) */
#define SAM_TWI_SR_RXRDY        (1 << 1) /* A byte has been received in the TWI_RHR since the last read */
#define SAM_TWI_SR_TXRDY        (1 << 2) /* Transmit Holding Register Ready (automatically set / reset) */
#define SAM_TWI_SR_SVREAD       (1 << 3) /* Slave Read (automatically set / reset) 
                                            Indicates that a read access is performed by a Master. */
#define SAM_TWI_SR_SVACC        (1 << 4) /* Slave Access (automatically set / reset)
                                            Indicates that the address decoding sequence has matched (A Master has sent SADR). 
                                            SVACC remains high until a NACK or a STOP condition is detected. */
#define SAM_TWI_SR_GACC         (1 << 5) /* GACC: General Call Access (clear on read) */
#define SAM_TWI_SR_OVRE         (1 << 6) /* OVRE: Overrun Error (clear on read) 
                                            (1 = TWI_RHR has been loaded while RXRDY was set. 
                                             Reset by read in TWI_SR when TXCOMP is set.) */
 
#define SAM_TWI_SR_NACK         (1 << 8) /* Master:  A data byte has not been acknowledged by the slave component. Set at the same time as TXCOMP */
#define SAM_TWI_SR_ARBLST       (1 << 9) /* Arbitration lost. Another master of the TWI bus has won the multi-master arbitration. TXCOMP is set at the same time. */
#define SAM_TWI_SR_SCLWS        (1 << 10) /* Slave: Clock Wait State (automatically set / reset) - Clock is Stretched */
#define SAM_TWI_SR_EOSACC       (1 << 11) /* End of Slave Access */
#define SAM_TWI_SR_ENDRX        (1 << 12) /* Master: End of RX Buffer - Receive Counter Register has reached 0 since the last write in TWI_RCR or TWI_RNCR.*/
#define SAM_TWI_SR_ENDTX        (1 << 13) /* Master: End of TX Buffer - Transmit Counter Register has reached 0 since the last write in TWI_TCR or TWI_TNCR */
#define SAM_TWI_SR_RXBUFF       (1 << 14) /* Master: RX Buffer Full:  Both TWI_RCR and TWI_RNCR have a value of 0. */
#define SAM_TWI_SR_TXBUFF       (1 << 15) /* Master: TX Buffer Full:  Both TWI_TCR and TWI_TNCR have a value of 0. */


/* TWI_IER - Interrupt Enable Register */
#define SAM_TWI_IER_TXCOMP      (1)
#define SAM_TWI_IER_RXRDY       (1 << 1)
#define SAM_TWI_IER_TXRDY       (1 << 2)

#define SAM_TWI_IER_SVACC       (1 << 4)
#define SAM_TWI_IER_GACC        (1 << 5)
#define SAM_TWI_IER_OVRE        (1 << 6)

#define SAM_TWI_IER_NACK        (1 << 8)
#define SAM_TWI_IER_ARBLST      (1 << 9)
#define SAM_TWI_IER_SCLWS       (1 << 10)
#define SAM_TWI_IER_EOSACC      (1 << 11) 
#define SAM_TWI_IER_ENDRX       (1 << 12)
#define SAM_TWI_IER_ENDTX       (1 << 13)
#define SAM_TWI_IER_RXBUFF      (1 << 14)
#define SAM_TWI_IER_TXBUFF      (1 << 15)


/* TWI_IDR - Interrupt Disable Register */
#define SAM_TWI_IDR_TXCOMP      (1)
#define SAM_TWI_IDR_RXRDY       (1 << 1)
#define SAM_TWI_IDR_TXRDY       (1 << 2)

#define SAM_TWI_IDR_SVACC       (1 << 4)
#define SAM_TWI_IDR_GACC        (1 << 5)
#define SAM_TWI_IDR_OVRE        (1 << 6)

#define SAM_TWI_IDR_NACK        (1 << 8)
#define SAM_TWI_IDR_ARBLST      (1 << 9)
#define SAM_TWI_IDR_SCLWS       (1 << 10)
#define SAM_TWI_IDR_EOSACC      (1 << 11) 
#define SAM_TWI_IDR_ENDRX       (1 << 12)
#define SAM_TWI_IDR_ENDTX       (1 << 13)
#define SAM_TWI_IDR_RXBUFF      (1 << 14)
#define SAM_TWI_IDR_TXBUFF      (1 << 15)

/* TWI_IMR - Interrupt Mask Register */
#define SAM_TWI_IMR_TXCOMP      (1)
#define SAM_TWI_IMR_RXRDY       (1 << 1)
#define SAM_TWI_IMR_TXRDY       (1 << 2)

#define SAM_TWI_IMR_SVACC       (1 << 4)
#define SAM_TWI_IMR_GACC        (1 << 5)
#define SAM_TWI_IMR_OVRE        (1 << 6)

#define SAM_TWI_IMR_NACK        (1 << 8)
#define SAM_TWI_IMR_ARBLST      (1 << 9)
#define SAM_TWI_IMR_SCLWS       (1 << 10)
#define SAM_TWI_IMR_EOSACC      (1 << 11) 
#define SAM_TWI_IMR_ENDRX       (1 << 12)
#define SAM_TWI_IMR_ENDTX       (1 << 13)
#define SAM_TWI_IMR_RXBUFF      (1 << 14)
#define SAM_TWI_IMR_TXBUFF      (1 << 15)

/* TWI_RHR - RXDATA: Master or Slave Receive Holding Data*/
#define SAM_TWI_RHR_MASK        (0xff)

/* TWI_THR - TXDATA: Master or Slave Transmit Holding Data*/
#define SAM_TWI_THR_MASK        (0xff)


int i2c_port_module_init()
{
    return (0);
}

int i2c_port_init(struct i2c_driver_t *self_p,
                  struct i2c_device_t *dev_p,
                  int baudrate,
                  int address)
{
    self_p->dev_p = dev_p;
    self_p->twbr = baudrate;
    self_p->address = address; /* Perhaps only used for i2c slave mode? */
    return (0);
}

/* Example: i2c_port_set_clock( SAM_TWI1, 100000, 84000000); // 100KHz i2c clock, 84MHz CPU Clock */
static void i2c_port_set_clock( struct i2c_device_t *dev_p, uint32_t dwTwCk, uint32_t dwMCk )
{
    uint32_t dwCkDiv = 0 ;
    uint32_t dwClDiv ;
    uint32_t dwOk = 0 ;

    /* Configure clock */
    while ( !dwOk )
    {
        dwClDiv = ((dwMCk / (2 * dwTwCk)) - 4) / (1<<dwCkDiv) ;

        if ( dwClDiv <= 255 )
        {
            dwOk = 1 ;
        }
        else
        {
            dwCkDiv++ ;
        }
    }

    /* Typical Arduino Due: 100KHz => CKDIV = 1 and CLDIV/CHDIV = 208 */
    /* std_printf( FSTR("Using CKDIV = %u and CLDIV/CHDIV = %u\n\r"), dwCkDiv, dwClDiv ) ; */

    dev_p->regs_p->CWGR = 0 ;
    dev_p->regs_p->CWGR = (dwCkDiv << 16) | (dwClDiv << 8) | dwClDiv ;
}


uint8_t waitForTxcomp(struct i2c_device_t *dev_p)
{
  uint32_t status = 0;
  while ((status & SAM_TWI_SR_TXCOMP) != SAM_TWI_SR_TXCOMP) {
    status = (dev_p->regs_p->SR & 0xffff);
    if ((status & SAM_TWI_SR_NACK) == SAM_TWI_SR_NACK) {
      return 0;
    }
  }
  return 1;
}

uint8_t waitForTxrdy(struct i2c_device_t *dev_p)
{
  uint32_t status = 0;
  while ((status & SAM_TWI_SR_TXRDY) != SAM_TWI_SR_TXRDY) {
    status = (dev_p->regs_p->SR & 0xffff);
    if ((status & SAM_TWI_SR_NACK) == SAM_TWI_SR_NACK) {
      return 0;
    }
  }
  return 1;
}

uint8_t waitForRxrdy(struct i2c_device_t *dev_p)
{
  uint32_t status = 0;
  while ((status & SAM_TWI_SR_RXRDY) != SAM_TWI_SR_RXRDY) {
    status = (dev_p->regs_p->SR & 0xffff);
    if ((status & SAM_TWI_SR_NACK) == SAM_TWI_SR_NACK) {
      return 0;
    }  else if ((status & SAM_TWI_SR_OVRE) == SAM_TWI_SR_OVRE) {
      return 0;
    }
  }
  return 1;
}


uint32_t getStatus()
{
  uint32_t status = (SAM_TWI1->SR & 0xffff);
  /* watch for errors first */
  if ((status & SAM_TWI_SR_NACK) == SAM_TWI_SR_NACK) {
    return SAM_TWI_SR_NACK;
  } else if ((status & SAM_TWI_SR_GACC) == SAM_TWI_SR_GACC) {
    return SAM_TWI_SR_GACC;
  } else if ((status & SAM_TWI_SR_OVRE) == SAM_TWI_SR_OVRE) {
    return SAM_TWI_SR_OVRE;
  } else if ((status & SAM_TWI_SR_ARBLST) == SAM_TWI_SR_ARBLST) {
    return SAM_TWI_SR_ARBLST;
  } else if ((status & SAM_TWI_SR_TXCOMP) == SAM_TWI_SR_TXCOMP) {
    return SAM_TWI_SR_TXCOMP;
  } else if ((status & SAM_TWI_SR_RXRDY) == SAM_TWI_SR_RXRDY) {
    return SAM_TWI_SR_RXRDY;
  } else if ((status & SAM_TWI_SR_TXRDY) == SAM_TWI_SR_TXRDY) {
    return SAM_TWI_SR_TXRDY;
  }
  return 0; /* nothing interesting */
}




int i2c_port_stop(struct i2c_driver_t *self_p)
{
    /* TWI software reset */
    self_p->dev_p->regs_p->CR = SAM_TWI_CR_SWRST;
    self_p->dev_p->regs_p->RHR;

    /* Wait at least 10 ms */
    for (uint32_t i=0; i < 1000000; i++);

    /* TWI Slave Mode Disabled, TWI Master Mode Disabled*/
    self_p->dev_p->regs_p->CR = SAM_TWI_CR_SVDIS | SAM_TWI_CR_MSDIS;

    return (0);
}

int i2c_port_start(struct i2c_driver_t *self_p)
{
    self_p->dev_p->drv_p = self_p;
    self_p->thrd_p = NULL;

    uint32_t mask;
    struct i2c_device_t *dev_p = self_p->dev_p;
    volatile struct sam_pio_t *pio_p;

    /* Configure twd pin. */
    mask = dev_p->twd_p->mask;
    pio_p = dev_p->twd_p->pio_p;
    pio_p->PDR = mask; /* 1: Disables the PIO from controlling the corresponding pin (enables peripheral control of the pin) */
    pio_p->ABSR &= ~mask; /* 0: Peripheral A Enable */

    /* Configure twck pin. */
    mask = dev_p->twck_p->mask;
    pio_p = dev_p->twck_p->pio_p;
    pio_p->PDR = mask; /* 1: Disables the PIO from controlling the corresponding pin (enables peripheral control of the pin) */
    pio_p->ABSR &= ~mask;

    /* SVEN: TWI Slave Mode Enabled */
    dev_p->regs_p->CR = SAM_TWI_CR_SVEN;
    i2c_port_stop(self_p);
    
    /* MSEN: TWI Master Mode Enabled */
    dev_p->regs_p->CR = SAM_TWI_CR_MSEN;
    i2c_port_set_clock( dev_p, self_p->twbr, F_CPU);

    /* Disable Peripheral DMA transfers */
    #define UART_PTCR_RXTDIS (0x1u << 1) /**< \brief (UART_PTCR) Receiver Transfer Disable */
    #define UART_PTCR_TXTDIS (0x1u << 9) /**< \brief (UART_PTCR) Transmitter Transfer Disable */
    dev_p->regs_p->PTCR = UART_PTCR_RXTDIS | UART_PTCR_TXTDIS;

    /* self_p->dev_p->regs_p->IDR = (0xfffffffful); */ /* Disable TWI Interrupts */
    pmc_peripheral_clock_enable(dev_p->id);
    /*nvic_enable_interrupt(self_p->dev_p->id);  */

    return (0);
}
        




ssize_t i2c_port_read_txn(struct i2c_driver_t *self_p,
                      int address,
                      int internalAddress,
                      int internalAddressSize,
                      void *buf_p,
                      size_t size)
{
    struct i2c_device_t *dev_p = self_p->dev_p;
    uint8_t remaining = size;

    /* self_p->address = address; */ /* Why do we need to remember this? */
    self_p->buf_p = (void *)buf_p;
    self_p->size = size;
    self_p->thrd_p = thrd_self();


    /* Master Mode: Transfer Direction Bit (READ ===> bit MREAD 1) */
    dev_p->regs_p->MMR = ((address << 16) & SAM_TWI_MMR_DADR_MASK) | SAM_TWI_MMR_MREAD | ((internalAddressSize & 0b11) << 8);

    /* 5574912: 00000000 01010101 00010001 00000000 */
    switch(internalAddressSize) {
        case 0:
            dev_p->regs_p->IADR = 0x00;
            break;
        case 1:
            dev_p->regs_p->IADR = internalAddress & 0x000000ff;
            break;
        case 2:
            dev_p->regs_p->IADR = internalAddress & 0x0000ffff;
            break;
        case 3:
            dev_p->regs_p->IADR = internalAddress & 0x00ffffff;
            break;
    }
            
    /* Read byte, Set Stop Condition if 1 byte left */
    dev_p->regs_p->RHR;
    if (remaining == 1) {
      /* 1 Byte: TWI_CR = START | STOP */
      dev_p->regs_p->CR = SAM_TWI_CR_START | SAM_TWI_CR_STOP;
    } else {
      /* > 1  Byte: TWI_CR = START */
      dev_p->regs_p->CR = SAM_TWI_CR_START ;
    }


    int done = 0;
    uint8_t rhr;
    while(!done) {
      if (waitForRxrdy(dev_p)) {
        /* Read a byte */
        rhr = dev_p->regs_p->RHR; /* Automatically resets RXRDY */
        *(uint8_t*)buf_p = (uint8_t)(0xff & rhr);
        buf_p++;;
        /* 16 of 16 left */
        remaining--;
        /* 15 of 16 left */
        /* 1 of 16 left - send stop before last byte read */
        /* 0 of 16 left - get TXcomp */
        if(remaining == 1) {
            dev_p->regs_p->CR = SAM_TWI_CR_STOP; /* Issue a stop for last byte */
        }
      } else {
        done = 1;
      }
      if (remaining == 0) done = 1;
    }

    if (!waitForTxcomp(dev_p)) {
          /* std_printf(FSTR("i2c0 SAM_TWI_CR_STOP NACK\n")); */
    }
    return (size - remaining);
}

ssize_t i2c_port_read(struct i2c_driver_t *self_p,
                      int address,
                      void *buf_p,
                      size_t size)
{
    /* Set reasonable defaults for typcailly unused internalAddress / internalAddressSize */
    return i2c_port_read_txn(self_p, address, 0, 0, buf_p, size);
}


ssize_t i2c_port_write_txn(struct i2c_driver_t *self_p,
                      int address,
                      int internalAddress,
                      int internalAddressSize,
                      const void *buf_p,
                      size_t size)
{
    struct i2c_device_t *dev_p = self_p->dev_p;
    uint8_t remaining = size;
    uint8_t *wbuf_p = (uint8_t *)buf_p;


    /* self_p->address = address; */ /* Why do we need to remember this? */
    self_p->buf_p = (void *)buf_p;
    self_p->size = size;
    self_p->thrd_p = thrd_self();


    /* Master Mode: Transfer Direction Bit (READ ===> bit MREAD 1) */
    dev_p->regs_p->MMR = SAM_TWI1->MMR = ((address << 16) & SAM_TWI_MMR_DADR_MASK) | ((internalAddressSize & 0b11) << 8);

    /* 5574912: 00000000 01010101 00010001 00000000 */
    switch(internalAddressSize) {
        case 0:
            dev_p->regs_p->IADR = 0x00;
            break;
        case 1:
            dev_p->regs_p->IADR = internalAddress & 0x000000ff;
            break;
        case 2:
            dev_p->regs_p->IADR = internalAddress & 0x0000ffff;
            break;
        case 3:
            dev_p->regs_p->IADR = internalAddress & 0x00ffffff;
            break;
    }

    while(remaining > 0) {
      /* Note SAM will clock stretch while waiting for THR */
      SAM_TWI1->THR = *wbuf_p;

      if (!waitForTxrdy(dev_p)) {
          break;
      }

      if (remaining == 1) {
        /* 1 Byte: TWI_CR = START | STOP */
        SAM_TWI1->CR = SAM_TWI_CR_STOP;
        if (!waitForTxcomp(dev_p)) {
          /* std_printf(FSTR("i2c0 SAM_TWI_CR_STOP NACK\n")); */
          break;
        }
      }
      remaining--;
      wbuf_p++;
    }
    return (size - remaining);
}

ssize_t i2c_port_write(struct i2c_driver_t *self_p,
                       int address,
                       const void *buf_p,
                       size_t size)
{
    return i2c_port_write_txn(self_p, address, 0, 0, buf_p, size);
}

int i2c_port_scan(struct i2c_driver_t *self_p,
                  int address)
{
    /* Not yet implemented */
    return (0);
}

/* Future Periperhal DMA Controller
    33.8.7 Using the Peripheral DMA Controller (PDC)
        The use of the PDC significantly reduces the CPU load.
        To assure correct implementation, respect the following programming sequences:
    33.8.7.1 Data Transmit with the PDC
        1. Initialize the transmit PDC (memory pointers, size, etc.).
        2. Configure the master mode (DADR, CKDIV, etc.).
        3. Start the transfer by setting the PDC TXTEN bit.
        4. Wait for the PDC end TX flag.
        5. Disable the PDC by setting the PDC TXDIS bit.
    33.8.7.2 Data Receive with the PDC
        1. Initialize the receive PDC (memory pointers, size - 1, etc.).
        2. Configure the master mode (DADR, CKDIV, etc.).
        3. Start the transfer by setting the PDC RXTEN bit.
        4. Wait for the PDC end RX flag.
        5. Disable the PDC by setting the PDC RXDIS bit.
*/

int i2c_port_slave_start(struct i2c_driver_t *self_p)
{
    /* Not yet implemented */
    return (0);
}

int i2c_port_slave_stop(struct i2c_driver_t *self_p)
{
    /* Not yet implemented */
    return (0);
}

ssize_t i2c_port_slave_read(struct i2c_driver_t *self_p,
                            void *buf_p,
                            size_t size)
{
    /* Not yet implemented */
    return (0);
}

ssize_t i2c_port_slave_write(struct i2c_driver_t *self_p,
                             const void *buf_p,
                             size_t size)
{
    /* Not yet implemented */
    return(0);
}

