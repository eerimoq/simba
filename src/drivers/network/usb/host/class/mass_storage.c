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

#if CONFIG_USB_HOST == 1

#define REQUEST_GET_MAX_LUN 0xfe
#define REQUEST_RESET       0xff

#define CBW_SIGNATURE 0x43425355

/* Command Block Wrapper. */
struct cbw_t {
    uint32_t signature; /* Signature that helps identify this data
                         * packet as a CBW.  The signature field shall
                         * contain the value 43425355h (little
                         * endian), indicating a CBW. */

    uint32_t tag; /* A Command Block Tag sent by the host. The device
                   * shall echo the contents of this field back to the
                   * host in the dCSWTag field of the associated
                   * CSW. The dCSWTag positively associates a CSW with
                   * the corresponding CBW. */

    uint32_t data_transfer_length;  /* The number of bytes of data
                                     * that the host expects to
                                     * transfer on the Bulk-In or
                                     * Bulk-Out endpoint (as indicated
                                     * by the Direction bit) during
                                     * the execution of this
                                     * command. If this field is zero,
                                     * the device and the host shall
                                     * transfer no data between the
                                     * CBW and the associated CSW, and
                                     * the device shall ignore the
                                     * value of the Direction bit in
                                     * bmCBWFlags . */

    uint8_t flags; /* The bits of this field are defined as follows:
                    * Bit 7 - Direction - The device shall ignore this
                    * bit if the dCBWDataTransferLength field is zero,
                    * otherwise:
                    *   0 = Data-Out from host to the device,
                    *   1 = Data-In from the device to the host.
                    *
                    * Bit 6: Obsolete - The host shall set this bit to
                    * zero.
                    *
                    * Bits 5..0: Reserved - The host shall set these
                    * bits to zero. */

    uint8_t lun; /* The device Logical Unit Number (LUN) to which the
                  * command block is being sent. For devices that
                  * support multiple LUNs, the host shall place into
                  * this field the LUN to which this command block is
                  * addressed. Otherwise, the host shall set this
                  * field to zero. */

    uint8_t command_block_length; /* The valid length of the CBWCB in
                                   * bytes.  This defines the valid
                                   * length of the command block.  The
                                   * only legal values are 1 through
                                   * 16 (01h through 10h).  All other
                                   * values are reserved. */

    uint8_t command_block[16]; /* The command block to be executed by
                                * the device.  The device shall
                                * interpret the first bCBWCBLength
                                * bytes in this field as a command
                                * block as defined by the command set
                                * identified by bInterfaceSubClass .
                                * If the command set supported by the
                                * device uses command blocks of fewer
                                * than 16 (10h) bytes in length, the
                                * significant bytes shall be
                                * transferred first, beginning with
                                * the byte at offset 15 (Fh).  The
                                * device shall ignore the content of
                                * the CBWCB field past the byte at
                                * offset (15 + bCBWCBLength - 1). */
};

#define CSW_SIGNATURE 0x53425355

#define CSW_STATUS_PASSED      0
#define CSW_STATUS_FAILED      1
#define CSW_STATUS_PHASE_ERROR 2

/* Only the SCSI subclass is supported. */
#define MASS_STORAGE_SUBCLASS_SCSI 0x06

/* Only the bulk-only protocol is supported. */
#define MASS_STORAGE_PROTOCOL_BBB  0x50

/* Command Status Wrapper. */
struct csw_t {
    uint32_t signature; /* Signature that helps identify this data
                         * packet as a CSW.  The signature field shall
                         * contain the value 53425355h (little
                         * endian), indicating CSW. */

    uint32_t tag; /* The device shall set this field to the value
                   * received in the dCBWTag of the associated CBW. */

    uint32_t data_residue; /* For Data-Out the device shall report in
                            * the dCSWDataResidue the difference
                            * between the amount of data expected as
                            * stated in the dCBWDataTransferLength ,
                            * and the actual amount of data processed
                            * by the device. For Data-In the device
                            * shall report in the dCSWDataResidue the
                            * difference between the amount of data
                            * expected as stated in the
                            * dCBWDataTransferLength and the actual
                            * amount of relevant data sent by the
                            * device.  The dCSWDataResidue shall not
                            * exceed the value sent in the
                            * dCBWDataTransferLength . */

    uint8_t status; /* bCSWStatus indicates the success or failure of
                     * the command.  The device shall set this byte to
                     * zero if the command completed successfully.  A
                     * non-zero value shall indicate a failure during
                     * command execution according to the following
                     * table:
                     *
                     * Value           Description
                     * 00h             Command Passed ("good status")
                     * 01h             Command Failed
                     * 02h             Phase Error
                     * 03h and 04h     Reserved (Obsolete)
                     * 05h to FFh      Reserved */
};

#define SCSI_CDB_OPERATION_CODE_TEST_UNIT_READY 0x00
#define SCSI_CDB_OPERATION_CODE_INQUIRY         0x12
#define SCSI_CDB_OPERATION_CODE_READ_10         0x28
#define SCSI_CDB_OPERATION_CODE_WRITE_10        0x2a

struct scsi_cdb_test_unit_ready_t {
    uint8_t operation_code;
    uint8_t reserved0[4];
    uint8_t control;
};

struct scsi_cdb_inquiry_t {
    uint8_t operation_code;
    uint8_t reserved0 : 6;
    uint8_t cmdt : 1;
    uint8_t evpd : 1;
    uint8_t page_or_operation_code;
    uint8_t reserved1;
    uint8_t allocation_length;
    uint8_t control;
};

struct scsi_cdb_read_t {
    uint8_t operation_code;
    uint8_t rdpotect : 3;
    uint8_t dpo : 1;
    uint8_t fua : 1;
    uint8_t reserved0 : 1;
    uint8_t fua_nv : 1;
    uint8_t reserved1 : 1;
    uint32_t logical_block_address;
    uint8_t reserved2 : 3;
    uint8_t group_number : 5;
    uint16_t transfer_length;
    uint8_t control;    
};

struct scsi_cdb_inquiry_data_t {
    uint8_t peripheral_qualifier : 3;
    uint8_t peripheral_device_type : 5;
    uint8_t rmb : 1;
    uint8_t reserved0 : 7;
    uint8_t version;
    uint8_t flags;
    uint8_t additional_length;
    uint8_t tmp[3];
    uint8_t vendor_id[8];
    uint8_t product_id[16];
};

static int get_max_lun(struct usb_host_device_t *device_p)
{
    struct usb_setup_t setup;
    uint8_t buf[1];

    std_printf(FSTR("get number of LUN:s (logical units) from mass storage device\r\n"));

    /* Initiate the setup datastructure. */
    setup.request_type = (REQUEST_TYPE_DATA_DIRECTION_DEVICE_TO_HOST
                          | REQUEST_TYPE_TYPE_CLASS
                          | REQUEST_TYPE_RECIPIENT_INTERFACE);
    setup.request = REQUEST_GET_MAX_LUN;
    setup.u.base.value = 0;
    setup.u.base.index = 0;
    setup.length = 1;

    if (usb_host_device_control_transfer(device_p,
                                         &setup,
                                         buf,
                                         sizeof(buf)) != sizeof(buf))
    {
        return (-1);
    }

    std_printf(FSTR("LUN max = %d\r\n"), buf[0]);

    /* Save the max LUN number. */
    //device_p->max_lun = 0;

    return (0);
}

static int scsi_get_inquiry(struct usb_host_device_t *device_p)
{
    struct cbw_t cbw;
    struct csw_t csw;
    struct scsi_cdb_inquiry_t *cdb_p;
    struct scsi_cdb_inquiry_data_t inquiry_data;

    std_printf(FSTR("get inquiry\r\n"));

    device_p->max_packet_size = 512;

    /* Initiate the Command Block Wrapper. */
    memset(&cbw, 0, sizeof(cbw));
    cbw.signature = CBW_SIGNATURE;
    cbw.tag = (uint32_t)device_p;
    cbw.data_transfer_length = sizeof(inquiry_data);
    cbw.flags = 0x80;
    cbw.lun = 0;
    cbw.command_block_length = sizeof(*cdb_p);

    cdb_p = (struct scsi_cdb_inquiry_t *)cbw.command_block;
    cdb_p->operation_code = SCSI_CDB_OPERATION_CODE_INQUIRY;
    cdb_p->allocation_length = sizeof(inquiry_data);

    /* Write the Command Block Wrapper to the device. */
    if (usb_host_device_write(device_p, 2, &cbw, sizeof(cbw))
        != sizeof(cbw)) {
        std_printf(FSTR("failed to write the command block wrapper.\r\n"));

        return (-1);
    }

    /* Read the data from the device. */
    if (usb_host_device_read(device_p, 1, &inquiry_data, sizeof(inquiry_data))
        != sizeof(inquiry_data)) {
        std_printf(FSTR("Failed to read data.\r\n"));

        return (-1);
    }

    /* Read the Comand Status Wrapper from the device. */
    if (usb_host_device_read(device_p, 1, &csw, sizeof(csw))
        != sizeof(csw)) {
        return (-1);
    }

    if (csw.status != CSW_STATUS_PASSED) {
        std_printf(FSTR("status = %d\r\n"), csw.status);

        return (-1);
    }

    std_printf(FSTR("version = %d\r\n"), inquiry_data.version);

    return (csw.data_residue);
}

static int scsi_test_unit_ready(struct usb_host_device_t *device_p)
{
    struct cbw_t cbw;
    struct csw_t csw;
    struct scsi_cdb_test_unit_ready_t *cdb_p;

    std_printf(FSTR("test unit ready\r\n"));

    device_p->max_packet_size = 512;

    /* Initiate the Command Block Wrapper. */
    memset(&cbw, 0, sizeof(cbw));
    cbw.signature = CBW_SIGNATURE;
    cbw.tag = (uint32_t)device_p;
    cbw.data_transfer_length = 0;
    cbw.flags = 0x80;
    cbw.lun = 0;
    cbw.command_block_length = sizeof(*cdb_p);

    cdb_p = (struct scsi_cdb_test_unit_ready_t *)cbw.command_block;
    cdb_p->operation_code = SCSI_CDB_OPERATION_CODE_TEST_UNIT_READY;

    /* Write the Command Block Wrapper to the device. */
    if (usb_host_device_write(device_p, 2, &cbw, sizeof(cbw))
        != sizeof(cbw)) {
        std_printf(FSTR("failed to write the command block wrapper.\r\n"));

        return (-1);
    }

    /* Read the Comand Status Wrapper from the device. */
    if (usb_host_device_read(device_p, 1, &csw, sizeof(csw))
        != sizeof(csw)) {
        return (-1);
    }

    if (csw.status != CSW_STATUS_PASSED) {
        std_printf(FSTR("status = %d\r\n"), csw.status);

        return (-1);
    }

    return (csw.data_residue);
}

static int supports(struct usb_host_device_t *device_p)
{
    struct usb_descriptor_interface_t *int_p;

    int_p = usb_desc_get_interface(device_p->descriptors.buf,
                                   device_p->descriptors.size,
                                   1,
                                   0);

    if (int_p == NULL) {
        return (0);
    }

    return ((int_p->interface_class == USB_CLASS_MASS_STORAGE)
            && (int_p->interface_subclass == MASS_STORAGE_SUBCLASS_SCSI)
            && (int_p->interface_protocol == MASS_STORAGE_PROTOCOL_BBB));
}

static int enumerate(struct usb_host_device_t *device_p)
{
    struct usb_descriptor_interface_t *int_desc_p;

    std_printf(FSTR("Enumerating a MASS_STORAGE device.\r\n"));

    int_desc_p = usb_desc_get_interface(device_p->descriptors.buf,
                                        device_p->descriptors.size,
                                        1,
                                        0);

    if (int_desc_p == NULL) {
        return (-1);
    }

    if (usb_host_device_set_configuration(device_p, 1) != 0) {
        return (-1);
    }

    get_max_lun(device_p);

    scsi_get_inquiry(device_p);

    if (scsi_test_unit_ready(device_p) != 0) {
        std_printf(FSTR("device is not ready\r\n"));
    }

    return (0);
}

int usb_host_class_mass_storage_init(struct usb_host_class_mass_storage_driver_t *self_p,
                                     struct usb_host_driver_t *usb_p,
                                     struct usb_host_class_mass_storage_device_t *devices_p,
                                     size_t length)
{
    self_p->usb_p = usb_p;
    self_p->devices_p = devices_p;
    self_p->length = length;
    self_p->device_driver.supports = supports;
    self_p->device_driver.enumerate = enumerate;

    return (0);
}

int usb_host_class_mass_storage_start(struct usb_host_class_mass_storage_driver_t *self_p)
{
    return (usb_host_driver_add(self_p->usb_p,
                                &self_p->device_driver,
                                self_p));
}

int usb_host_class_mass_storage_stop(struct usb_host_class_mass_storage_driver_t *self_p)
{
    return (usb_host_driver_remove(self_p->usb_p,
                                   &self_p->device_driver));
}

ssize_t usb_host_class_mass_storage_device_read(
    struct usb_host_device_t *device_p,
    void *buf_p,
    size_t address,
    size_t size)
{
    struct cbw_t cbw;
    struct csw_t csw;
    struct scsi_cdb_read_t *cdb_p;

    device_p->max_packet_size = 512;

    /* Initiate the Command Block Wrapper. */
    memset(&cbw, 0, sizeof(cbw));
    cbw.signature = CBW_SIGNATURE;
    cbw.tag = (uint32_t)device_p;
    cbw.data_transfer_length = size;
    cbw.flags = 0x80;
    cbw.lun = 0;
    cbw.command_block_length = sizeof(*cdb_p);

    cdb_p = (struct scsi_cdb_read_t *)cbw.command_block;
    cdb_p->operation_code = SCSI_CDB_OPERATION_CODE_READ_10;
    cdb_p->logical_block_address = htonl(address / 512);
    cdb_p->transfer_length = htons(size / 512);

    /* Write the Command Block Wrapper to the device. */
    if (usb_host_device_write(device_p, 2, &cbw, sizeof(cbw))
        != sizeof(cbw)) {
        std_printf(FSTR("failed to write the command block wrapper.\r\n"));

        return (-1);
    }

    /* Read the dat from the device. */
    if (usb_host_device_read(device_p, 1, buf_p, size) != size) {
        std_printf(FSTR("Failed to read data.\r\n"));

        return (-1);
    }

    /* Read the Comand Status Wrapper from the device. */
    if (usb_host_device_read(device_p, 1, &csw, sizeof(csw))
        != sizeof(csw)) {
        return (-1);
    }

    if (csw.status != CSW_STATUS_PASSED) {
        std_printf(FSTR("status = %d\r\n"), csw.status);

        return (-1);
    }

    return (size - csw.data_residue);
}

#endif
