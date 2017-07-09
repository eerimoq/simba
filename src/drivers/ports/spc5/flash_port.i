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

static int is_sector_in_range(uint32_t sector_address,
                              uint32_t sector_size,
                              uint32_t address,
                              uint32_t size)
{
    return !(((sector_address + sector_size) <= address)
             || (sector_address >= (address + size)));
}

static int write_aligned(struct flash_device_t *dev_p,
                         uint32_t *dst_p,
                         const uint32_t *src_p,
                         size_t size)
{
    int res;
    int i;
    volatile struct spc5_flash_t *regs_p;

    regs_p = dev_p->regs_p;
    res = 0;

    for (i = 0; (i < size / 4) && (res == 0); i += dev_p->program_size) {
        regs_p->MCR = SPC5_FLASH_MCR_PGM;
        dst_p[i] = src_p[i];
        /* For some reason double word program does not work in
           DFLASH. Does ECC still work? */
        if (dev_p->program_size == 2) {
            dst_p[i + 1] = src_p[i + 1];
        }

        regs_p->MCR = (SPC5_FLASH_MCR_PGM | SPC5_FLASH_MCR_EHV);

        while ((regs_p->MCR & SPC5_FLASH_MCR_DONE) == 0) {
#if CONFIG_SYSTEM_INTERRUPTS == 1
            thrd_yield();
#endif
        }

        res = ((regs_p->MCR & SPC5_FLASH_MCR_PEG) == 0 ? -1 : 0);
        regs_p->MCR = SPC5_FLASH_MCR_PGM;
        regs_p->MCR = 0;
    }

    return (res);
}

static int flash_port_module_init(void)
{
    /* Unlock CFLASH and DFLASH sectors for erase and programming. */
    SPC5_CFLASH->LML = 0xa1a11111;
    SPC5_CFLASH->SLL = 0xc3c33333;
    SPC5_DFLASH->LML = 0xa1a11111;
    SPC5_DFLASH->SLL = 0xc3c33333;

    return (0);
}

static ssize_t flash_port_read(struct flash_driver_t *self_p,
                               void *dst_p,
                               uintptr_t src,
                               size_t size)
{
    ssize_t res;

    memcpy(dst_p, (void *)src, size);

    /* Failed if an ECC error occured. */
    if (self_p->dev_p->regs_p->MCR & SPC5_FLASH_MCR_EER) {
        self_p->dev_p->regs_p->MCR = SPC5_FLASH_MCR_EER;
        res = -1;
    } else {
        res = size;
    }

    return (res);
}

static ssize_t flash_port_write(struct flash_driver_t *self_p,
                                uintptr_t dst,
                                const void *src_p,
                                size_t size)
{
    int res;
    const uint8_t *buf_p;
    struct flash_device_t *dev_p;
    volatile struct spc5_flash_t *regs_p;
    uint8_t buf[16];
    int pos;
    size_t left;
    size_t n;
    int i;
    size_t main_size;

    res = 0;
    dev_p = self_p->dev_p;
    regs_p = dev_p->regs_p;
    buf_p = src_p;
    pos = (dst & 0x7);
    left = size;

    /* Alignment buffer. */
    for (i = 0; i < membersof(buf); i++) {
        buf[i] = 0xff;
    }

    /* Unlock sectors. */
    regs_p->LML = 0xfffff000;
    regs_p->SLL = 0xfffff000;

    /* Alignment prologue. */
    if (pos != 0) {
        n = (left > 8 - pos ? 8 - pos : left);

        for (i = 0; i < n; i++) {
            buf[pos + i] = buf_p[i];
        }

        res = write_aligned(dev_p,
                            (uint32_t *)(dst & 0xfffffff8),
                            (uint32_t *)&buf[0],
                            8);
        dst += n;
        left -= n;
        buf_p += n;
    }

    if (res == 0) {
        if (left > 0) {
            /* Write the big chunk of data. */
            main_size = (left & 0xfffffff8);
            res = write_aligned(dev_p,
                                (uint32_t *)dst,
                                (uint32_t *)buf_p,
                                main_size);
            dst += main_size;
            left -= main_size;
            buf_p += main_size;

            if (res == 0) {
                /* Alignment epilogue. */
                if (left > 0) {

                    for (i = 0; i < left; i++) {
                        buf[8 + i] = buf_p[i];
                    }

                    res = write_aligned(dev_p,
                                        (uint32_t *)dst,
                                        (uint32_t *)&buf[8],
                                        8);
                }
            }
        }
    }

    /* Lock all sectors. */
    regs_p->LML = 0xffffffff;
    regs_p->SLL = 0xffffffff;

    return (res == 0 ? size : -1);
}

static int flash_port_erase(struct flash_driver_t *self_p,
                            uintptr_t address,
                            uint32_t size)
{
    int i;
    int res;
    uint32_t mask;
    struct flash_device_t *dev_p;
    volatile struct spc5_flash_t *regs_p;
    int32_t *sector_sizes_p;
    uint32_t offset;

    dev_p = self_p->dev_p;
    regs_p = dev_p->regs_p;
    res = 0;

    /* Create the erase mask. */
    mask = 0;
    offset = 0;
    sector_sizes_p = dev_p->sector_sizes_p;

    for (i = 0; sector_sizes_p[i] != -1; i++) {
        if (is_sector_in_range(dev_p->address + offset,
                               sector_sizes_p[i],
                               address,
                               size) == 1) {
            mask |= (1 << i);
        }

        offset += sector_sizes_p[i];
    }

    /* Unlock selected sectors. */
    regs_p->LML &= ~mask;
    regs_p->SLL &= ~mask;

    /* Erase selected sectors. */
    regs_p->MCR = SPC5_FLASH_MCR_ERS;
    regs_p->LMS = mask;
    *(uint32_t *)(address & 0xfffffff8) = 0xffffffff;
    regs_p->MCR = (SPC5_FLASH_MCR_EHV | SPC5_FLASH_MCR_ERS);

    while ((regs_p->MCR & SPC5_FLASH_MCR_DONE) == 0) {
#if CONFIG_SYSTEM_INTERRUPTS == 1
        thrd_yield();
#endif
    }

    res = ((regs_p->MCR & SPC5_FLASH_MCR_PEG) == 0 ? -1 : 0);
    regs_p->MCR = SPC5_FLASH_MCR_ERS;
    regs_p->MCR = 0;

    /* Lock selected sectors. */
    regs_p->LML |= mask;
    regs_p->SLL |= mask;

    return (res);
}
