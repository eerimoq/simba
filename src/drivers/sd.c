/**
 * @file sd.c
 * @version 1.0
 *
 * @section License
 * Copyright (C) 2014-2015, Erik Moqvist
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * This file is part of the Simba project.
 */

#include "simba.h"
#include "arpa/inet.h"

/* Command Abbreviations (section 4.7.4 in specification). */
#define CMD_GO_IDLE_STATE         0    /* Reset the SD Memory Card. */
#define CMD_ALL_SEND_CID          2    /* Sends host capacity support information. */
#define CMD_SEND_RELATIVE_ADDR    3    /* Checks switchable function. */
#define CMD_SET_DSR               4    /* Programs the DSR of all cards. */
#define CMD_SWITCH_FUNC           6    /* Checks switchable function. */
#define CMD_SELECT_DESELECT_CARD  7    /* Toggles card state (stdby/prog and disc). */
#define CMD_SEND_IF_COND          8    /* Sends SD Memory Card interface condition. */
#define CMD_SEND_CSD              9    /* Asks the selected card to send CSD. */
#define CMD_SEND_CID             10    /* Asks the selected card to send CID. */
#define CMD_VOLTAGE_SWITCH       11    /* Switch to 1V8 bus signaling level. */
#define CMD_STOP_TRANSMISSION    12    /* Stop Multiple Block Read. */
#define CMD_SEND_STATUS          13    /* Asks the selected card status register. */
#define CMD_GO_INACTIVE_STATE    15    /* Addressed card into inactive state. */
#define CMD_SET_BLOCKLEN         16    /* Set block length (in bytes). */
#define CMD_READ_SINGLE_BLOCK    17    /* Read block length bytes. */
#define CMD_READ_MULTIPLE_BLOCK  18    /* Read blocks until STOP_TRANSMISSION. */
#define CMD_SEND_TUNING_BLOCK    19    /* Send 64 byte tuning pattern to card. */
#define CMD_SPEED_CLASS_CONTROL  20    /* Check speed class. */
#define CMD_SET_BLOCK_COUNT      23    /* Specify block count for multiple block. */
#define CMD_WRITE_BLOCK          24    /* Write block length bytes. */
#define CMD_WRITE_MULTIPLE_BLOCK 25    /* Write block until STOP_TRANSMISSION. */
#define CMD_PROGRAM_CSD          27    /* Set programmable bits in CSD. */
#define CMD_SET_WRITE_PROT       28    /* Set write protect bit. */
#define CMD_CLR_WRITE_PROT       29    /* Clears write protect bit. */
#define CMD_SEND_WRITE_PROT      30    /* Read write protect bit. */
#define CMD_ERASE_WR_BLK_START   32    /* Set first write block to be erased. */
#define CMD_ERASE_WR_BLK_END     33    /* Set last write block to be erased. */
#define CMD_ERASE                38    /* Erases selected write blocks. */
#define CMD_LOCK_UNLOCK          40    /* Set/reset password or unlock/lock card. */
#define CMD_APP_CMD              55    /* Next command is application specific command. */
#define CMD_GEN_CMD              56    /* Data block for application specific command. */
#define CMD_READ_OCR             58    /* Read OCR register of a card. */
#define CMD_CRC_ON_OFF           59    /* Turns the CRC on/off. */

/* Table 4-28: Application-Specific Commands. */
#define ACMD_SET_BUS_WIDTH           6   /* Defines the data bus width. */
#define ACMD_SD_STATUS              13   /* Card status. */
#define ACMD_SEND_NUM_WR_BLOCKS     22   /* Number of write blocks. */
#define ACMD_SET_WR_BLK_ERASE_COUNT 23   /* Number of pre-erased write blocks. */
#define ACMD_SD_SEND_OP_COND        41   /* Host capacity support information. */
#define ACMD_SET_CLR_CARD_DETECT    42   /* Enable pull-up resistor on CD/DAT3 (pin 1). */
#define ACMD_SEND_SCR               51   /* Reads the Configuration Register. */

/* Card States. */
#define STATE_IDLE  0
#define STATE_READY 1
#define STATE_IDENT 2
#define STATE_STBY  3
#define STATE_TRAN  4
#define STATE_DATA  5
#define STATE_RCV   6
#define STATE_PRG   7
#define STATE_DIS   8

/* SD card type. */
#define TYPE_UNKNOWN 0
#define TYPE_SD1     1
#define TYPE_SD2     2
#define TYPE_SDHC    3

/* Responses. */

#define R1_IS_ERROR(r1) (((r1).as_uint8 & 0x7e) != 0)
#define R1_IS_READY(r1) ((r1).as_uint8 == 0)

/* R1 (Status). */
union r1_t {
    uint8_t as_uint8;
    struct {
        uint8_t in_idle_state : 1;
        uint8_t erase_reset : 1;
        uint8_t illegal_command : 1;
        uint8_t com_crc_error : 1;
        uint8_t erase_sequence_error : 1;
        uint8_t address_error : 1;
        uint8_t parameter_error : 1;
        uint8_t reserved : 1;
    };
};

/** R2 (Extended status). */
union r2_t {
    uint8_t as_uint8;
    struct {
        uint8_t card_is_locked : 1;
        uint8_t wp_erased : 1;
        uint8_t error : 1;
        uint8_t cc_error : 1;
        uint8_t card_ecc_failed : 1;
        uint8_t wp_violation : 1;
        uint8_t erase_param : 1;
        uint8_t out_of_range : 1;
    };
};

/** R3 (OCR register). */
struct r3_t {
    uint32_t vdd_voltage_window : 24;
    uint8_t switch_to_1V8_accepted : 1;
    uint8_t reserved : 4;
    uint8_t uhs_ii_card : 1;
    uint8_t card_capacity : 1;
    uint8_t card_power_up : 1;
};

/** R6 (Published RCA response). */
union r6_t {
    uint32_t as_uint32;
    struct {
        uint16_t rca;
        uint16_t status;
    };
};

/** R7 (Card interface condition). */
union r7_t {
    uint32_t as_uint32;
    struct {
        uint8_t check_pattern;
        uint8_t voltage_accepted : 4;
        uint32_t reserved : 16;
        uint8_t command_version : 4;
    };
};

/** SD initialization check pattern. */
#define CHECK_PATTERN 0xaa

#define TOKEN_DATA_START_BLOCK     0xfe
#define TOKEN_WRITE_MULTIPLE_TOKEN 0xfc
#define TOKEN_STOP_TRAN_TOKEN      0xfd
#define TOKEN_DATA_RES_MASK        0x1f
#define TOKEN_DATA_RES_ACCEPTED    0x05
#define TOKEN_DATA_RES_CRC_ERR     0x0b
#define TOKEN_DATA_RES_WR_ERR      0x0e

/* SD request block with command, arugment and check sum (CRC7). */
struct command_t {
    uint8_t index;
    uint32_t arg;
    uint8_t crc;
} PACKED;

/** Internal timeout periods. */
static const uint16_t INIT_TIMEOUT = 2000;
static const uint16_t ERASE_TIMEOUT = 10000;
static const uint16_t READ_TIMEOUT = 300;
static const uint16_t WRITE_TIMEOUT = 600;

/** Internal number of init pulses. */
static const uint8_t INIT_PULSES = 10;

/** Internal number of retry. */
static const uint8_t INIT_RETRY = 200;
static const uint8_t RESPONSE_RETRY = 100;

FAR static const uint16_t crctab[] = {
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
    0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
    0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
    0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
    0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
    0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
    0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
    0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
    0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
    0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
    0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
    0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
    0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
    0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
    0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
    0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
    0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
    0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
    0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
    0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
    0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
    0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
    0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
    0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
    0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
    0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
    0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
    0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
    0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
    0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
    0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
    0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0
};

static uint8_t crc7(const void* buf, size_t size)
{
    uint8_t* bp = (uint8_t*) buf;
    uint8_t crc = 0;
    uint8_t data;

    while (size--) {
        data = *bp++;
        data ^= crc << 1;

        if (data & 0x80) {
            data ^= 9;
        }

        crc = data ^ (crc & 0x78) ^ (crc << 4) ^ ((crc >> 3) & 0x0f);
    }

    crc = (crc << 1) ^ (crc << 4) ^ (crc & 0x70) ^ ((crc >> 3) & 0x0f);

    return (crc | 1);
}

/**
 * CRC-XMODEM calculation.
 *
 * Polynomial: x^16 + x^12 + x^5 + 1 (0x1021)
 */
static uint16_t crc_xmodem_update(uint16_t crc, uint8_t data)
{
    return (crctab[((crc >> 8) ^ data) & 0xff] ^ (crc << 8));
}

static uint16_t crc_xmodem(uint16_t crc, uint8_t *data_p, size_t size)
{
    while (size > 0) {
        crc = crc_xmodem_update(crc, *data_p++);
        size--;
    }

    return (crc);
}

static int wait_for_response(struct sd_driver_t *drv_p,
                             uint8_t mask,
                             uint8_t value,
                             uint8_t *response_p)
{
    do {
        if (spi_get(drv_p->spi_p, response_p) != 1) {
            return (-1);
        }
    } while ((*response_p & mask) != value);

    return (0);
}

/**
 * Send command index with given argument to SD card and wait for
 * response.
 */
static int command_call(struct sd_driver_t *drv_p,
                        uint8_t index,
                        uint32_t arg,
                        uint8_t *response_p)
{
    /* Build request with command, argument and add check-sum (CRC7) */
    struct command_t command;

    /* Initiate the command. */
    command.index = (0x40 | index);
    command.arg = htonl(arg);
    command.crc = crc7(&command, sizeof(command) - 1);

    /* Issue the command; wait while busy. */
    wait_for_response(drv_p, 0xff, 0xff, response_p);

    spi_write(drv_p->spi_p, &command, sizeof(command));

    /* Wait for the response. */
    wait_for_response(drv_p, 0x80, 0x0, response_p);

    return (0);
}

static uint8_t application_command_call(struct sd_driver_t *drv_p,
                                        uint8_t index,
                                        uint32_t arg,
                                        uint8_t *response_p)
{
    if (command_call(drv_p, CMD_APP_CMD, 0, response_p) != 0) {
        return (-1);
    }

    return (command_call(drv_p, index, arg, response_p));
}

static int read_command_response(struct sd_driver_t *drv_p,
                                 uint32_t *response_p)
{
    spi_read(drv_p->spi_p, response_p, sizeof(*response_p));

    *response_p = ntohl(*response_p);

    return (0);
}

static ssize_t read(struct sd_driver_t *drv_p,
                    uint8_t index,
                    void *dst_p,
                    size_t size)
{
    uint16_t real_crc, expected_crc;
    uint8_t response;

    /* Issue read command. */
    if (command_call(drv_p, index, 0, &response) != 0) {
        return (-1);
    }

    /* Receive the data block start token. */
    wait_for_response(drv_p, 0xff, TOKEN_DATA_START_BLOCK, &response);

    /* Receive the data and it's checksum. */
    spi_read(drv_p->spi_p, dst_p, size);    
    spi_read(drv_p->spi_p, &expected_crc, sizeof(expected_crc));

    /* Calculate the checksum of the received data. */
    real_crc = crc_xmodem(0, dst_p, size);
    expected_crc = ntohs(expected_crc);

    if (real_crc != expected_crc) {
        return (-1);
    } else {
        return (size);
    }
}

int sd_init(struct sd_driver_t *drv_p,
            struct spi_driver_t *spi_p)
{
    drv_p->spi_p = spi_p;

    return (0);
}

int sd_start(struct sd_driver_t *drv_p)
{
    uint32_t arg;
    union r1_t r1;
    union r7_t r7;
    int i;
    uint32_t ocr;
    int res;

    /* Start with unknown card type */
    drv_p->type = TYPE_UNKNOWN;

    /* Wait for at least one millisecond. */
    thrd_usleep(1000);

    /* Send 74 dummy clock pulses. */
    for (i = 0; i < 10; i++) {
        spi_put(drv_p->spi_p, 0xff);
    }

    /* Reset card. */
    if (command_call(drv_p, CMD_GO_IDLE_STATE, 0, &r1.as_uint8) != 0) {
        return (-1);
    }

    /* Enable CRC. */
    if (command_call(drv_p, CMD_CRC_ON_OFF, 1, &r1.as_uint8) != 0) {
        return (-1);
    }

    if (R1_IS_ERROR(r1)) {
        return (-1);
    }

    /* Check for version of SD card specification; 2.7-3.6V and check
       pattern. */
    drv_p->type = TYPE_SD1;
    arg = (0x100 | CHECK_PATTERN);

    res = command_call(drv_p, CMD_SEND_IF_COND, arg, &r1.as_uint8);

    /* SD type 1 on error or no response. */
    if ((res == 0) && !R1_IS_ERROR(r1)) {
        if (read_command_response(drv_p, &r7.as_uint32) != 0) {
            /* SD1 not supported. */
            return (-1);
        }

        if (r7.check_pattern != CHECK_PATTERN) {
            return (-1);
        }
        
        drv_p->type = TYPE_SD2;
    }

    /* Tell the device that the host supports SDHC. */
    arg = (drv_p->type == TYPE_SD1 ? 0L : 0x40000000L);

    for (i = 0; i < 50; i++) {
        if (application_command_call(drv_p,
                                     ACMD_SD_SEND_OP_COND,
                                     arg,
                                     &r1.as_uint8) != 0) {
            return (-1);
        }

        if (r1.in_idle_state == 0) {
            break;
        }

        thrd_usleep(5000);
    }

    /* Read OCR register and check type. */
    if (command_call(drv_p, CMD_READ_OCR, 0, &r1.as_uint8) != 0) {
        return (-1);
    }

    if (read_command_response(drv_p, &ocr) != 0) {
            return (-1);
    }
    
    if ((ocr & 0xc0000000L) == 0xc0000000L) {
        drv_p->type = TYPE_SDHC;
    }
    
    return (0);
}

int sd_stop(struct sd_driver_t *drv_p)
{
    return (0);
}

ssize_t sd_read_cid(struct sd_driver_t *drv_p,
                    struct sd_cid_t *cid_p)
{
    return (read(drv_p, CMD_SEND_CID, cid_p, sizeof(*cid_p)));
}

ssize_t sd_read_csd(struct sd_driver_t *drv_p,
                    union sd_csd_t *csd_p)
{
    return (read(drv_p, CMD_SEND_CSD, csd_p, sizeof(*csd_p)));
}

int sd_erase_blocks(struct sd_driver_t *drv_p,
                    uint32_t start,
                    uint32_t end)
{
    /* Check if block address should be mapped to byte address */
    if (drv_p->type != TYPE_SDHC) {
        start <<= 9;
        end <<= 9;
    }
#if 0
    /* Send commands for block erase */
    if (send(ERASE_WR_BLK_START, start)) {
        return (-1);
    }

    if (send(ERASE_WR_BLK_END, end)) {
        return (-1);
    }

    if (send(ERASE)) {
        return (-1);
    }

    if (!await(ERASE_TIMEOUT)) {
        return (-1);
    }
#endif
    return (0);
}

ssize_t sd_read_block(struct sd_driver_t *drv_p,
                      void *dst_p,
                      uint32_t src_block)
{
    if (drv_p->type != TYPE_SDHC) {
        src_block <<= 9;
    }
    
    return (read(drv_p, CMD_READ_SINGLE_BLOCK, dst_p, SD_BLOCK_SIZE));
}

ssize_t sd_write_block(struct sd_driver_t *drv_p,
                       uint32_t dst_block,
                       const void *src_p)
{
    /* uint16_t crc; */
    /* uint8_t status; */

    /* Check for byte address adjustment */
    if (drv_p->type != TYPE_SDHC) {
        dst_block <<= 9;
    }
#if 0
    /* Calculate the checksum of the reseiced data. */
    crc = crc_xmodem(0, src_p, SD_BLOCK_SIZE);

    /* Issue write block command, transfer block, calculate check sum. */
    if (command_call(drv_p, CMD_WRITE_BLOCK, dst_block)) {
        return (-1);
    }

    spi_put(drv_p->spi_p, TOKEN_DATA_START_BLOCK);

    /* Write the data and it's checksum. */
    spi_write(drv_p->spi_p, src_p, size);    
    spi_write(drv_p->spi_p, &crc, sizeof(crc));

    spi_get(drv_p->spi_p, &status);

    if ((status & TOKEN_DATA_RES_MASK) != TOKEN_DATA_RES_ACCEPTED) {
        return (-1);
    }

    /* Wait for the write operation to complete and check status */
    if (!wait_until_idle(WRITE_TIMEOUT)) {
        return (-1);
    }
    
    status = send(CMD_SEND_STATUS);

    if (status != 0) {
        goto error;
    }

    spi_get(&status);
#endif

    return (-1);
}
