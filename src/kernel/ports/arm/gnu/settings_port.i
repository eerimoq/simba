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

#define PRIMARY 0
#define SECONDARY  1

extern uint8_t settings_area[2][CONFIG_SETTINGS_AREA_SIZE]
__attribute__ ((section (".settings")));

extern uint8_t settings_default_area[CONFIG_SETTINGS_AREA_SIZE]
__attribute__ ((section (".settings")));

static struct flash_driver_t drv;

static uint32_t calculate_area_crc(const uint8_t *area_p)
{
    uint32_t crc;
    uint8_t buf[256];
    size_t size;
    int i;

    /* Calculate the crc. */
    crc = 0;

    for (i = 0; i < CONFIG_SETTINGS_AREA_SIZE / sizeof(buf); i++) {
        flash_read(&drv,
                   buf,
                   (size_t)&area_p[i * sizeof(buf)],
                   sizeof(buf));

        size = sizeof(buf);

        /* Don't include the crc at the end of the area. */
        if (i == (CONFIG_SETTINGS_AREA_SIZE / sizeof(buf)) - 1) {
            size -= 4;
        }

        crc = crc_32(crc, buf, size);
    }

    return (crc);
}

/**
 * Check if the area is ok. That means, the crc of the content matches
 * the crc at the end of the area.
 *
 * return true(1) if the area is ok, otherwise false(0)
 */
static int is_area_ok(const uint8_t *area_p)
{
    uint32_t expected_crc, real_crc;

    /* Read the expected crc.*/
    flash_read(&drv,
               &expected_crc,
               (size_t)&area_p[SETTINGS_AREA_CRC_OFFSET],
               sizeof(expected_crc));

    real_crc = calculate_area_crc(area_p);

    return (expected_crc == real_crc);
}

static int copy_area(uint8_t *dst_p, const uint8_t *src_p)
{
    uint8_t buf[256];
    int i;

    for (i = 0; i < CONFIG_SETTINGS_AREA_SIZE / sizeof(buf); i++) {
        if (flash_read(&drv,
                       buf,
                       (size_t)&src_p[i * sizeof(buf)],
                       sizeof(buf)) != sizeof(buf)) {
            return (-1);
        }

        if (flash_write(&drv,
                        (size_t)&dst_p[i * sizeof(buf)],
                        buf,
                        sizeof(buf)) != sizeof(buf)) {
            return (-1);
        }
    }

    return (0);
}

static int settings_port_module_init(void)
{
    flash_init(&drv, &flash_0_dev);

    /* Is the primary area ok? */
    if (!is_area_ok(&settings_area[PRIMARY][0])) {
        /* Is the backup area ok? */
        if (!is_area_ok(&settings_area[SECONDARY][0])) {
            return (-1);
        }

        if (copy_area(&settings_area[PRIMARY][0], &settings_area[SECONDARY][0]) != 0) {
            return (-1);
        }
    }

    return (0);
}

static ssize_t settings_port_read(void *dst_p, size_t src, size_t size)
{
    return (flash_read(&drv, dst_p, (size_t)&settings_area[PRIMARY][src], size));
}

static ssize_t settings_port_write(size_t dst, const void *src_p, size_t size)
{
    uint32_t crc;

    /* Copy the primary area to the secondary area. */
    if (copy_area(&settings_area[SECONDARY][0], &settings_area[PRIMARY][0]) != 0) {
        return (-1);
    }

    /* Update the primary area. */
    if (flash_write(&drv, (size_t)&settings_area[PRIMARY][dst], src_p, size) != size) {
        return (-1);
    }

    /* Update the crc. */
    crc = calculate_area_crc(&settings_area[PRIMARY][0]);

    if (flash_write(&drv,
                    (size_t)&settings_area[PRIMARY][SETTINGS_AREA_CRC_OFFSET],
                    &crc,
                    sizeof(crc)) != sizeof(crc)) {
        return (-1);
    }

    return (size);
}

static ssize_t settings_port_reset()
{
    return (copy_area(&settings_area[PRIMARY][0], &settings_default_area[0]));
}
