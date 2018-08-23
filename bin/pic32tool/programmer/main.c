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

#include "simba.h"

/* ICSP pin devices. */
#define pin_pgec_dev   pin_d2_dev
#define pin_pged_dev   pin_d3_dev
#define pin_mclrn_dev  pin_d4_dev

/* Device status. */
#define STATUS_CPS     reverse_8(BIT(7))
#define STATUS_NVMERR  reverse_8(BIT(5))
#define STATUS_CFGRDY  reverse_8(BIT(3))
#define STATUS_FCBUSY  reverse_8(BIT(2))
#define STATUS_DEVRST  reverse_8(BIT(0))

#define CONTROL_PRACC     reverse_32(BIT(18))
#define CONTROL_PROBEN    reverse_32(BIT(15))
#define CONTROL_PROBTRAP  reverse_32(BIT(14))
#define CONTROL_EJTAGBRK  reverse_32(BIT(12))

/* MCHP TAP instructions. */
#define MTAP_COMMAND    reverse_8(0x07)
#define MTAP_SW_MTAP    reverse_8(0x04)
#define MTAP_SW_ETAP    reverse_8(0x05)
#define MTAP_IDCODE     reverse_8(0x01)

/* EJTAG TAP instructions. */
#define ETAP_ADDRESS    reverse_8(0x08)
#define ETAP_DATA       reverse_8(0x09)
#define ETAP_CONTROL    reverse_8(0x0a)
#define ETAP_EJTAGBOOT  reverse_8(0x0c)
#define ETAP_FASTDATA   reverse_8(0x0e)

/* MTAP commands. */
#define MCHP_STATUS         reverse_8(0x00)
#define MCHP_ASSERT_RST     reverse_8(0xd1)
#define MCHP_DE_ASSERT_RST  reverse_8(0xd0)
#define MCHP_ERASE          reverse_8(0xfc)

/* Protocol. */
#define TYPE_SIZE                                           2
#define SIZE_SIZE                                           2
#define MAXIMUM_PAYLOAD_SIZE                             1024
#define CRC_SIZE                                            2

#define PAYLOAD_OFFSET                (TYPE_SIZE + SIZE_SIZE)

/* Command types. */
#define COMMAND_TYPE_FAILED                                -1
#define COMMAND_TYPE_PING                                 100
#define COMMAND_TYPE_CONNECT                              101
#define COMMAND_TYPE_DISCONNECT                           102

static int connected = 0;
static struct icsp_soft_driver_t icsp;
static uint8_t buf[PAYLOAD_OFFSET + MAXIMUM_PAYLOAD_SIZE + CRC_SIZE + 2];
static uint32_t ramapp_upload_instructions[] = {
#include "ramapp_upload_instructions.i"
};

static uint8_t reverse_8(uint8_t v)
{
    v = (((v & 0xaa) >> 1) | ((v & 0x55) << 1));
    v = (((v & 0xcc) >> 2) | ((v & 0x33) << 2));

    return (((v & 0xf0) >> 4) | ((v & 0x0f) << 4));
}

static uint32_t reverse_32(uint32_t v)
{
    v = (((v & 0xaaaaaaaa) >> 1) | ((v & 0x55555555) << 1));
    v = (((v & 0xcccccccc) >> 2) | ((v & 0x33333333) << 2));
    v = (((v & 0xf0f0f0f0) >> 4) | ((v & 0x0f0f0f0f) << 4));
    v = (((v & 0xff00ff00) >> 8) | ((v & 0x00ff00ff) << 8));

    return ((v >> 16) | (v << 16));
}

static int send_command(struct icsp_soft_driver_t *icsp_p,
                        uint8_t command)
{
    int res;

    res = icsp_soft_instruction_write(icsp_p, &command, 5);

    return (res);
}

static int xfer_data_32(struct icsp_soft_driver_t *icsp_p,
                        uint32_t request,
                        uint32_t *response_p)
{
    int res;

    request = htonl(request);

    res = icsp_soft_data_transfer(icsp_p,
                                  (uint8_t *)response_p,
                                  (uint8_t *)&request,
                                  32);

    if (res != 0) {
        return (res);
    }

    *response_p = ntohl(*response_p);

    return (0);
}

static int xfer_fast_data_32(struct icsp_soft_driver_t *icsp_p,
                             uint32_t request,
                             uint32_t *response_p)
{
    int res;

    request = htonl(request);

    res = icsp_soft_fast_data_transfer(icsp_p,
                                       (uint8_t *)response_p,
                                       (uint8_t *)&request,
                                       32);

    if (res != 0) {
        return (res);
    }

    *response_p = ntohl(*response_p);

    return (res);
}

static int xfer_instruction(struct icsp_soft_driver_t *icsp_p,
                            uint32_t instruction)
{
    int res;
    uint32_t response;

    res = send_command(icsp_p, ETAP_CONTROL);

    if (res != 0) {
        return (res);
    }

    do {
        res = xfer_data_32(icsp_p,
                           reverse_32(0x0004c000),
                           &response);

        if (res != 0) {
            return (res);
        }
    } while (!(response & CONTROL_PRACC));

    res = send_command(icsp_p, ETAP_DATA);

    if (res != 0) {
        return (res);
    }

    res = xfer_data_32(icsp_p, reverse_32(instruction), &response);

    if (res != 0) {
        return (res);
    }

    res = send_command(icsp_p, ETAP_CONTROL);

    if (res != 0) {
        return (res);
    }

    res = xfer_data_32(icsp_p, reverse_32(0x0000c000), &response);

    return (res);
}

static int enter_serial_execution_mode(struct icsp_soft_driver_t *icsp_p)
{
    int res;
    uint8_t command;
    uint8_t status;

    res = send_command(icsp_p, MTAP_SW_MTAP);

    if (res != 0) {
        return (res);
    }

    res = send_command(icsp_p, MTAP_COMMAND);

    if (res != 0) {
        return (res);
    }

    command = MCHP_STATUS;
    res = icsp_soft_data_transfer(icsp_p, &status, &command, 8);

    if (res != 0) {
        return (res);
    }

    if ((status & STATUS_CPS) == 0) {
        return (-EPROTO);
    }

    command = MCHP_ASSERT_RST;
    res = icsp_soft_data_write(icsp_p, &command, 8);

    if (res != 0) {
        return (res);
    }

    res = send_command(icsp_p, MTAP_SW_ETAP);

    if (res != 0) {
        return (res);
    }

    res = send_command(icsp_p, ETAP_EJTAGBOOT);

    if (res != 0) {
        return (res);
    }

    res = send_command(icsp_p, MTAP_SW_MTAP);

    if (res != 0) {
        return (res);
    }

    res = send_command(icsp_p, MTAP_COMMAND);

    if (res != 0) {
        return (res);
    }

    command = MCHP_DE_ASSERT_RST;
    res = icsp_soft_data_write(icsp_p, &command, 8);

    if (res != 0) {
        return (res);
    }

    res = send_command(icsp_p, MTAP_SW_ETAP);

    return (res);
}

static int upload_ramapp(struct icsp_soft_driver_t *icsp_p)
{
    int res;
    size_t i;

    res = 0;

    for (i = 0; i < membersof(ramapp_upload_instructions); i++) {
        res = xfer_instruction(icsp_p, ramapp_upload_instructions[i]);

        if (res != 0) {
            break;
        }
    }

    /* Start the uploaded application. */
    if (res == 0) {
        res = xfer_instruction(icsp_p, 0x00000000);
    }

    return (res);
}

#if 0

static int read_from_address(struct icsp_soft_driver_t *icsp_p,
                             uint32_t address,
                             uint32_t *response_p)
{
    int res;

    /* Load Fast Data register address to s3. */
    res = xfer_instruction(icsp_p, 0xff2041b3);

    if (res != 0) {
        return (res);
    }

    /* Load memory address to be read into t0. */
    res = xfer_instruction(icsp_p, (address & 0xffff0000) | 0x41a8);

    if (res != 0) {
        return (res);
    }

    res = xfer_instruction(icsp_p, (address << 16) | 0x5108);

    if (res != 0) {
        return (res);
    }

    /* Read data. */
    res = xfer_instruction(icsp_p, 0x0000fd28);

    if (res != 0) {
        return (res);
    }

    /* Store data into Fast Data register. */
    res = xfer_instruction(icsp_p, 0x0000f933);

    if (res != 0) {
        return (res);
    }

    res = xfer_instruction(icsp_p, 0x00000000);

    if (res != 0) {
        return (res);
    }

    /* Shift out the data. */
    res = send_command(icsp_p, ETAP_FASTDATA);

    if (res != 0) {
        return (res);
    }

    res = xfer_fast_data_32(icsp_p, 0, response_p);

    return (res);
}

static int read_device_status(struct icsp_soft_driver_t *icsp_p)
{
    int res;
    uint8_t command;
    uint8_t status;

    res = send_command(icsp_p, MTAP_SW_MTAP);

    if (res != 0) {
        return (res);
    }

    res = send_command(icsp_p, MTAP_COMMAND);

    if (res != 0) {
        return (res);
    }

    command = MCHP_STATUS;
    res = icsp_soft_data_transfer(icsp_p, &status, &command, 8);

    if (res != 0) {
        return (res);
    }

    return (status);
}

static void print_device_status(struct icsp_soft_driver_t *icsp_p,
                                void *out_p)
{
    int status;

    status = read_device_status(icsp_p);

    if (status < 0) {
        std_fprintf(out_p,
                    OSTR("Failed to read device status with %d.\r\n"),
                    status);
    } else {
        std_fprintf(out_p,
                    OSTR("STATUS: 0x%02x\r\n"
                         "  CPS:    %d\r\n"
                         "  NVMERR: %d\r\n"
                         "  CFGRDY: %d\r\n"
                         "  FCBUSY: %d\r\n"
                         "  DEVRST: %d\r\n"),
                    reverse_8(status),
                    (status & STATUS_CPS) != 0,
                    (status & STATUS_NVMERR) != 0,
                    (status & STATUS_CFGRDY) != 0,
                    (status & STATUS_FCBUSY) != 0,
                    (status & STATUS_DEVRST) != 0);
    }
}

static int read_device_id(struct icsp_soft_driver_t *icsp_p,
                          uint32_t *device_id_p)
{
    return (xfer_data_32(icsp_p, 0, device_id_p));
}

 void print_device_id(struct icsp_soft_driver_t *icsp_p,
                            void *out_p)
{
    int res;
    uint32_t device_id;

    res = read_device_id(icsp_p, &device_id);

    if (res < 0) {
        std_fprintf(out_p,
                    OSTR("Failed to read device id with %d.\r\n"),
                    res);
    } else {
        std_fprintf(out_p,
                    OSTR("DEVICE ID: 0x%08x\r\n"),
                    reverse_32(device_id));
    }
}

static void print_device_configuration(struct icsp_soft_driver_t *icsp_p,
                                       void *out_p)
{
    uint32_t i;
    uint32_t response;
    int res;

    for (i = 0; i < 10; i++) {
        res = read_from_address(icsp_p, 0xbfc017c0 + 4 * i, &response);

        if (res == 0) {
            std_printf(OSTR("[%2d]: 0x%08x\r\n"), i, reverse_32(response));
        } else {
            std_printf(
                OSTR("Failed to read device configuration with %d.\r\n"),
                res);
        }
    }
}

#endif

static ssize_t ramapp_read(uint8_t *buf_p)
{
    uint32_t data;
    size_t size;
    size_t number_of_words;
    size_t i;

    /* Read type and size. */
    if (xfer_fast_data_32(&icsp, 0, &data) != 0) {
        return (-EPROTO);
    }

    data = reverse_32(data);

    buf_p[0] = (data >> 24);
    buf_p[1] = (data >> 16);
    buf_p[2] = (data >> 8);
    buf_p[3] = (data >> 0);

    size = ((buf_p[2] << 8) | buf_p[3]);

    if (size > MAXIMUM_PAYLOAD_SIZE) {
        return (-EPROTO);
    }

    /* Read payload and crc. */
    number_of_words = DIV_CEIL(size + CRC_SIZE, 4);

    for (i = 0; i < number_of_words; i++) {
        if (xfer_fast_data_32(&icsp, 0, &data) != 0) {
            return (-EPROTO);
        }

        data = reverse_32(data);
        buf_p[4 * i + 4] = (data >> 24);
        buf_p[4 * i + 5] = (data >> 16);
        buf_p[4 * i + 6] = (data >> 8);
        buf_p[4 * i + 7] = (data >> 0);
    }

    return (PAYLOAD_OFFSET + size + CRC_SIZE);
}

static ssize_t ramapp_write(uint8_t *buf_p, size_t size)
{
    uint32_t data;
    size_t number_of_words;
    size_t i;
    int res;

    number_of_words = DIV_CEIL(size, 4);

    for (i = 0; i < number_of_words; i++) {
        data = ((buf_p[4 * i + 0] << 24)
                | (buf_p[4 * i + 1] << 16)
                | (buf_p[4 * i + 2] << 8)
                | (buf_p[4 * i + 3] << 0));

        res = xfer_fast_data_32(&icsp, reverse_32(data), &data);

        if (res != 0) {
            return (res);
        }
    }

    return (size);

}

static ssize_t handle_ramapp_command(uint8_t *buf_p, size_t size)
{
    ssize_t res;

    res = -ENOTCONN;

    if (connected) {
        res = ramapp_write(buf_p, size);

        if (res != size) {
            return (res);
        }

        res = ramapp_read(buf_p);
    }

    return (res);
}

static ssize_t handle_connect(uint8_t *buf_p, size_t size)
{
    int res;

    if (connected) {
        return (-EISCONN);
    }

    icsp_soft_init(&icsp,
                   &pin_pgec_dev,
                   &pin_pged_dev,
                   &pin_mclrn_dev);
    icsp_soft_start(&icsp);

    res = enter_serial_execution_mode(&icsp);

    if (res != 0) {
        return (res);
    }

    res = upload_ramapp(&icsp);

    if (res != 0) {
        return (res);
    }

    res = send_command(&icsp, ETAP_FASTDATA);

    connected = 1;

    return (res);
}

static ssize_t handle_disconnect(uint8_t *buf_p, size_t size)
{
    if (!connected) {
        return (-ENOTCONN);
    }

    icsp_soft_stop(&icsp);

    connected = 0;

    return (0);
}

static ssize_t prepare_command_response(uint8_t *buf_p,
                                        ssize_t size)
{
    uint16_t crc;

    /* Failure. */
    if (size < 0) {
        buf_p[0] = (COMMAND_TYPE_FAILED >> 8);
        buf_p[1] = COMMAND_TYPE_FAILED;
        buf_p[4] = (size >> 24);
        buf_p[5] = (size >> 16);
        buf_p[6] = (size >> 8);
        buf_p[7] = (size >> 0);
        size = 4;
    }

    buf_p[2] = (size >> 8);
    buf_p[3] = size;

    size += PAYLOAD_OFFSET;

    crc = crc_ccitt(0xffff, buf_p, size);

    buf_p[size] = (crc >> 8);
    buf_p[size + 1] = crc;
    size += CRC_SIZE;

    return (size);
}

static ssize_t handle_programmer_command(int type,
                                         uint8_t *buf_p,
                                         size_t size)
{
    ssize_t res;
    uint16_t actual_crc;
    uint16_t expected_crc;

    res = -EBADCRC;
    actual_crc = ((buf_p[size - CRC_SIZE] << 8)
                  | buf_p[size - CRC_SIZE + 1]);
    expected_crc = crc_ccitt(0xffff, &buf_p[0], size - CRC_SIZE);

    if (actual_crc == expected_crc) {
        switch (type) {

        case COMMAND_TYPE_PING:
            res = 0;
            break;

        case COMMAND_TYPE_CONNECT:
            res = handle_connect(buf_p, size);
            break;

        case COMMAND_TYPE_DISCONNECT:
            res = handle_disconnect(buf_p, size);
            break;

        default:
            res = -1;
            break;
        }
    }

    return (prepare_command_response(buf_p, res));
}

static ssize_t handle_command(uint8_t *buf_p, size_t size)
{
    ssize_t res;
    int type;

    type = ((buf_p[0] << 8) | buf_p[1]);

    if (type < 100) {
        res = handle_ramapp_command(buf_p, size);

        if (res < 0) {
            res = prepare_command_response(buf_p, res);
        }
    } else {
        res = handle_programmer_command(type, buf_p, size);
    }

    return (res);
}

static ssize_t read_command_request(uint8_t *buf_p)
{
    ssize_t size;

    /* Read type and size. */
    chan_read(sys_get_stdin(), &buf_p[0], PAYLOAD_OFFSET);

    size = ((buf_p[2] << 8) | buf_p[3]);

    if (size > MAXIMUM_PAYLOAD_SIZE) {
        return (-EMSGSIZE);
    }

    /* Read payload and crc. */
    chan_read(sys_get_stdin(), &buf_p[4], size + CRC_SIZE);

    return (PAYLOAD_OFFSET + size + CRC_SIZE);
}

static void write_command_response(uint8_t *buf_p, ssize_t size)
{
    if (size > 0) {
        chan_write(sys_get_stdout(), &buf_p[0], size);
    }
}

int main()
{
    ssize_t size;

    sys_start();

    while (1) {
        size = read_command_request(&buf[0]);

        if (size >= 0) {
            size = handle_command(&buf[0], size);
        }

        write_command_response(&buf[0], size);
    }

    return (0);
}
