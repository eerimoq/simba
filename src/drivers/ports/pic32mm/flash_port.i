/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2018, Erik Moqvist
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

static void write_protect_unlock(volatile struct pic32mm_flash_t *regs_p)
{
    pic32mm_reg_write(&regs_p->NVMKEY, 0xaa996655);
    pic32mm_reg_write(&regs_p->NVMKEY, 0x556699aa);
}

static void write_protect_lock(volatile struct pic32mm_flash_t *regs_p)
{
    pic32mm_reg_write(&regs_p->NVMKEY, 0);
}

static void wait_for_operation_completed(volatile struct pic32mm_flash_t *regs_p)
{
    while ((pic32mm_reg_read(&regs_p->NVMCON) & PIC32MM_FLASH_NVMCON_WR) != 0);
}

static int commit(volatile struct pic32mm_flash_t *regs_p)
{
    int res;

    res = 0;

    write_protect_unlock(regs_p);

    pic32mm_reg_set(&regs_p->NVMCON, PIC32MM_FLASH_NVMCON_WR);

    wait_for_operation_completed(regs_p);

    if (pic32mm_reg_read(&regs_p->NVMCON) & PIC32MM_FLASH_NVMCON_WRERR) {
        res = -1;
    }

    pic32mm_reg_clr(&regs_p->NVMCON, PIC32MM_FLASH_NVMCON_WREN);

    write_protect_lock(regs_p);

    return (res);
}

static int flash_port_module_init(void)
{
    /* Unlock boot flash. */
    write_protect_unlock(PIC32MM_FLASH);
    pic32mm_reg_set(&PIC32MM_FLASH->NVMBWP, 0x8000);
    pic32mm_reg_clr(&PIC32MM_FLASH->NVMBWP, 0x0700);
    write_protect_lock(PIC32MM_FLASH);

    return (0);
}

static ssize_t flash_port_read(struct flash_driver_t *self_p,
                               void *dst_p,
                               size_t src,
                               size_t size)
{
    memcpy(dst_p, (void *)src, size);

    return (size);
}

static ssize_t flash_port_write(struct flash_driver_t *self_p,
                                uintptr_t dst,
                                const void *src_p,
                                size_t size)
{
    volatile struct pic32mm_flash_t *regs_p;
    size_t i;
    const uint32_t *u32_src_p;
    ssize_t res;

    res = size;
    u32_src_p = src_p;
    regs_p = self_p->dev_p->regs_p;
    regs_p = PIC32MM_FLASH;

    for (i = 0; (i < size) && (res == size); i += 8) {
        pic32mm_reg_write(&regs_p->NVMADDR, dst + i);
        pic32mm_reg_write(&regs_p->NVMDATA0, *u32_src_p++);
        pic32mm_reg_write(&regs_p->NVMDATA1, *u32_src_p++);
        pic32mm_reg_write(&regs_p->NVMCON,
                          (PIC32MM_FLASH_NVMCON_WREN
                           | PIC32MM_FLASH_NVMCON_NVMOP_DWORD_PROGRAM));

        if (commit(regs_p) != 0) {
            res = -EFLASHWRITE;
        }
    }

    return (res);
}

static int flash_port_erase(struct flash_driver_t *self_p,
                            uintptr_t addr,
                            size_t size)
{
    volatile struct pic32mm_flash_t *regs_p;
    uintptr_t end;
    int res;

    res = 0;
    regs_p = self_p->dev_p->regs_p;
    regs_p = PIC32MM_FLASH;

    end = ((addr + size - 1) & 0xfffff800);
    addr &= 0xfffff800;

    for (; (addr <= end) && (res == 0); addr += 0x800) {
        pic32mm_reg_write(&regs_p->NVMADDR, addr);
        pic32mm_reg_write(&regs_p->NVMCON,
                          (PIC32MM_FLASH_NVMCON_WREN
                           | PIC32MM_FLASH_NVMCON_NVMOP_PAGE_ERASE));

        if (commit(regs_p) != 0) {
            res = -EFLASHERASE;
        }
    }

    return (res);
}

int flash_async_write_row(struct flash_driver_t *self_p,
                          uintptr_t dst,
                          const void *src_p)
{
    volatile struct pic32mm_flash_t *regs_p;

    regs_p = self_p->dev_p->regs_p;
    regs_p = PIC32MM_FLASH;

    pic32mm_reg_write(&regs_p->NVMADDR, dst);
    pic32mm_reg_write(&regs_p->NVMSRCADDR,
                      PIC32MM_PHYSICAL_ADDRESS((uint32_t)src_p));
    pic32mm_reg_write(&regs_p->NVMCON,
                      (PIC32MM_FLASH_NVMCON_WREN
                       | PIC32MM_FLASH_NVMCON_NVMOP_ROW_PROGRAM));
    write_protect_unlock(regs_p);
    pic32mm_reg_set(&regs_p->NVMCON, PIC32MM_FLASH_NVMCON_WR);

    return (0);
}

int flash_async_wait(struct flash_driver_t *self_p)
{
    int res;
    volatile struct pic32mm_flash_t *regs_p;

    res = 0;
    regs_p = self_p->dev_p->regs_p;
    regs_p = PIC32MM_FLASH;

    wait_for_operation_completed(regs_p);

    if (pic32mm_reg_read(&regs_p->NVMCON) & PIC32MM_FLASH_NVMCON_WRERR) {
        res = -1;
    }

    pic32mm_reg_clr(&regs_p->NVMCON, PIC32MM_FLASH_NVMCON_WREN);

    write_protect_lock(regs_p);

    return (res);
}
