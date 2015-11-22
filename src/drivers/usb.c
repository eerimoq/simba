/**
 * @file usb.c
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

#include "usb_port.i"

int usb_format_descriptors(chan_t *out_p,
                           uint8_t *buf_p,
                           size_t size)
{
    union usb_descriptor_t *desc_p;
    size_t pos = 0;

    while (pos < size) {
        desc_p = (union usb_descriptor_t *)buf_p;

        switch (desc_p->header.descriptor_type) {

        case DESCRIPTOR_TYPE_DEVICE:
            std_printf(FSTR("Device descriptor:\r\n"
                            "  length = %d\r\n"
                            "  descriptor_type = %d\r\n"
                            "  bcd_usb = 0x%04x\r\n"
                            "  device_class = %d\r\n"
                            "  device_sub_class = %d\r\n"
                            "  device_protocol = %d\r\n"
                            "  max_packet_size_0 = %d\r\n"
                            "  id_vendor = 0x%04x\r\n"
                            "  id_product = 0x%04x\r\n"
                            "  bcd_device = 0x%04x\r\n"
                            "  manufacturer = %d\r\n"
                            "  product = %d\r\n"
                            "  serial_number = %d\r\n"
                            "  num_configurations = %d\r\n"),
                       (int)desc_p->device.length,
                       (int)desc_p->device.descriptor_type,
                       (int)desc_p->device.bcd_usb,
                       (int)desc_p->device.device_class,
                       (int)desc_p->device.device_sub_class,
                       (int)desc_p->device.device_protocol,
                       (int)desc_p->device.max_packet_size_0,
                       (int)desc_p->device.id_vendor,
                       (int)desc_p->device.id_product,
                       (int)desc_p->device.bcd_device,
                       (int)desc_p->device.manufacturer,
                       (int)desc_p->device.product,
                       (int)desc_p->device.serial_number,
                       (int)desc_p->device.num_configurations);
            break;

        case DESCRIPTOR_TYPE_CONFIGURATION:
            std_printf(FSTR("  Configuration descriptor:\r\n"
                            "    length = %d\r\n"
                            "    descriptor_type = %d\r\n"
                            "    total_length = %d\r\n"
                            "    num_interfaces = %d\r\n"
                            "    configuration_value = %d\r\n"
                            "    configuration = %d\r\n"
                            "    configuration_attributes = %d\r\n"
                            "    max_power = %d\r\n"),
                       (int)desc_p->configuration.length,
                       (int)desc_p->configuration.descriptor_type,
                       (int)desc_p->configuration.total_length,
                       (int)desc_p->configuration.num_interfaces,
                       (int)desc_p->configuration.configuration_value,
                       (int)desc_p->configuration.configuration,
                       (int)desc_p->configuration.configuration_attributes,
                       (int)desc_p->configuration.max_power);
            break;

        case DESCRIPTOR_TYPE_INTERFACE:
            std_printf(FSTR("    Interface descriptor:\r\n"
                            "      length = %d\r\n"
                            "      descriptor_type = %d\r\n"
                            "      interface_number = %d\r\n"
                            "      alternate_setting = %d\r\n"
                            "      num_endpoints = %d\r\n"
                            "      interface_class = %d\r\n"
                            "      interface_subclass = %d\r\n"
                            "      interface_protocol = %d\r\n"
                            "      interface = %d\r\n"),
                       (int)desc_p->interface.length,
                       (int)desc_p->interface.descriptor_type,
                       (int)desc_p->interface.interface_number,
                       (int)desc_p->interface.alternate_setting,
                       (int)desc_p->interface.num_endpoints,
                       (int)desc_p->interface.interface_class,
                       (int)desc_p->interface.interface_subclass,
                       (int)desc_p->interface.interface_protocol,
                       (int)desc_p->interface.interface);
            break;

        case DESCRIPTOR_TYPE_ENDPOINT:
            std_printf(FSTR("      Endpoint descriptor:\r\n"
                            "        length = %d\r\n"
                            "        descriptor_type = %d\r\n"
                            "        endpoint_address = 0x%02x\r\n"
                            "        attributes = %d\r\n"
                            "        max_packet_size = %d\r\n"
                            "        interval = %d\r\n"),
                       (int)desc_p->endpoint.length,
                       (int)desc_p->endpoint.descriptor_type,
                       (int)desc_p->endpoint.endpoint_address,
                       (int)desc_p->endpoint.attributes,
                       (int)desc_p->endpoint.max_packet_size,
                       (int)desc_p->endpoint.interval);
            break;

        default:
            break;
        }

        pos += desc_p->header.length;
        buf_p += desc_p->header.length;
    }

    return (0);
}

struct usb_descriptor_configuration_t *
usb_desc_get_configuration(uint8_t *buf_p,
                           size_t size,
                           int configuration)
{
    union usb_descriptor_t *desc_p;
    size_t pos = 0;

    while (pos < size) {
        desc_p = (union usb_descriptor_t *)buf_p;

        switch (desc_p->header.descriptor_type) {

        case DESCRIPTOR_TYPE_CONFIGURATION:
            if (desc_p->configuration.configuration_value == configuration) {
                return (&desc_p->configuration);
            }
            break;

        default:
            break;
        }

        pos += desc_p->header.length;
        buf_p += desc_p->header.length;
    }

    return (NULL);
}

struct usb_descriptor_interface_t *
usb_desc_get_interface(uint8_t *buf_p,
                       size_t size,
                       int configuration,
                       int interface)
{
    union usb_descriptor_t *desc_p;
    struct usb_descriptor_configuration_t *conf_desc_p = NULL;
    size_t pos = 0;

    while (pos < size) {
        desc_p = (union usb_descriptor_t *)buf_p;

        switch (desc_p->header.descriptor_type) {

        case DESCRIPTOR_TYPE_CONFIGURATION:
            if (desc_p->configuration.configuration_value == configuration) {
                conf_desc_p = &desc_p->configuration;
            }
            break;

        case DESCRIPTOR_TYPE_INTERFACE:
            if (conf_desc_p != NULL) {
                if (desc_p->interface.interface_number == interface) {
                    return (&desc_p->interface);
                }
            }
            break;

        default:
            break;
        }

        pos += desc_p->header.length;
        buf_p += desc_p->header.length;
    }

    return (NULL);
}

struct usb_descriptor_endpoint_t *
usb_desc_get_endpoint(uint8_t *buf_p,
                      size_t size,
                      int configuration,
                      int interface,
                      int endpoint)
{
    union usb_descriptor_t *desc_p;
    struct usb_descriptor_configuration_t *conf_desc_p = NULL;
    struct usb_descriptor_interface_t *int_desc_p = NULL;
    size_t pos = 0;

    while (pos < size) {
        desc_p = (union usb_descriptor_t *)buf_p;

        switch (desc_p->header.descriptor_type) {

        case DESCRIPTOR_TYPE_CONFIGURATION:
            if (desc_p->configuration.configuration_value == configuration) {
                conf_desc_p = &desc_p->configuration;
            }
            break;

        case DESCRIPTOR_TYPE_INTERFACE:
            if (conf_desc_p != NULL) {
                if (desc_p->interface.interface_number == interface) {
                    int_desc_p = &desc_p->interface;
                }
            }
            break;

        case DESCRIPTOR_TYPE_ENDPOINT:
            if (int_desc_p != NULL) {
                if ((desc_p->endpoint.endpoint_address & 0x7f) == endpoint) {
                    return (&desc_p->endpoint);
                }
            }
            break;

        default:
            break;
        }

        pos += desc_p->header.length;
        buf_p += desc_p->header.length;
    }

    return (NULL);
}

int usb_desc_get_class(uint8_t *buf_p,
                       size_t size,
                       int configuration,
                       int interface)
{
    struct usb_descriptor_interface_t *int_desc_p;

    int_desc_p = usb_desc_get_interface(buf_p,
                                        size,
                                        configuration,
                                        interface);

    if (int_desc_p == NULL) {
        return (-1);
    }

    return (int_desc_p->interface_class);
}
