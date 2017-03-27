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

#ifndef __DRIVERS_USB_H__
#define __DRIVERS_USB_H__

#include "simba.h"

#include "usb_port.h"

/* Request types. */
#define REQUEST_TYPE_DATA_MASK                     (0x80)
#define REQUEST_TYPE_DATA_DIRECTION_HOST_TO_DEVICE (0x00)
#define REQUEST_TYPE_DATA_DIRECTION_DEVICE_TO_HOST (0x80)

#define REQUEST_TYPE_TYPE_MASK                     (0x60)
#define REQUEST_TYPE_TYPE_STANDARD                 (0x00)
#define REQUEST_TYPE_TYPE_CLASS                    (0x20)
#define REQUEST_TYPE_TYPE_VENDOR                   (0x40)

#define REQUEST_TYPE_RECIPIENT_MASK                (0x0f)
#define REQUEST_TYPE_RECIPIENT_DEVICE              (0x00)
#define REQUEST_TYPE_RECIPIENT_INTERFACE           (0x01)
#define REQUEST_TYPE_RECIPIENT_ENDPOINT            (0x02)
#define REQUEST_TYPE_RECIPIENT_OTHER               (0x03)

/* Setup requests. */
#define REQUEST_GET_STATUS         0
#define REQUEST_SET_ADDRESS        5
#define REQUEST_GET_DESCRIPTOR     6
#define REQUEST_SET_CONFIGURATION  9

/* USB descriptor types. */
#define DESCRIPTOR_TYPE_DEVICE                   1
#define DESCRIPTOR_TYPE_CONFIGURATION            2
#define DESCRIPTOR_TYPE_STRING                   3
#define DESCRIPTOR_TYPE_INTERFACE                4
#define DESCRIPTOR_TYPE_ENDPOINT                 5
#define DESCRIPTOR_TYPE_INTERFACE_ASSOCIATION   11
#define DESCRIPTOR_TYPE_RPIPE                   34
#define DESCRIPTOR_TYPE_CDC                     36

/* USB classes. */
#define USB_CLASS_USE_INTERFACE          0x00 /* Device. */
#define USB_CLASS_AUDIO                  0x01 /* Interface. */
#define USB_CLASS_CDC_CONTROL            0x02 /* Both. */
#define USB_CLASS_HID                    0x03 /* Interface. */
#define USB_CLASS_PHYSICAL               0x05 /* Interface. */
#define USB_CLASS_IMAGE                  0x06 /* Interface. */
#define USB_CLASS_PRINTER                0x07 /* Interface. */
#define USB_CLASS_MASS_STORAGE           0x08 /* Interface. */
#define USB_CLASS_HUB                    0x09 /* Device. */
#define USB_CLASS_CDC_DATA               0x0a /* Interface. */
#define USB_CLASS_SMART_CARD             0x0b /* Interface. */
#define USB_CLASS_CONTENT_SECURITY       0x0d /* Interface. */
#define USB_CLASS_VIDEO                  0x0e /* Interface. */
#define USB_CLASS_PERSONAL_HEALTHCARE    0x0f /* Interface. */
#define USB_CLASS_AUDIO_VIDEO_DEVICES    0x10 /* Interface. */
#define USB_CLASS_BILLBOARD_DEVICE_CLASS 0x11 /* Device. */
#define USB_CLASS_DIAGNOSTIC_DEVICE      0xdc /* Both. */
#define USB_CLASS_WIRELESS_CONTROLLER    0xe0 /* Interface. */
#define USB_CLASS_MISCELLANEOUS          0xef /* Both. */
#define USB_CLASS_APPLICATION_SPECIFIC   0xfe /* Interface. */
#define USB_CLASS_VENDOR_SPECIFIC        0xff /* Both. */

/* Endpoint descriptor helper macros. */
#define ENDPOINT_ENDPOINT_ADDRESS_DIRECTION(address) (((address) >> 7) & 0x1)
#define ENDPOINT_ENDPOINT_ADDRESS_NUMBER(address) (((address) >> 0) & 0xf)

#define ENDPOINT_ATTRIBUTES_USAGE_TYPE(attributes) (((attributes) >> 4) & 0x3)
#define ENDPOINT_ATTRIBUTES_SYNCHRONISATION_TYPE(attributes) (((attributes) >> 2) & 0x3)
#define ENDPOINT_ATTRIBUTES_TRANSFER_TYPE(attributes) (((attributes) >> 0) & 0x3)
#define ENDPOINT_ATTRIBUTES_TRANSFER_TYPE_CONTROL     0
#define ENDPOINT_ATTRIBUTES_TRANSFER_TYPE_ISOCHRONOUS 1
#define ENDPOINT_ATTRIBUTES_TRANSFER_TYPE_BULK        2
#define ENDPOINT_ATTRIBUTES_TRANSFER_TYPE_INTERRUPT   3

/* Setups. */
struct usb_setup_t {
    uint8_t request_type;
    uint8_t request;
    union {
        struct {
            uint16_t feature_selector;
            uint16_t zero_interface_endpoint;
        } clear_feature;
        struct {
            uint16_t zero0;
            uint16_t zero1;
        } get_configuration;
        struct {
            uint8_t descriptor_index;
            uint8_t descriptor_type;
            uint16_t language_id;
        } get_descriptor;
        struct {
            uint16_t device_address;
            uint16_t zero;
        } set_address;
        struct {
            uint16_t configuration_value;
            uint16_t zero;
        } set_configuration;
        struct {
            uint16_t value;
            uint16_t index;                
        } base;
    } u;
    uint16_t length;
};

/* USB descriptors. */

/* All descriptors starts with a length field and the descriptor
 * type. */
struct usb_descriptor_header_t {
    uint8_t length;
    uint8_t descriptor_type;
};

/* Device descriptor. */
struct usb_descriptor_device_t {
    uint8_t length;
    uint8_t descriptor_type;
    uint16_t bcd_usb;
    uint8_t device_class;
    uint8_t device_subclass;
    uint8_t device_protocol;
    uint8_t max_packet_size_0;
    uint16_t id_vendor;
    uint16_t id_product;
    uint16_t bcd_device;
    uint8_t manufacturer;
    uint8_t product;
    uint8_t serial_number;
    uint8_t num_configurations;
};

#define CONFIGURATION_ATTRIBUTES_BUS_POWERED          0x80

/* Configuration descriptor. */
struct usb_descriptor_configuration_t {
    uint8_t length;
    uint8_t descriptor_type;
    uint16_t total_length;
    uint8_t num_interfaces;
    uint8_t configuration_value;
    uint8_t configuration;
    uint8_t configuration_attributes;
    uint8_t max_power;
};

/* Interface descriptor. */
struct usb_descriptor_interface_t {
    uint8_t length;
    uint8_t descriptor_type;
    uint8_t interface_number;
    uint8_t alternate_setting;
    uint8_t num_endpoints;
    uint8_t interface_class;
    uint8_t interface_subclass;
    uint8_t interface_protocol;
    uint8_t interface;
};

/* Endpoint descriptor. */
struct usb_descriptor_endpoint_t {
    uint8_t length;
    uint8_t descriptor_type;
    uint8_t endpoint_address;
    uint8_t attributes;
    uint16_t max_packet_size;
    uint8_t interval;
};

/* String descriptor. */
struct usb_descriptor_string_t {
    uint8_t length;
    uint8_t descriptor_type;
    uint8_t string[256];
};

struct usb_descriptor_interface_association_t {
    uint8_t length;
    uint8_t descriptor_type;
    uint8_t first_interface;
    uint8_t interface_count;
    uint8_t function_class;
    uint8_t function_subclass;
    uint8_t function_protocol;
    uint8_t function;
};

struct usb_descriptor_cdc_header_t {
    uint8_t length;
    uint8_t descriptor_type;
    uint8_t sub_type;
    uint16_t bcd;
};

struct usb_descriptor_cdc_acm_t {
    uint8_t length;
    uint8_t descriptor_type;
    uint8_t sub_type;
    uint8_t capabilities;
};

struct usb_descriptor_cdc_union_t {
    uint8_t length;
    uint8_t descriptor_type;
    uint8_t sub_type;
    uint8_t master_interface;
    uint8_t slave_interface;
};

struct usb_descriptor_cdc_call_management_t {
    uint8_t length;
    uint8_t descriptor_type;
    uint8_t sub_type;
    uint8_t capabilities;
    uint8_t data_interface;
};

union usb_descriptor_t {
    struct usb_descriptor_header_t header;
    struct usb_descriptor_device_t device;
    struct usb_descriptor_configuration_t configuration;
    struct usb_descriptor_interface_t interface;
    struct usb_descriptor_endpoint_t endpoint;
    struct usb_descriptor_string_t string;
};

#define USB_CDC_LINE_CODING             0x20
#define USB_CDC_CONTROL_LINE_STATE      0x22
#define USB_CDC_SEND_BREAK              0x23

struct usb_cdc_line_info_t {
  uint32_t dte_rate;
  uint8_t char_format;
  uint8_t parity_type;
  uint8_t data_bits;
};

/* Message types. */
#define USB_MESSAGE_TYPE_ADD    0
#define USB_MESSAGE_TYPE_REMOVE 1

struct usb_message_header_t {
    int type;
};

struct usb_message_add_t {
    struct usb_message_header_t header;
    int device;
};

union usb_message_t {
    struct usb_message_header_t header;
    struct usb_message_add_t add;
};

extern struct usb_device_t usb_device[USB_DEVICE_MAX];

/**
 * Format the descriptors and write them to given channel.
 *
 * @param[in] out_p Output channel.
 * @param[in] buf_p Pointer to the descriptors to format.
 * @param[in] size Number of bytes in the descriptors buffer.
 *
 * @return zero(0) or negative error code.
 */
int usb_format_descriptors(void *out_p,
                           uint8_t *buf_p,
                           size_t size);

/**
 * Get the configuration descriptor for given configuration index.
 *
 * @param[in] buf_p Pointer to the descriptors.
 * @param[in] size Number of bytes in the descriptors buffer.
 * @param[in] configuration Configuration to find.
 *
 * @return Configuration or NULL on failure.
 */
struct usb_descriptor_configuration_t *
usb_desc_get_configuration(uint8_t *desc_p,
                           size_t size,
                           int configuration);

/**
 * Get the interface descriptor for given configuration and interface
 * index.
 *
 * @param[in] buf_p Pointer to the descriptors.
 * @param[in] size Number of bytes in the descriptors buffer.
 * @param[in] configuration Configuration to find.
 * @param[in] interface Interface to find.
 *
 * @return Interface or NULL on failure.
 */
struct usb_descriptor_interface_t *
usb_desc_get_interface(uint8_t *desc_p,
                       size_t size,
                       int configuration,
                       int interface);

/**
 * Get the endpoint descriptor for given configuration, interface and
 * endpoint index.
 *
 * @param[in] buf_p Pointer to the descriptors.
 * @param[in] size Number of bytes in the descriptors buffer.
 * @param[in] configuration Configuration to find.
 * @param[in] interface Interface to find.
 * @param[in] endpoint Endpoint to find.
 *
 * @return Endpoint or NULL on failure.
 */
struct usb_descriptor_endpoint_t *
usb_desc_get_endpoint(uint8_t *desc_p,
                      size_t size,
                      int configuration,
                      int interface,
                      int endpoint);

/**
 * Get the interface class.
 *
 * @param[in] buf_p Pointer to the descriptors.
 * @param[in] size Number of bytes in the descriptors buffer.
 * @param[in] configuration Configuration to find.
 * @param[in] interface Interface to find.
 *
 * @return
 */
int usb_desc_get_class(uint8_t *buf_p,
                       size_t size,
                       int configuration,
                       int interface);

#endif
