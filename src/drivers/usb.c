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

#if CONFIG_USB == 1

struct id_str_t {
    int id;
    char *str_p;
};

/**
 * USB class map.
 */
static const struct id_str_t class_map[] = {
    { .id = 0x00, .str_p = "USE_INTERFACE" },
    { .id = 0x01, .str_p = "AUDIO" },
    { .id = 0x02, .str_p = "CDC_CONTROL" },
    { .id = 0x03, .str_p = "HID" },
    { .id = 0x05, .str_p = "PHYSICAL" },
    { .id = 0x06, .str_p = "IMAGE" },
    { .id = 0x07, .str_p = "PRINTER" },
    { .id = 0x08, .str_p = "MASS_STORAGE" },
    { .id = 0x09, .str_p = "HUB" },
    { .id = 0x0a, .str_p = "CDC_DATA" },
    { .id = 0x0b, .str_p = "SMART_CARD" },
    { .id = 0x0d, .str_p = "CONTENT_SECURITY" },
    { .id = 0x0e, .str_p = "VIDEO" },
    { .id = 0x0f, .str_p = "PERSONAL_HEALTHCARE" },
    { .id = 0x10, .str_p = "AUDIO_VIDEO_DEVICES" },
    { .id = 0x11, .str_p = "BILLBOARD_DEVICE_CLASS" },
    { .id = 0xdc, .str_p = "DIAGNOSTIC_DEVICE" },
    { .id = 0xe0, .str_p = "WIRELESS_CONTROLLER" },
    { .id = 0xef, .str_p = "MISCELLANEOUS" },
    { .id = 0xfe, .str_p = "APPLICATION_SPECIFIC" },
    { .id = 0xff, .str_p = "VENDOR_SPECIFIC" },
    { .id = 0x00, .str_p = NULL }
};

/**
 * USB endpoint direction map.
 */
static const struct id_str_t endpoint_direction_map[] = {
    { .id = 0x00, .str_p = "OUT" },
    { .id = 0x01, .str_p = "IN" },
    { .id = 0x00, .str_p = NULL }
};

/**
 * USB endpoint transfer type map.
 */
static const struct id_str_t endpoint_transfer_type_map[] = {
    { .id = 0x00, .str_p = "CONTROL" },
    { .id = 0x01, .str_p = "ISOCHRONOUS" },
    { .id = 0x02, .str_p = "BULK" },
    { .id = 0x03, .str_p = "INTERRUPT" },
    { .id = 0x00, .str_p = NULL }
};

/**
 * USB endpoint synchronisation type map.
 */
static const struct id_str_t endpoint_synchronisation_type_map[] = {
    { .id = 0x00, .str_p = "NO_SYNCHRONISATION" },
    { .id = 0x01, .str_p = "ASYNCHRONOUS" },
    { .id = 0x02, .str_p = "ADAPTIVE" },
    { .id = 0x03, .str_p = "SYNCHRONOUS" },
    { .id = 0x00, .str_p = NULL }
};

/**
 * USB endpoint usage type map.
 */
static const struct id_str_t endpoint_usage_type_map[] = {
    { .id = 0x00, .str_p = "DATA" },
    { .id = 0x01, .str_p = "FEEDBACK" },
    { .id = 0x02, .str_p = "EXPLICIT_FEEDBACK" },
    { .id = 0x00, .str_p = NULL }
};

static const char *get_str_by_id(const struct id_str_t *id_str_map_p,
                                 int id)
{
    while (id_str_map_p->str_p != NULL) {
        if (id_str_map_p->id == id) {
            return (id_str_map_p->str_p);
        }

        id_str_map_p++;
    }

    return (NULL);
}

int usb_format_descriptors(void *out_p,
                           uint8_t *buf_p,
                           size_t size)
{
    ASSERTN(out_p != NULL, EINVAL);
    ASSERTN(buf_p != NULL, EINVAL);
    ASSERTN(size > 0, EINVAL);

    union usb_descriptor_t *desc_p;
    size_t pos = 0;

    while (pos < size) {
        desc_p = (union usb_descriptor_t *)buf_p;

        switch (desc_p->header.descriptor_type) {

        case DESCRIPTOR_TYPE_DEVICE:
            std_printf(FSTR("Device descriptor:\r\n"
                            "  length = %d\r\n"
                            "  descriptor_type = %d\r\n"
                            "  bcd_usb = %02x.%02x\r\n"
                            "  device_class = %d (%s)\r\n"
                            "  device_subclass = %d\r\n"
                            "  device_protocol = %d\r\n"
                            "  max_packet_size_0 = %d\r\n"
                            "  id_vendor = 0x%04x\r\n"
                            "  id_product = 0x%04x\r\n"
                            "  bcd_device = %02x.%02x\r\n"
                            "  manufacturer = %d\r\n"
                            "  product = %d\r\n"
                            "  serial_number = %d\r\n"
                            "  num_configurations = %d\r\n"),
                       desc_p->device.length,
                       desc_p->device.descriptor_type,
                       ((desc_p->device.bcd_usb >> 8) & 0xff),
                       (desc_p->device.bcd_usb & 0xff),
                       desc_p->device.device_class,
                       get_str_by_id(class_map,
                                     desc_p->device.device_class),
                       desc_p->device.device_subclass,
                       desc_p->device.device_protocol,
                       desc_p->device.max_packet_size_0,
                       desc_p->device.id_vendor,
                       desc_p->device.id_product,
                       ((desc_p->device.bcd_device >> 8) & 0xff),
                       (desc_p->device.bcd_device & 0xff),
                       desc_p->device.manufacturer,
                       desc_p->device.product,
                       desc_p->device.serial_number,
                       desc_p->device.num_configurations);
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
                            "    max_power = %d mA\r\n"),
                       desc_p->configuration.length,
                       desc_p->configuration.descriptor_type,
                       desc_p->configuration.total_length,
                       desc_p->configuration.num_interfaces,
                       desc_p->configuration.configuration_value,
                       desc_p->configuration.configuration,
                       desc_p->configuration.configuration_attributes,
                       2 * desc_p->configuration.max_power);
            break;

        case DESCRIPTOR_TYPE_INTERFACE:
            std_printf(FSTR("    Interface descriptor:\r\n"
                            "      length = %d\r\n"
                            "      descriptor_type = %d\r\n"
                            "      interface_number = %d\r\n"
                            "      alternate_setting = %d\r\n"
                            "      num_endpoints = %d\r\n"
                            "      interface_class = %d (%s)\r\n"
                            "      interface_subclass = %d\r\n"
                            "      interface_protocol = %d\r\n"
                            "      interface = %d\r\n"),
                       desc_p->interface.length,
                       desc_p->interface.descriptor_type,
                       desc_p->interface.interface_number,
                       desc_p->interface.alternate_setting,
                       desc_p->interface.num_endpoints,
                       desc_p->interface.interface_class,
                       get_str_by_id(class_map,
                                     desc_p->interface.interface_class),
                       desc_p->interface.interface_subclass,
                       desc_p->interface.interface_protocol,
                       desc_p->interface.interface);
            break;

        case DESCRIPTOR_TYPE_ENDPOINT:
        {
            std_printf(FSTR("      Endpoint descriptor:\r\n"
                            "        length = %d\r\n"
                            "        descriptor_type = %d\r\n"),
                       desc_p->endpoint.length,
                       desc_p->endpoint.descriptor_type);

            int type = ENDPOINT_ATTRIBUTES_TRANSFER_TYPE(desc_p->endpoint.attributes);

            if (type == ENDPOINT_ATTRIBUTES_TRANSFER_TYPE_CONTROL) {
                std_printf(FSTR("        endpoint_address = 0x%02x (EP %d)\r\n"),
                           desc_p->endpoint.endpoint_address,
                           ENDPOINT_ENDPOINT_ADDRESS_NUMBER(desc_p->endpoint.endpoint_address));
            } else {
                std_printf(FSTR("        endpoint_address = 0x%02x (%s, EP %d)\r\n"),
                           desc_p->endpoint.endpoint_address,
                           get_str_by_id(endpoint_direction_map,
                                         ENDPOINT_ENDPOINT_ADDRESS_DIRECTION(desc_p->endpoint.endpoint_address)),
                           ENDPOINT_ENDPOINT_ADDRESS_NUMBER(desc_p->endpoint.endpoint_address));
            }

            if (type == ENDPOINT_ATTRIBUTES_TRANSFER_TYPE_ISOCHRONOUS) {
                std_printf(FSTR("        attributes = 0x%02x (%s, %s, %s)\r\n"),
                           desc_p->endpoint.attributes,
                           get_str_by_id(endpoint_usage_type_map,
                                         ENDPOINT_ATTRIBUTES_USAGE_TYPE(desc_p->endpoint.attributes)),
                           get_str_by_id(endpoint_synchronisation_type_map,
                                         ENDPOINT_ATTRIBUTES_SYNCHRONISATION_TYPE(desc_p->endpoint.attributes)),
                           get_str_by_id(endpoint_transfer_type_map, type));
            } else {
                std_printf(FSTR("        attributes = 0x%02x (%s)\r\n"),
                           desc_p->endpoint.attributes,
                           get_str_by_id(endpoint_transfer_type_map, type));
            }

            std_printf(FSTR("        max_packet_size = %d\r\n"
                            "        interval = %d\r\n"),
                       desc_p->endpoint.max_packet_size,
                       desc_p->endpoint.interval);
            break;
        }

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
    ASSERTNRN(buf_p != NULL, EINVAL);
    ASSERTNRN(size > 0, EINVAL);

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
    ASSERTNRN(buf_p != NULL, EINVAL);
    ASSERTNRN(size > 0, EINVAL);

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
    ASSERTNRN(buf_p != NULL, EINVAL);
    ASSERTNRN(size > 0, EINVAL);

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
    ASSERTN(buf_p != NULL, EINVAL);
    ASSERTN(size > 0, EINVAL);

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

#endif
