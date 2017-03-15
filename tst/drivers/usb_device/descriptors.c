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

static FAR const struct usb_descriptor_device_t
device_descriptor = {
    .length = sizeof(device_descriptor),
    .descriptor_type = DESCRIPTOR_TYPE_DEVICE,
    .bcd_usb = 0x0200,
    .device_class = USB_CLASS_MISCELLANEOUS,
    .device_subclass = 2,
    .device_protocol = 1,
    .max_packet_size_0 = 64,
    .id_vendor = CONFIG_USB_DEVICE_VID,
    .id_product = CONFIG_USB_DEVICE_PID,
    .bcd_device = 0x0100,
    .manufacturer = 0,
    .product = 0,
    .serial_number = 0,
    .num_configurations = 1
};

static FAR const struct usb_descriptor_configuration_t
configuration_descriptor = {
    .length = sizeof(configuration_descriptor),
    .descriptor_type = DESCRIPTOR_TYPE_CONFIGURATION,
    .total_length = 141,
    .num_interfaces = 4,
    .configuration_value = 1,
    .configuration = 0,
    .configuration_attributes = CONFIGURATION_ATTRIBUTES_BUS_POWERED,
    .max_power = 250
};

static FAR const struct usb_descriptor_interface_association_t
inferface_association_0_descriptor = {
    .length = sizeof(inferface_association_0_descriptor),
    .descriptor_type = DESCRIPTOR_TYPE_INTERFACE_ASSOCIATION,
    .first_interface = 0,
    .interface_count = 2,
    .function_class = 2,
    .function_subclass = 2,
    .function_protocol = 1,
    .function = 0
};

static FAR const struct usb_descriptor_interface_t
inferface_0_descriptor = {
    .length = sizeof(inferface_0_descriptor),
    .descriptor_type = DESCRIPTOR_TYPE_INTERFACE,
    .interface_number = 0,
    .alternate_setting = 0,
    .num_endpoints = 1,
    .interface_class = USB_CLASS_CDC_CONTROL,
    .interface_subclass = 2,
    .interface_protocol = 0,
    .interface = 0
};

static FAR const struct usb_descriptor_cdc_header_t
cdc_header_descriptor = {
    .length = sizeof(cdc_header_descriptor),
    .descriptor_type = DESCRIPTOR_TYPE_CDC,
    .sub_type = 0,
    .bcd = 0x1001
};

static FAR const struct usb_descriptor_cdc_acm_t
cdc_acm_descriptor = {
    .length = sizeof(cdc_acm_descriptor),
    .descriptor_type = DESCRIPTOR_TYPE_CDC,
    .sub_type = 2,
    .capabilities = 0x06
};

static FAR const struct usb_descriptor_cdc_union_t
cdc_union_0_descriptor = {
    .length = sizeof(cdc_union_0_descriptor),
    .descriptor_type = DESCRIPTOR_TYPE_CDC,
    .sub_type = 6,
    .master_interface = 0,
    .slave_interface = 1
};

static FAR const struct usb_descriptor_cdc_call_management_t
cdc_call_management_0_descriptor = {
    .length = sizeof(cdc_call_management_0_descriptor),
    .descriptor_type = DESCRIPTOR_TYPE_CDC,
    .sub_type = 1,
    .capabilities = 0x00,
    .data_interface = 1
};

static FAR const struct usb_descriptor_endpoint_t
endpoint_1_descriptor = {
    .length = sizeof(endpoint_1_descriptor),
    .descriptor_type = DESCRIPTOR_TYPE_ENDPOINT,
    .endpoint_address = 0x81, /* EP 1 IN. */
    .attributes = ENDPOINT_ATTRIBUTES_TRANSFER_TYPE_INTERRUPT,
    .max_packet_size = 16,
    .interval = 64
};

static FAR const struct usb_descriptor_interface_t
inferface_1_descriptor = {
    .length = sizeof(inferface_1_descriptor),
    .descriptor_type = DESCRIPTOR_TYPE_INTERFACE,
    .interface_number = 1,
    .alternate_setting = 0,
    .num_endpoints = 2,
    .interface_class = USB_CLASS_CDC_DATA,
    .interface_subclass = 0,
    .interface_protocol = 0,
    .interface = 0
};

static FAR const struct usb_descriptor_endpoint_t
endpoint_2_descriptor = {
    .length = sizeof(endpoint_2_descriptor),
    .descriptor_type = DESCRIPTOR_TYPE_ENDPOINT,
    .endpoint_address = 0x02, /* EP 2 OUT. */
    .attributes = ENDPOINT_ATTRIBUTES_TRANSFER_TYPE_BULK,
    .max_packet_size = 64,
    .interval = 0
};

static FAR const struct usb_descriptor_endpoint_t
endpoint_3_descriptor = {
    .length = sizeof(endpoint_3_descriptor),
    .descriptor_type = DESCRIPTOR_TYPE_ENDPOINT,
    .endpoint_address = 0x83, /* EP 3 IN. */
    .attributes = ENDPOINT_ATTRIBUTES_TRANSFER_TYPE_BULK,
    .max_packet_size = 64,
    .interval = 0
};

static FAR const struct usb_descriptor_interface_association_t
inferface_association_1_descriptor = {
    .length = sizeof(inferface_association_1_descriptor),
    .descriptor_type = DESCRIPTOR_TYPE_INTERFACE_ASSOCIATION,
    .first_interface = 2,
    .interface_count = 2,
    .function_class = 2,
    .function_subclass = 2,
    .function_protocol = 1,
    .function = 0
};

static FAR const struct usb_descriptor_interface_t
inferface_2_descriptor = {
    .length = sizeof(inferface_2_descriptor),
    .descriptor_type = DESCRIPTOR_TYPE_INTERFACE,
    .interface_number = 2,
    .alternate_setting = 0,
    .num_endpoints = 1,
    .interface_class = USB_CLASS_CDC_CONTROL,
    .interface_subclass = 2,
    .interface_protocol = 0,
    .interface = 0
};

static FAR const struct usb_descriptor_cdc_union_t
cdc_union_2_descriptor = {
    .length = sizeof(cdc_union_2_descriptor),
    .descriptor_type = DESCRIPTOR_TYPE_CDC,
    .sub_type = 6,
    .master_interface = 2,
    .slave_interface = 3
};

static FAR const struct usb_descriptor_cdc_call_management_t
cdc_call_management_2_descriptor = {
    .length = sizeof(cdc_call_management_2_descriptor),
    .descriptor_type = DESCRIPTOR_TYPE_CDC,
    .sub_type = 1,
    .capabilities = 0x00,
    .data_interface = 3
};

static FAR const struct usb_descriptor_endpoint_t
endpoint_4_descriptor = {
    .length = sizeof(endpoint_4_descriptor),
    .descriptor_type = DESCRIPTOR_TYPE_ENDPOINT,
    .endpoint_address = 0x84, /* EP 4 IN. */
    .attributes = ENDPOINT_ATTRIBUTES_TRANSFER_TYPE_INTERRUPT,
    .max_packet_size = 16,
    .interval = 64
};

static FAR const struct usb_descriptor_interface_t
inferface_3_descriptor = {
    .length = sizeof(inferface_3_descriptor),
    .descriptor_type = DESCRIPTOR_TYPE_INTERFACE,
    .interface_number = 3,
    .alternate_setting = 0,
    .num_endpoints = 2,
    .interface_class = USB_CLASS_CDC_DATA,
    .interface_subclass = 0,
    .interface_protocol = 0,
    .interface = 0
};

static FAR const struct usb_descriptor_endpoint_t
endpoint_5_descriptor = {
    .length = sizeof(endpoint_5_descriptor),
    .descriptor_type = DESCRIPTOR_TYPE_ENDPOINT,
    .endpoint_address = 0x05, /* EP 5 OUT. */
    .attributes = ENDPOINT_ATTRIBUTES_TRANSFER_TYPE_BULK,
    .max_packet_size = 64,
    .interval = 0
};

static FAR const struct usb_descriptor_endpoint_t
endpoint_6_descriptor = {
    .length = sizeof(endpoint_6_descriptor),
    .descriptor_type = DESCRIPTOR_TYPE_ENDPOINT,
    .endpoint_address = 0x86, /* EP 6 IN. */
    .attributes = ENDPOINT_ATTRIBUTES_TRANSFER_TYPE_BULK,
    .max_packet_size = 64,
    .interval = 0
};

/**
 * An array of all USB device descriptors.
 */
FAR const union usb_descriptor_t *
usb_device_descriptors[] = {
    (FAR const union usb_descriptor_t *)&device_descriptor,
    (FAR const union usb_descriptor_t *)&configuration_descriptor,
    /* Interface association 0 (/dev/ttyACM0). */
    (FAR const union usb_descriptor_t *)&inferface_association_0_descriptor,
    (FAR const union usb_descriptor_t *)&inferface_0_descriptor,
    (FAR const union usb_descriptor_t *)&cdc_header_descriptor,
    (FAR const union usb_descriptor_t *)&cdc_acm_descriptor,
    (FAR const union usb_descriptor_t *)&cdc_union_0_descriptor,
    (FAR const union usb_descriptor_t *)&cdc_call_management_0_descriptor,
    (FAR const union usb_descriptor_t *)&endpoint_1_descriptor,
    (FAR const union usb_descriptor_t *)&inferface_1_descriptor,
    (FAR const union usb_descriptor_t *)&endpoint_2_descriptor,
    (FAR const union usb_descriptor_t *)&endpoint_3_descriptor,
    /* Interface association 1 (/dev/ttyACM1). */
    (FAR const union usb_descriptor_t *)&inferface_association_1_descriptor,
    (FAR const union usb_descriptor_t *)&inferface_2_descriptor,
    (FAR const union usb_descriptor_t *)&cdc_header_descriptor,
    (FAR const union usb_descriptor_t *)&cdc_acm_descriptor,
    (FAR const union usb_descriptor_t *)&cdc_union_2_descriptor,
    (FAR const union usb_descriptor_t *)&cdc_call_management_2_descriptor,
    (FAR const union usb_descriptor_t *)&endpoint_4_descriptor,
    (FAR const union usb_descriptor_t *)&inferface_3_descriptor,
    (FAR const union usb_descriptor_t *)&endpoint_5_descriptor,
    (FAR const union usb_descriptor_t *)&endpoint_6_descriptor,
    NULL
};
