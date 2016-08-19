/**
 * @file descriptors.c
 *
 * @section License
 * Copyright (C) 2016, Erik Moqvist
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

static FAR const struct usb_descriptor_device_t
device_descriptor = {
    .length = sizeof(device_descriptor),
    .descriptor_type = DESCRIPTOR_TYPE_DEVICE,
    .bcd_usb = 0x0200,
    .device_class = USB_CLASS_USE_INTERFACE,
    .device_subclass = 0,
    .device_protocol = 0,
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
    .total_length = 75,
    .num_interfaces = 2,
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
cdc_union_descriptor = {
    .length = sizeof(cdc_union_descriptor),
    .descriptor_type = DESCRIPTOR_TYPE_CDC,
    .sub_type = 6,
    .master_interface = 0,
    .slave_interface = 1
};

static FAR const struct usb_descriptor_cdc_call_management_t
cdc_call_management_descriptor = {
    .length = sizeof(cdc_call_management_descriptor),
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

/**
 * An array of all USB device descriptors.
 */
__attribute__((weak)) FAR const union usb_descriptor_t *
usb_device_descriptors[] = {
    (FAR const union usb_descriptor_t *)&device_descriptor,
    (FAR const union usb_descriptor_t *)&configuration_descriptor,
    (FAR const union usb_descriptor_t *)&inferface_association_0_descriptor,
    (FAR const union usb_descriptor_t *)&inferface_0_descriptor,
    (FAR const union usb_descriptor_t *)&cdc_header_descriptor,
    (FAR const union usb_descriptor_t *)&cdc_acm_descriptor,
    (FAR const union usb_descriptor_t *)&cdc_union_descriptor,
    (FAR const union usb_descriptor_t *)&cdc_call_management_descriptor,
    (FAR const union usb_descriptor_t *)&endpoint_1_descriptor,
    (FAR const union usb_descriptor_t *)&inferface_1_descriptor,
    (FAR const union usb_descriptor_t *)&endpoint_2_descriptor,
    (FAR const union usb_descriptor_t *)&endpoint_3_descriptor,
    NULL
};

__attribute__((weak)) FAR const uint16_t usb_device_descriptor_string_language[2] = {
    (DESCRIPTOR_TYPE_STRING << 8) | (2 + 2),
    0x0409
};

__attribute__((weak)) FAR const uint16_t usb_device_descriptor_string_iproduct[17] = {
    (DESCRIPTOR_TYPE_STRING << 8) | (2 + 2 * 16),
    'A','r','d','u','i','n','o',' ','M','i','c','r','o',' ',' ',' '
};

__attribute__((weak)) FAR const uint16_t usb_device_descriptor_string_imanufacturer[12] = {
    (DESCRIPTOR_TYPE_STRING << 8) | (2 + 2 * 11),
    'U','n','k','n','o','w','n',' ',' ',' ',' '
};
