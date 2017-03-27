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

#include "simba.h"

#if CONFIG_SD == 1

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
#define R1_IDLE_STATE                 0x01
#define R1_ERASE_RESET                0x02
#define R1_ILLEGAL_COMMAND            0x04
#define R1_COM_CRC_ERROR              0x08
#define R1_ERASE_SEQUENCE_ERROR       0x10
#define R1_ADDRESS_ERROR              0x20
#define R1_PARAMETER_ERROR            0x40
#define R1_RESERVED                   0x80

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
};

/** Internal timeout periods. */
#define WRITE_TIMEOUT      2000

/** Internal number of init pulses. */
#define INIT_PULSES          10

/** Internal number of retry. */
#define RESPONSE_RETRIES   2000

/**
 * Wait for the the data start block
 */
static int wait_for_data_start_block(struct sd_driver_t *self_p)
{
    int attempt;
    uint8_t data;

    attempt = 0;

    while (attempt < RESPONSE_RETRIES) {
        if (spi_get(self_p->spi_p, &data) != 1) {
            return (-1);
        }

        if (data == TOKEN_DATA_START_BLOCK) {
            return (0);
        }

        attempt++;
    }

    return (-SD_ERR_NORESPONSE_WAIT_FOR_DATA_START_BLOCK);
}

/**
 * Wait until the SD card is not busy, or until a timeout occurs.
 */
static int wait_not_busy(struct sd_driver_t *self_p,
                         int timeout_ms)
{
    int i;
    uint8_t status;

    /* Wait for the card to be idle. */
    for (i = 0; i < timeout_ms; i++) {
        if (spi_get(self_p->spi_p, &status) != 1) {
            return (-1);
        }

        if (status == 0xff) {
            return (0);
        }

        if (i >= 10) {
            thrd_sleep_us(1000);
        }
    }

    return (-1);
}

/**
 * Send command index with given argument to SD card.
 */
static int command_write(struct sd_driver_t *self_p,
                         uint8_t index,
                         uint32_t arg)
{
    struct command_t command;

    /* Wait for the card to be idle. */
    wait_not_busy(self_p, 300);

    /* Initiate the command. */
    command.index = (0x40 | index);
    command.arg = htonl(arg);
    command.crc = crc_7(&command, sizeof(command) - 1);

    if (spi_write(self_p->spi_p,
                  &command,
                  sizeof(command)) != sizeof(command)) {
        return (-1);
    }

    return (0);
}

/**
 * Send command index with given argument to SD card and wait for the
 * response a response with only the idle bit set.
 */
static int command_call(struct sd_driver_t *self_p,
                        uint8_t index,
                        uint32_t arg,
                        uint8_t *response_p)
{
    int i;

    if (command_write(self_p, index, arg) != 0) {
        return (-1);
    }

    /* Wait for the response. If bit 7 is one(1) the slave did not
       answer. */
    for (i = 0; i < RESPONSE_RETRIES; i++) {
        if (spi_get(self_p->spi_p, response_p) != 1) {
            return (-1);
        }

        if ((*response_p & R1_RESERVED) == 0) {
            return (0);
        }

        if (i >= 10) {
            thrd_sleep_us(100);
        }
    }

    return (-1);
}

static int command_check_call(struct sd_driver_t *self_p,
                              uint8_t index,
                              uint32_t arg,
                              uint8_t status)
{
    uint8_t response;

    if (command_call(self_p, index, arg, &response) != 0) {
        return (-1);
    }

    return (response != status);
}

static int command_check_call_idle(struct sd_driver_t *self_p,
                                   uint8_t index,
                                   uint32_t arg)
{
    return (command_check_call(self_p, index, arg, R1_IDLE_STATE));
}

/**
 * Execute given application command.
 */
static uint8_t application_command_call(struct sd_driver_t *self_p,
                                        uint8_t index,
                                        uint32_t arg,
                                        uint8_t *response_p)
{
    if (command_check_call_idle(self_p, CMD_APP_CMD, 0) != 0) {
        return (-1);
    }

    return (command_call(self_p, index, arg, response_p));
}

/**
 * Read from the SD card.
 */
static ssize_t read(struct sd_driver_t *self_p,
                    uint8_t index,
                    uint32_t arg,
                    void *dst_p,
                    size_t size)
{
    uint16_t real_crc, expected_crc;
    ssize_t res;

    spi_take_bus(self_p->spi_p);
    spi_select(self_p->spi_p);

    /* Issue read command. */
    if (command_check_call(self_p, index, arg, 0) != 0) {
        res = -SD_ERR_READ_COMMAND;
        goto out;
    }

    /* Receive the data block start token. */
    if (wait_for_data_start_block(self_p) != 0) {
        res = -SD_ERR_READ_DATA_START_BLOCK;
        goto out;
    }

    /* Receive the data and it's checksum. */
    spi_read(self_p->spi_p, dst_p, size);
    spi_read(self_p->spi_p, &expected_crc, sizeof(expected_crc));

    /* Calculate the checksum of the received data. */
    real_crc = crc_xmodem(0, dst_p, size);
    expected_crc = ntohs(expected_crc);

    if (real_crc != expected_crc) {
        res = -SD_ERR_READ_WRONG_DATA_CRC;
        goto out;
    }

    res = size;

 out:
    spi_deselect(self_p->spi_p);
    spi_give_bus(self_p->spi_p);

    return (res);
}

int sd_init(struct sd_driver_t *self_p,
            struct spi_driver_t *spi_p)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(spi_p != NULL, EINVAL);

    self_p->spi_p = spi_p;

    return (0);
}

int sd_start(struct sd_driver_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    uint32_t arg;
    int i;
    int res;
    uint8_t buf[8];
    uint8_t response;

    res = -1;

    /* Start with unknown card type */
    self_p->type = TYPE_UNKNOWN;

    /* Wait for at least one millisecond. */
    thrd_sleep_us(1000);

    /* Start SPI and take ownership of the spi bus. */
    spi_start(self_p->spi_p);
    spi_take_bus(self_p->spi_p);

    /* Send 74 dummy clock pulses with the slave deselected. */
    for (i = 0; i < INIT_PULSES; i++) {
        spi_put(self_p->spi_p, 0xff);
    }

    /* Select the slave once for all initialization data transfers. */
    spi_select(self_p->spi_p);

    /* Reset the card. */
    if (command_check_call_idle(self_p, CMD_GO_IDLE_STATE, 0) != 0) {
        res = -SD_ERR_GO_IDLE_STATE;
        goto out;
    }

    /* Enable CRC. Host should enable CRC verification before issuing
       ACMD41. */
    if (command_check_call_idle(self_p, CMD_CRC_ON_OFF, 1) != 0) {
        res = -SD_ERR_CRC_ON_OFF;
        goto out;
    }

    /* Check for version of SD card specification; 2.7-3.6V and check
       pattern. */
    arg = (0x100 | CHECK_PATTERN);

    /* SD type 1 if no response is received. */
    if (command_check_call_idle(self_p, CMD_SEND_IF_COND, arg) != 0) {
        res = -SD_ERR_SEND_IF_COND;
        goto out;
    }

    /* Check if the check pattern is correct. */
    if (spi_read(self_p->spi_p, &buf[0], 4) != 4) {
        goto out;
    }

    if (buf[3] != CHECK_PATTERN) {
        res = -SD_ERR_CHECK_PATTERN;
        goto out;
    }

    self_p->type = TYPE_SD2;

    /* Tell the device that the host supports SDHC. */
    for (i = 0; i < 10; i++) {
        if (application_command_call(self_p,
                                     ACMD_SD_SEND_OP_COND,
                                     0x40000000UL,
                                     &response) != 0) {
            continue;
        }

        if (response == 0) {
            break;
        }
    }

    if (i == 10) {
        res = -SD_ERR_SD_SEND_OP_COND;
        goto out;
    }

    /* Read OCR. */
    if (command_check_call(self_p, CMD_READ_OCR, arg, 0) != 0) {
        res = -SD_ERR_READ_OCR;
        goto out;
    }

    /* Read the OCR value. */
    if (spi_read(self_p->spi_p, &buf[0], 4) != 4) {
        goto out;
    }

    /* Wait for card initialization complete. */
    if (buf[0] & 0xc0) {
        self_p->type = TYPE_SDHC;
    }

    res = 0;

 out:
    spi_deselect(self_p->spi_p);
    spi_give_bus(self_p->spi_p);

    return (res);
}

int sd_stop(struct sd_driver_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    return (0);
}

ssize_t sd_read_cid(struct sd_driver_t *self_p,
                    struct sd_cid_t *cid_p)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(cid_p != NULL, EINVAL);

    return (read(self_p, CMD_SEND_CID, 0, cid_p, sizeof(*cid_p)));
}

ssize_t sd_read_csd(struct sd_driver_t *self_p,
                    union sd_csd_t *csd_p)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(csd_p != NULL, EINVAL);

    return (read(self_p, CMD_SEND_CSD, 0, csd_p, sizeof(*csd_p)));
}

#if 0
int sd_erase_blocks(struct sd_driver_t *self_p,
                    uint32_t start,
                    uint32_t end)
{
    ASSERTN(self_p != NULL, EINVAL);

    union r1_t r1;

    /* Check if block address should be mapped to byte address */
    if (self_p->type != TYPE_SDHC) {
        start <<= 9;
        end <<= 9;
    }

    /* Send commands for block erase */
    if (command_check_call_idle(self_p, CMD_ERASE_WR_BLK_START, start)) {
        return (-1);
    }

    if (command_check_call_idle(self_p, CMD_ERASE_WR_BLK_END, end)) {
        return (-1);
    }

    if (command_call(self_p, CMD_ERASE, 0)) {
        return (-1);
    }

    wait_for_response(self_p, 0xff, 0x0, &r1.as_uint8);

    if (R1_IS_ERROR(r1)) {
        return (-1);
    }

    return (0);
}
#endif

ssize_t sd_read_block(struct sd_driver_t *self_p,
                      void *dst_p,
                      uint32_t src_block)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(dst_p != NULL, EINVAL);

    if (self_p->type != TYPE_SDHC) {
        src_block <<= 9;
    }

    return (read(self_p,
                 CMD_READ_SINGLE_BLOCK,
                 src_block,
                 dst_p,
                 SD_BLOCK_SIZE));
}

ssize_t sd_write_block(struct sd_driver_t *self_p,
                       uint32_t dst_block,
                       const void *src_p)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(src_p != NULL, EINVAL);

    ssize_t res;
    uint16_t crc;
    uint8_t response;

    res = -1;

    /* Check for byte address adjustment. */
    if (self_p->type != TYPE_SDHC) {
        dst_block <<= 9;
    }

    /* Calculate the checksum of the reseiced data. */
    crc = crc_xmodem(0, src_p, SD_BLOCK_SIZE);
    crc = htons(crc);

    spi_take_bus(self_p->spi_p);
    spi_select(self_p->spi_p);

    /* Issue write block command. */
    if (command_check_call(self_p, CMD_WRITE_BLOCK, dst_block, 0) != 0) {
        res = -SD_ERR_WRITE_BLOCK;
        goto out;
    }

    /* Write the start token. */
    spi_put(self_p->spi_p, TOKEN_DATA_START_BLOCK);

    /* Write the data and it's checksum. */
    spi_write(self_p->spi_p, src_p, SD_BLOCK_SIZE);
    spi_write(self_p->spi_p, &crc, sizeof(crc));

    /* Wait for the data-response token. */
    spi_get(self_p->spi_p, &response);

    if ((response & TOKEN_DATA_RES_MASK) != TOKEN_DATA_RES_ACCEPTED) {
        res = -SD_ERR_WRITE_BLOCK_TOKEN_DATA_RES_ACCEPTED;
        goto out;
    }

    /* Wait for the write operation to complete and check status */
    if (wait_not_busy(self_p, WRITE_TIMEOUT) != 0) {
        res = -SD_ERR_WRITE_BLOCK_WAIT_NOT_BUSY;
        goto out;
    }

    if (command_check_call(self_p, CMD_SEND_STATUS, 0, 0) != 0) {
        res = -SD_ERR_WRITE_BLOCK_SEND_STATUS;
        goto out;
    }

    spi_get(self_p->spi_p, &response);

    res = (response == 0 ? SD_BLOCK_SIZE : -1);

 out:
    spi_deselect(self_p->spi_p);
    spi_give_bus(self_p->spi_p);

    return (res);
}

#endif
