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

/* Application valid flag. */
#define APPLICATION_VALID_FLAG                             0xbe

/* The maximum data transfer size. */
#define TRANSFER_DATA_SIZE_MAX                             4096

/* Data IDentifiers (DID). */
#define DID_VERSION                                      0xf000
#define DID_SYSTEM_TIME                                  0xf001

/* Routines. */
#define ROUTINE_ID_ERASE                                 0xff00

/* Diagnostic session types. */
#define DIAGNOSTIC_SESSION_CONTROL_DEFAULT                 0x01

/* UDS services. */
#define DIAGNOSTIC_SESSION_CONTROL                         0x10
#define READ_DATA_BY_IDENTIFIER                            0x22
#define READ_MEMORY_BY_ADDRESS                             0x23
#define ROUTINE_CONTROL                                    0x31
#define REQUEST_DOWNLOAD                                   0x34
#define TRANSFER_DATA                                      0x36
#define REQUEST_TRANSFER_EXIT                              0x37

/* Positive response bit. */
#define POSITIVE_RESPONSE                                  0x40

/* Negative response codes. */
#define GENERAL_REJECT                                     0x10
#define SERVICE_NOT_SUPPORTED                              0x11
#define SUBFUNCTION_NOT_SUPPORTED                          0x12
#define INCORRECT_MESSAGE_LENGTH_OR_INVALID_FORMAT         0x13
#define RESPONSE_TOO_LONG                                  0x14
#define BUSY_REPEAT_REQUEST                                0x21
#define CONDITIONS_NOT_CORRECT                             0x22
#define REQUEST_SEQUENCE_ERROR                             0x24
#define NO_RESPONSE_FROM_SUBNET_COMPONENT                  0x25
#define FAILURE_PREVENTS_EXECUTION_OF_REQUESTED_ACTION     0x26
#define REQUEST_OUT_OF_RANGE                               0x31
#define SECURITY_ACCESS_DENIED                             0x33
#define INVALID_KEY                                        0x35
#define EXCEED_NUMBER_OF_ATTEMPTS                          0x36
#define REQUIRED_TIME_DELAY_NOT_EXPIRED                    0x37
#define UPLOAD_DOWNLOAD_NOT_ACCEPTED                       0x70
#define TRANSFER_DATA_SUSPENDED                            0x71
#define GENERAL_PROGRAMMING_FAILURE                        0x72
#define WRONG_BLOCK_SEQUENCE_NUMBER                        0x73
#define REQUEST_CORRECTLY_RECEIVED_BUT_RESPONSE_IS_PENDING 0x78
#define SUBFUNCTION_NOT_SUPPORTED_INACTIVE_SESSION         0x7e
#define SERVICE_NOT_SUPPORTED_INACTIVE_SESSION             0x7f
#define RPM_TOO_HIGH                                       0x81
#define RPM_TO_LOW                                         0x82
#define ENGINE_IS_RUNNING                                  0x83
#define ENGINE_IS_NOT_RUNNING                              0x84
#define ENGINE_RUNTIME_TOO_LOW                             0x85
#define TEMPERATURE_TOO_HIGH                               0x86
#define TEMPERATURE_TOO_LOW                                0x87
#define VEHICLE_SPEED_TOO_HIGH                             0x88
#define VEHICLE_SPEED_TOO_LOW                              0x89
#define THROTTLE_PEDAL_TOO_HIGH                            0x8a
#define THROTTLE_PEDAL_TOO_LOW                             0x8b
#define TRANSMISSION_RANGE_NOT_IN_NEUTRAL                  0x8c
#define TRANSMISSION_RANGE_NOT_IN_GEAR                     0x8d
#define BRAKE_SWITCH_NOT_CLOSED                            0x8f
#define SHIFTER_LEVER_NOT_IN_PARK                          0x90
#define TORQUE_CONVERTER_CLUTCH_LOCKED                     0x91
#define VOLTAGE_TOO_HIGH                                   0x92
#define VOLTAGE_TOO_LOW                                    0x93

/**
 * Read and discard given number of bytes from the input channel.
 *
 * @param[in] self_p UDS object..
 * @param[in] size Number of byte to ignore.
 *
 * @returns zero(0) or negative error code.
 */
static int ignore(struct upgrade_uds_t *self_p,
                  int size)
{
    uint8_t dummy;

    while (size > 0) {
        chan_read(self_p->chin_p, &dummy, sizeof(dummy));
        size--;
    }

    return (0);
}

/**
 * Read given number of bytes from the input channel and write them to
 * the flash memory.
 *
 * @param[in] self_p UDS object..
 * @param[in] size Number of byte to write.
 *
 * @returns zero(0) or negative error code.
 */
static int write_application(struct upgrade_uds_t *self_p,
                             size_t size)
{
    size_t left;
    uint8_t buffer[TRANSFER_DATA_SIZE_MAX];
    size_t n;

    left = size;

    while (left > 0) {
        if (left > sizeof(buffer)) {
            n = sizeof(buffer);
        } else {
            n = left;
        }

        /* Read from the input channel and write to the memory. */
        if (chan_read(self_p->chin_p, buffer, n) != n) {
            return (-EIO);
        }

        if (upgrade_binary_upload(buffer, n) != 0) {
            return (-1);
        }

        left -= n;
    }

    return (0);
}

/**
 * Write an UDS response on the output channel.
 *
 * @param[in] self_p UDS object.
 * @param[in] length Number of bytes in the response after the code.
 * @param[in] code Response code.
 *
 * @returns zero(0) or negative error code.
 */
static int write_response(struct upgrade_uds_t *self_p,
                          int32_t length,
                          uint8_t code)
{
    length = htonl(length + 1);
    chan_write(self_p->chout_p, &length, sizeof(length));
    chan_write(self_p->chout_p, &code, sizeof(code));

    return (0);
}

/**
 * Ignore given number of bytes and then write an UDS response on the
 * output channel.
 *
 * @param[in] self_p UDS object..
 * @param[in] length Number of bytes to ignore on the input channel.
 * @param[in] code Response code.
 *
 * @returns zero(0) or negative error code.
 */
static int ignore_and_write_response_no_data(struct upgrade_uds_t *self_p,
                                             size_t length,
                                             uint8_t code)
{
    ignore(self_p, length);

    return (write_response(self_p, 0, code));
}

/**
 * Ignore given number of bytes and then write an negative UDS
 * response on the output channel.
 *
 * @param[in] self_p UDS object..
 * @param[in] length Number of bytes to ignore on the input channel.
 * @param[in] buf_p Two bytes or negative response data after 0x7f.
 *
 * @returns zero(0) or negative error code.
 */
static int ignore_and_write_negative_response(struct upgrade_uds_t *self_p,
                                              size_t length,
                                              uint8_t service_id,
                                              uint8_t code)
{
    ignore(self_p, length);
    write_response(self_p, 2, 0x7f);
    chan_write(self_p->chout_p, &service_id, 1);
    chan_write(self_p->chout_p, &code, 1);

    return (0);
}

/**
 * Write a DID response on the output channel.
 *
 * @param[in] self_p UDS object..
 * @param[in] response_code Response code.
 * @param[in] did DID.
 * @param[in] data_p Response data.
 * @param[in] size Response data size.
 *
 * @returns zero(0) or negative error code.
 */
static int write_did_response(void *chout_p,
                              uint8_t response_code,
                              uint16_t did,
                              const void *data_p,
                              size_t size)
{
    uint32_t length;

    length = (sizeof(response_code)
              + sizeof(did)
              + size);

    length = htonl(length);
    did = htons(did);

    chan_write(chout_p, &length, sizeof(length));
    chan_write(chout_p, &response_code, sizeof(response_code));
    chan_write(chout_p, &did, sizeof(did));
    chan_write(chout_p, data_p, size);

    return (0);
}

/**
 * Handle the read version DID request.
 *
 * @param[in] self_p UDS object..
 *
 * @returns zero(0) or negative error code.
 */
static int handle_read_data_by_identifier_version(struct upgrade_uds_t *self_p)
{
    const char version[] = STRINGIFY(VERSION);

    return (write_did_response(self_p->chout_p,
                               (READ_DATA_BY_IDENTIFIER | POSITIVE_RESPONSE),
                               DID_VERSION,
                               version,
                               sizeof(version)));
}

/**
 * Handle the read system time DID request.
 *
 * @param[in] self_p UDS object..
 *
 * @returns zero(0) or negative error code.
 */
static int handle_read_data_by_identifier_system_time(struct upgrade_uds_t *self_p)
{
    char buf[32];
    struct time_t now;

    time_get(&now);
    std_sprintf(buf, FSTR("%lu"), now.seconds);

    return (write_did_response(self_p->chout_p,
                               (READ_DATA_BY_IDENTIFIER | POSITIVE_RESPONSE),
                               DID_SYSTEM_TIME,
                               buf,
                               strlen(buf) + 1));
}

/**
 * Handle the diagnostic session control request to enter the default
 * session (application).
 *
 * @param[in] self_p UDS object..
 * @param[in] length Number of bytes left on the input channel.
 *
 * @returns zero(0) or negative error code.
 */
static int handle_diagnostic_session_control_default(struct upgrade_uds_t *self_p,
                                                     int length)
{
# if !defined(UPGRADE_TEST)
    if (upgrade_application_is_valid(0) == 0) {
        ignore_and_write_negative_response(self_p,
                                           0,
                                           DIAGNOSTIC_SESSION_CONTROL,
                                           CONDITIONS_NOT_CORRECT);

        return (-1);
    }
#endif

    ignore_and_write_response_no_data(self_p,
                                      0,
                                      (DIAGNOSTIC_SESSION_CONTROL | POSITIVE_RESPONSE));

    /* Call the application. */
# if !defined(UPGRADE_TEST)
    upgrade_application_enter();
#endif

    return (0);
}

/**
 * Handle the erase control routine.
 *
 * @param[in] self_p UDS object..
 * @param[in] sub_function Sub function.
 * @param[in] length Number of bytes left on the input channel.
 *
 * @returns zero(0) or negative error code.
 */
static int handle_routine_control_erase(struct upgrade_uds_t *self_p,
                                        int sub_function,
                                        int length)
{
    upgrade_application_erase();
    ignore_and_write_response_no_data(self_p,
                                      length,
                                      (ROUTINE_CONTROL | POSITIVE_RESPONSE));

    return (0);
}

/**
 * UDS uses different operating sessions, which can be changed using
 * the "Diagnostic Session Control". Depending on which session is
 * active, different services are available. On start, the control
 * unit is by default in the "Default Session". Other sessions are
 * defined, but are not required to be implemented depending on the
 * type of device:
 *
 * - "Programming Session" used to upload software.
 *
 * - "Extended Diagnostic Session" used to unlock additional
 *   diagnostic functions, such as the adjustment of sensors.
 *
 * - "Safety system diagnostic session" used to test all
 *   safety-critical diagnostic functions, such as airbag tests.
 *
 * In addition, there are reserved session identifiers that can be
 * defined for vehicle manufacturers and vehicle suppliers specific
 * use.
 *
 * @param[in] self_p UDS object..
 * @param[in] length Number of bytes left on the input channel.
 *
 * @returns zero(0) or negative error code.
 */
static int handle_diagnostic_session_control(struct upgrade_uds_t *self_p,
                                             int length)
{
    uint8_t session;
    int res;

    /* Length check. */
    if (length < 1) {
        ignore_and_write_negative_response(self_p,
                                           0,
                                           DIAGNOSTIC_SESSION_CONTROL,
                                           INCORRECT_MESSAGE_LENGTH_OR_INVALID_FORMAT);

        return (-1);
    }

    /* Read session type. */
    chan_read(self_p->chin_p, &session, sizeof(session));
    length -= 1;

    /* Handle the session. */
    switch (session) {

    case DIAGNOSTIC_SESSION_CONTROL_DEFAULT:
        res = handle_diagnostic_session_control_default(self_p, length);
        break;

    default:
        ignore_and_write_negative_response(self_p,
                                           0,
                                           DIAGNOSTIC_SESSION_CONTROL,
                                           REQUEST_OUT_OF_RANGE);
        res = -1;
        break;
    }

    return (res);
}

/**
 * With this service it is possible to retrieve one or more values of
 * a control unit. This can be information of all kinds and of
 * different lengths such as Partnumber or the software
 * version. Dynamic values such as the current state of the sensor
 * can be queried. Each value is associated to a Data Identifier
 * (DID) between 0 and 65535.
 *
 * @param[in] self_p UDS object..
 * @param[in] length Number of bytes left on the input channel.
 *
 * @returns zero(0) or negative error code.
 */
static int handle_read_data_by_identifier(struct upgrade_uds_t *self_p,
                                          int length)
{
    uint16_t did;
    int res;

    if (length < 2) {
        ignore_and_write_negative_response(self_p,
                                           length,
                                           READ_DATA_BY_IDENTIFIER,
                                           INCORRECT_MESSAGE_LENGTH_OR_INVALID_FORMAT);

        return (-1);
    }

    chan_read(self_p->chin_p, &did, sizeof(did));

    did = ntohs(did);
    length -= 2;

    switch (did) {

    case DID_VERSION:
        res = handle_read_data_by_identifier_version(self_p);
        break;

    case DID_SYSTEM_TIME:
        res = handle_read_data_by_identifier_system_time(self_p);
        break;

    default:
        ignore_and_write_negative_response(self_p,
                                           length,
                                           READ_DATA_BY_IDENTIFIER,
                                           SERVICE_NOT_SUPPORTED);
        res = -1;
        break;
    }

    return (res);
}

/**
 * Read data from the physical memory at the provided address.
 *
 * @param[in] self_p UDS object..
 * @param[in] length Number of bytes left on the input channel.
 *
 * @returns zero(0) or negative error code.
 */
static int handle_read_memory_by_address(struct upgrade_uds_t *self_p,
                                         int length)
{
    uint32_t address;
    uint8_t value;

    if (length < 4) {
        ignore_and_write_negative_response(self_p,
                                           length,
                                           READ_MEMORY_BY_ADDRESS,
                                           INCORRECT_MESSAGE_LENGTH_OR_INVALID_FORMAT);

        return (-1);
    }

    chan_read(self_p->chin_p, &address, sizeof(address));

#if defined(UPGRADE_TEST)
    value = 0x01;
#else
    value = *((uint8_t *)(uintptr_t)htonl(address));
#endif

    write_response(self_p,
                   sizeof(address) + sizeof(value),
                   (READ_MEMORY_BY_ADDRESS | POSITIVE_RESPONSE));
    chan_write(self_p->chout_p, &address, sizeof(address));
    chan_write(self_p->chout_p, &value, sizeof(value));

    return (0);
}

/**
 * The Control service routine services of all kinds can be
 * performed. There are three different message types:
 *
 * 1. With the start-message, a service can be initiated. It can be
 *    defined to confirm the beginning of the execution or to notify
 *    when the service is completed.
 *
 * 2. With the Stop message, a running service can be interrupted at
 *    any time.
 *
 * 3. The third option is a message to query the results of the
 *    service.
 *
 * The start and stop message parameters can be specified. This makes
 * it possible to implement every possible project-specific service.
 *
 * @param[in] self_p UDS object..
 * @param[in] length Number of bytes left on the input channel.
 *
 * @returns zero(0) or negative error code.
 */
static int handle_routine_control(struct upgrade_uds_t *self_p,
                                  int length)
{
    uint8_t sub_function;
    uint16_t routine_id;
    int res;

    /* Length check. */
    if (length < 3) {
        ignore_and_write_negative_response(self_p,
                                           length,
                                           ROUTINE_CONTROL,
                                           INCORRECT_MESSAGE_LENGTH_OR_INVALID_FORMAT);

        return (-1);
    }

    /* Read sub-function and routine id. */
    chan_read(self_p->chin_p, &sub_function, sizeof(sub_function));
    chan_read(self_p->chin_p, &routine_id, sizeof(routine_id));

    routine_id = ntohs(routine_id);
    length -= 3;

    /* Handle the routine. */
    switch (routine_id) {

    case ROUTINE_ID_ERASE:
        res = handle_routine_control_erase(self_p, sub_function, length);
        break;

    default:
        ignore_and_write_negative_response(self_p,
                                           length,
                                           ROUTINE_CONTROL,
                                           SERVICE_NOT_SUPPORTED);
        res = -1;
        break;
    }

    return (res);
}

/**
 * Downloading new software or other data into the control unit is
 * introduced using the "Request Download". Here, the location and
 * size of the data is specified. In turn, the controller specifies
 * how large the data packets can be.
 *
 * @param[in] self_p UDS object..
 * @param[in] length Number of bytes left on the input channel.
 *
 * @returns zero(0) or negative error code.
 */
static int handle_request_download(struct upgrade_uds_t *self_p,
                                   int length)
{
    uint8_t buf[5];
    uint32_t address;
    uint32_t size;

    /* Length check. */
    if (length < 3) {
        ignore_and_write_negative_response(self_p,
                                           length,
                                           REQUEST_DOWNLOAD,
                                           INCORRECT_MESSAGE_LENGTH_OR_INVALID_FORMAT);

        return (-1);
    }

    chan_read(self_p->chin_p, &buf, 3);
    length -= 3;

    /* The address and size should be 4 bytes each. */
    if ((buf[1] != sizeof(address)) || (buf[2] != sizeof(size))) {
        ignore_and_write_negative_response(self_p,
                                           length,
                                           REQUEST_DOWNLOAD,
                                           REQUEST_OUT_OF_RANGE);

        return (-1);
    }

    /* Length check. */
    if (length < 8) {
        ignore_and_write_negative_response(self_p,
                                           length,
                                           REQUEST_DOWNLOAD,
                                           INCORRECT_MESSAGE_LENGTH_OR_INVALID_FORMAT);

        return (-1);
    }

    chan_read(self_p->chin_p, &address, sizeof(address));
    chan_read(self_p->chin_p, &size, sizeof(size));
    length -= 8;

    /* Save the address and size. */
    self_p->swdl.next_block_sequence_counter = 1;

    if (upgrade_binary_upload_begin() != 0) {
        ignore_and_write_negative_response(self_p,
                                           length,
                                           REQUEST_DOWNLOAD,
                                           CONDITIONS_NOT_CORRECT);

        return (-1);
    }

    self_p->state = UDS_STATE_SWDL;

    /* Send the positive response. */
    write_response(self_p,
                   sizeof(buf),
                   (REQUEST_DOWNLOAD | POSITIVE_RESPONSE));

    buf[0] = 0x40;
    buf[1] = ((TRANSFER_DATA_SIZE_MAX >> 24) & 0xff);
    buf[2] = ((TRANSFER_DATA_SIZE_MAX >> 16) & 0xff);
    buf[3] = ((TRANSFER_DATA_SIZE_MAX >> 8) & 0xff);
    buf[4] = ((TRANSFER_DATA_SIZE_MAX >> 0) & 0xff);

    chan_write(self_p->chout_p, buf, sizeof(buf));

    return (0);
}

/**
 * For the actual transmission of data, the service "Transfer Data" is
 * used. This service is used for both uploading and downloading
 * data. The transfer direction is notified in advance by the service
 * "Request Download" or "Upload Request". This service should try to
 * send packets at maximum length, as specified in previous
 * services. If the data set is larger than the maximum, the "Transfer
 * Data" service must be used several times in succession until all
 * data has arrived.
 *
 * @param[in] self_p UDS object..
 * @param[in] length Number of bytes left on the input channel.
 *
 * @returns zero(0) or negative error code.
 */
static int handle_transfer_data(struct upgrade_uds_t *self_p,
                                int length)
{
    int res;
    uint8_t block_sequence_counter;

    /* A request download request must be sent before data can be
       transferred. */
    if (self_p->state != UDS_STATE_SWDL) {
        ignore_and_write_negative_response(self_p,
                                           length,
                                           TRANSFER_DATA,
                                           CONDITIONS_NOT_CORRECT);

        return (-1);
    }

    /* Length check. */
    if (length < 1) {
        ignore_and_write_negative_response(self_p,
                                           0,
                                           TRANSFER_DATA,
                                           INCORRECT_MESSAGE_LENGTH_OR_INVALID_FORMAT);

        return (-1);
    }

    /* Sequence counter. */
    chan_read(self_p->chin_p, &block_sequence_counter, sizeof(block_sequence_counter));
    length--;

    /* Only accept the expected sequence counter. */
    if (block_sequence_counter != self_p->swdl.next_block_sequence_counter) {
        ignore_and_write_negative_response(self_p,
                                           length,
                                           TRANSFER_DATA,
                                           WRONG_BLOCK_SEQUENCE_NUMBER);

        return (-1);
    }

    /* Write to the memory. */
    if (write_application(self_p, length) == 0) {
        self_p->swdl.next_block_sequence_counter++;
        write_response(self_p,
                       sizeof(block_sequence_counter),
                       (TRANSFER_DATA | POSITIVE_RESPONSE));
        chan_write(self_p->chout_p,
                   &block_sequence_counter,
                   sizeof(block_sequence_counter));
        res = 0;
    } else {
        ignore_and_write_negative_response(self_p,
                                           length,
                                           TRANSFER_DATA,
                                           GENERAL_PROGRAMMING_FAILURE);
        res = -1;
    }

    return (res);
}

/**
 * A data transmission can be 'completed' when using the "Transfer
 * Exit" service. This service is used for comparison between the
 * control unit and the tester. When it is running, a control unit
 * can answer negatively on this request to stop a data transfer
 * request. This will be used when the amount of data (set in
 * "Request Download" or "Upload Request") has not been transferred.
 *
 * @param[in] self_p UDS object..
 * @param[in] length Number of bytes left on the input channel.
 *
 * @returns zero(0) or negative error code.
 */
static int handle_request_transfer_exit(struct upgrade_uds_t *self_p,
                                        int length)
{
    /* Sanity check. */
    if (length != 0) {
        ignore_and_write_negative_response(self_p,
                                           length,
                                           REQUEST_TRANSFER_EXIT,
                                           INCORRECT_MESSAGE_LENGTH_OR_INVALID_FORMAT);

        return (-1);
    }

    /* Can only exit if swdl is active. */
    if (self_p->state != UDS_STATE_SWDL) {
        ignore_and_write_negative_response(self_p,
                                           0,
                                           REQUEST_TRANSFER_EXIT,
                                           CONDITIONS_NOT_CORRECT);

        return (-1);
    }

    upgrade_binary_upload_end();

    self_p->state = UDS_STATE_IDLE;

    ignore_and_write_response_no_data(self_p,
                                      0,
                                      (REQUEST_TRANSFER_EXIT | POSITIVE_RESPONSE));

    return (0);
}

/**
 * Handle an unknown service id by sending a negative response.
 *
 * @param[in] self_p UDS object..
 * @param[in] length Number of bytes left on the input channel.
 *
 * @returns zero(0) or negative error code.
 */
static int handle_unknown_service_id(struct upgrade_uds_t *self_p,
                                     int length,
                                     uint8_t service_id)
{
    ignore_and_write_negative_response(self_p,
                                       length,
                                       service_id,
                                       SERVICE_NOT_SUPPORTED);

    return (0);
}

int upgrade_uds_init(struct upgrade_uds_t *self_p,
                                void *chin_p,
                                void *chout_p)
{
    self_p->state = UDS_STATE_IDLE;
    self_p->chin_p = chin_p;
    self_p->chout_p = chout_p;

    return (0);
}

int upgrade_uds_handle_service(struct upgrade_uds_t *self_p)
{
    int32_t length;
    int8_t service_id;
    int res;

    /* Wait for a service header. */
    chan_read(self_p->chin_p, &length, sizeof(length));
    length = ntohl(length);

    /* The minimum packet size is one byte. */
    if (length < 1) {
        return (-1);
    }

    /* Read the service id. */
    chan_read(self_p->chin_p, &service_id, sizeof(service_id));
    length -= 1;

    /* Handle the service. */
    switch (service_id) {

    case DIAGNOSTIC_SESSION_CONTROL:
        res = handle_diagnostic_session_control(self_p, length);
        break;

    case READ_DATA_BY_IDENTIFIER:
        res = handle_read_data_by_identifier(self_p, length);
        break;

    case READ_MEMORY_BY_ADDRESS:
        res = handle_read_memory_by_address(self_p, length);
        break;

    case ROUTINE_CONTROL:
        res = handle_routine_control(self_p, length);
        break;

    case REQUEST_DOWNLOAD:
        res = handle_request_download(self_p, length);
        break;

    case TRANSFER_DATA:
        res = handle_transfer_data(self_p, length);
        break;

    case REQUEST_TRANSFER_EXIT:
        res = handle_request_transfer_exit(self_p, length);
        break;

    default:
        res = handle_unknown_service_id(self_p, length, service_id);
        break;
    }

    return (res);
}
