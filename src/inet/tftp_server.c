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

/* Operation codes. */
#define OPCODE_READ_REQUEST                                1
#define OPCODE_WRITE_REQUEST                               2
#define OPCODE_DATA                                        3
#define OPCODE_ACKNOWLEDGMENT                              4
#define OPCODE_ERROR                                       5

/* Error codes. */
#define ERROR_NOT_DEFINED                                  0
#define ERROR_FILE_NOT_FOUND                               1
#define ERROR_ACCESS_VIOLATION                             2
#define ERROR_DISK_FULL_OR_ALLOCATION_EXCEEDED             3
#define ERROR_ILLEGAL_TFTP_OPERATION                       4
#define ERROR_UNKNOWN_TRANSFER_ID                          5
#define ERROR_FILE_ALREADY_EXISTS                          6
#define ERROR_NO_SUCH_USER                                 7
#define ERROR_CODE_MAX                                     8

/* Protocol acces macros. */
#define OPCODE(buf_p)           ((buf_p[0] << 8) | buf_p[1])
#define BLOCK_NUMBER(buf_p)     ((buf_p[2] << 8) | buf_p[3])
#define ERROR_CODE(buf_p)       ((buf_p[2] << 8) | buf_p[3])

/* Sizes. */
#define DATA_SIZE                                        512
#define BUFFER_SIZE                          (DATA_SIZE + 4)

struct client_t {
    struct tftp_server_t *server_p;
    struct socket_t socket;
    uint8_t *buf_p;
    struct fs_file_t file;
    const char *filename_p;
    uint32_t number_of_bytes_transferred;
    struct {
        uint16_t block_number;
        ssize_t size;
        int retransmit_counter;
    } data;
};

static const char *error_code_str[ERROR_CODE_MAX + 1] = {
    "not defined",
    "file not found",
    "access violation",
    "disk full or allocation exceeded",
    "illegal tftp operation",
    "unknown transfer id",
    "file already exists",
    "no such user",
    "invalid error code"
};

static int find_string(const char **buf_pp,
                       size_t *size_p,
                       const char **string_pp)
{
    char *end_p;
    size_t length;

    if (*size_p == 0) {
        return (-1);
    }

    end_p = strchr(*buf_pp, '\0');

    if (end_p == *buf_pp) {
        return (-1);
    }

    length = (strlen(*buf_pp) + 1);
    *string_pp = *buf_pp;
    *buf_pp += length;
    *size_p -= length;

    return (0);
}

static int parse_request(const char *buf_p,
                         size_t size,
                         const char **filename_pp,
                         const char **mode_pp)
{
    if (find_string(&buf_p, &size, filename_pp) != 0) {
        return (-1);
    }

    if (find_string(&buf_p, &size, mode_pp) != 0) {
        return (-1);
    }

    return (0);
}

static int error_transmit(struct tftp_server_t *server_p,
                          struct inet_addr_t *remote_addr_p,
                          uint8_t *buf_p,
                          uint16_t error,
                          const char *error_message_p)
{
    size_t size;

    buf_p[0] = 0;
    buf_p[1] = OPCODE_ERROR;
    buf_p[2] = (error >> 8);
    buf_p[3] = error;
    size = 4;

    if (error_message_p == NULL) {
        error_message_p = error_code_str[error];
    }

    strcpy((char *)&buf_p[4], error_message_p);
    size += (strlen(error_message_p) + 1);

    if (socket_sendto(&server_p->listener,
                      &buf_p[0],
                      size,
                      0,
                      remote_addr_p) != size) {
        return (-1);
    }

    return (0);
}

static int client_data_write(struct client_t *self_p)
{
    size_t size;

    self_p->buf_p[0] = 0;
    self_p->buf_p[1] = OPCODE_DATA;
    self_p->buf_p[2] = (self_p->data.block_number >> 8);
    self_p->buf_p[3] = self_p->data.block_number;
    size = 4;

    self_p->data.size = fs_read(&self_p->file,
                                &self_p->buf_p[4],
                                DATA_SIZE);

    if (self_p->data.size < 0) {
        self_p->data.size = 0;
    }

    size += self_p->data.size;

    if (socket_write(&self_p->socket, self_p->buf_p, size) != size) {
        return (-1);
    }

    return (0);
}

static int client_data_transmit(struct client_t *self_p)
{
    self_p->data.retransmit_counter = 0;

    return (client_data_write(self_p));
}

static int client_data_retransmit(struct client_t *self_p)
{
    if (fs_seek(&self_p->file, -self_p->data.size, FS_SEEK_CUR) != 0) {
        return (-1);
    }

    self_p->data.retransmit_counter++;

    return (client_data_write(self_p));
}

static int client_ack_write(struct client_t *self_p)
{
    uint16_t block_number;

    /* Block number holds the value of the next expected block to
       receive. */
    block_number = (self_p->data.block_number - 1);

    self_p->buf_p[0] = 0;
    self_p->buf_p[1] = OPCODE_ACKNOWLEDGMENT;
    self_p->buf_p[2] = (block_number >> 8);
    self_p->buf_p[3] = block_number;

    if (socket_write(&self_p->socket, self_p->buf_p, 4) != 4) {
        return (-1);
    }

    return (0);
}

static int client_ack_transmit(struct client_t *self_p)
{
    self_p->data.retransmit_counter = 0;

    return (client_ack_write(self_p));
}

static int client_ack_retransmit(struct client_t *self_p)
{
    self_p->data.retransmit_counter++;

    return (client_ack_write(self_p));
}

static int client_error_transmit(struct client_t *self_p,
                                 uint16_t error)
{
    size_t size;

    self_p->buf_p[0] = 0;
    self_p->buf_p[1] = OPCODE_ERROR;
    self_p->buf_p[2] = (error >> 8);
    self_p->buf_p[3] = error;
    size = 4;

    strcpy((char *)&self_p->buf_p[4], error_code_str[error]);
    size += (strlen(error_code_str[error]) + 1);

    if (socket_write(&self_p->socket, &self_p->buf_p[0], size) != size) {
        return (-1);
    }

    return (0);
}

static int client_read_request_transfer_data(struct client_t *self_p)
{
    int opcode;
    uint16_t block_number;
    uint16_t error_code;
    struct time_t timeout;
    ssize_t size;

    timeout.seconds = (self_p->server_p->timeout_ms / 1000);
    timeout.nanoseconds = 1000000L * (self_p->server_p->timeout_ms % 1000);

    if (client_data_transmit(self_p) != 0) {
        return (-1);
    }

    while (1) {
        /* Waiting for acknowlegement or error. Retransmit outstanding
           data packet on timeout, or bail. */
        if (chan_poll(&self_p->socket, &timeout) == NULL) {
            if (self_p->data.retransmit_counter == 2) {
                return (-1);
            }

            if (client_data_retransmit(self_p) != 0) {
                return (-1);
            }

            continue;
        }

        /* Read the incoming packet. */
        size = socket_read(&self_p->socket, self_p->buf_p, BUFFER_SIZE);

        /* Acknowlegement and error packets are at least 4 bytes. */
        if (size < 4) {
            return (-1);
        }

        opcode = OPCODE(self_p->buf_p);

        switch (opcode) {

        case OPCODE_ACKNOWLEDGMENT:
            block_number = BLOCK_NUMBER(self_p->buf_p);

            /* Ignore bad acknowlegement packets. */
            if (block_number != self_p->data.block_number) {
                log_object_print(NULL,
                                 LOG_DEBUG,
                                 OSTR("ignoring block number %u when"
                                      " expecting %u\r\n"),
                                 block_number,
                                 self_p->data.block_number);
                continue;
            }

            self_p->number_of_bytes_transferred += self_p->data.size;

            /* The last packet is not full. */
            if (self_p->data.size < DATA_SIZE) {
                log_object_print(NULL,
                                 LOG_INFO,
                                 OSTR("sent %u bytes\r\n"),
                                 self_p->number_of_bytes_transferred);
                return (0);
            }

            /* The block has been acknowleged. Increment the block
               number for the next block. */
            self_p->data.block_number++;

            /* Transmit next data packet. */
            if (client_data_transmit(self_p) != 0) {
                return (-1);
            }
            break;

        case OPCODE_ERROR:
            error_code = ERROR_CODE(self_p->buf_p);

            if (error_code > ERROR_CODE_MAX) {
                error_code = ERROR_CODE_MAX;
            }

            log_object_print(NULL,
                             LOG_ERROR,
                             OSTR("error code %u: %s\r\n"),
                             error_code,
                             error_code_str[error_code]);
            return (-1);

        default:
            log_object_print(NULL,
                             LOG_ERROR,
                             OSTR("bad opcode %u\r\n"),
                             opcode);
            return (-1);
        }
    }

    return (0);
}

static int client_write_request_transfer_data(struct client_t *self_p)
{
    int opcode;
    uint16_t block_number;
    int error_code;
    struct time_t timeout;
    ssize_t size;

    timeout.seconds = (self_p->server_p->timeout_ms / 1000);
    timeout.nanoseconds = 1000000L * (self_p->server_p->timeout_ms % 1000);

    if (client_ack_transmit(self_p) != 0) {
        return (-1);
    }

    while (1) {
        /* Waiting for data or error. Retransmit the ack packet on
           timeout, or bail. */
        if (chan_poll(&self_p->socket, &timeout) == NULL) {
            if (self_p->data.retransmit_counter == 2) {
                return (-1);
            }

            if (client_ack_retransmit(self_p) != 0) {
                return (-1);
            }

            continue;
        }

        /* Read the incoming packet. */
        size = socket_read(&self_p->socket, self_p->buf_p, BUFFER_SIZE);

        /* Data and error packets are at least 4 bytes. */
        if (size < 4) {
            return (-1);
        }

        size -= 4;

        opcode = OPCODE(self_p->buf_p);

        switch (opcode) {

        case OPCODE_DATA:
            block_number = BLOCK_NUMBER(self_p->buf_p);

            /* Ignore bad data packets. */
            if (block_number != self_p->data.block_number) {
                log_object_print(NULL,
                                 LOG_INFO,
                                 OSTR("ignoring block number %u when"
                                      " expecting %u\r\n"),
                                 block_number,
                                 self_p->data.block_number);
                continue;
            }

            if (fs_write(&self_p->file, &self_p->buf_p[4], size) != size) {
                return (-1);
            }

            self_p->data.block_number++;

            /* Transmit ack packet. */
            if (client_ack_transmit(self_p) != 0) {
                return (-1);
            }

            self_p->number_of_bytes_transferred += size;

            /* The last packet is not full. */
            if (size < DATA_SIZE) {
                log_object_print(NULL,
                                 LOG_INFO,
                                 OSTR("received %u bytes\r\n"),
                                 self_p->number_of_bytes_transferred);
                return (0);
            }
            break;

        case OPCODE_ERROR:
            error_code = ERROR_CODE(self_p->buf_p);

            if (error_code > ERROR_CODE_MAX) {
                error_code = ERROR_CODE_MAX;
            }

            log_object_print(NULL,
                             LOG_ERROR,
                             OSTR("error code %u: %s\r\n"),
                             error_code,
                             error_code_str[error_code]);
            return (-1);

        default:
            log_object_print(NULL,
                             LOG_ERROR,
                             OSTR("bad opcode %u\r\n"),
                             opcode);
            return (-1);
        }
    }

    return (0);
}

static int client_open(struct client_t *self_p,
                       struct tftp_server_t *server_p,
                       uint8_t *buf_p,
                       size_t size,
                       struct inet_addr_t *remote_addr_p)
{
    const char *mode_p;
    const char *error_message_p;

    error_message_p = NULL;

    if (parse_request((const char *)&buf_p[2],
                      size - 2,
                      &self_p->filename_p,
                      &mode_p) != 0) {
        error_message_p = "malformed request";
        goto err;
    }

    if (strcmp(mode_p, "octet") != 0) {
        error_message_p = "only binary mode supported";
        goto err;
    }

    if (socket_open_udp(&self_p->socket) != 0) {
        goto err;
    }

    if (socket_connect(&self_p->socket, remote_addr_p) != 0) {
        goto err;
    }

    self_p->buf_p = buf_p;
    self_p->number_of_bytes_transferred = 0;
    self_p->data.block_number = 1;
    self_p->server_p = server_p;

    return (0);

 err:
    error_transmit(server_p,
                   remote_addr_p,
                   buf_p,
                   ERROR_NOT_DEFINED,
                   error_message_p);

    return (-1);
}

static int client_close(struct client_t *self_p)
{
    socket_close(&self_p->socket);

    return (0);
}

static int handle_read_request(struct tftp_server_t *self_p,
                               uint8_t *buf_p,
                               size_t size,
                               struct inet_addr_t *remote_addr_p)
{
    int res;
    struct client_t client;

    res = -1;

    if (client_open(&client, self_p, buf_p, size, remote_addr_p) == 0) {
        if (fs_open(&client.file, client.filename_p, FS_READ) == 0) {
            log_object_print(NULL,
                             LOG_INFO,
                             OSTR("reading from '%s'\r\n"),
                             client.filename_p);
            res = client_read_request_transfer_data(&client);
            (void)fs_close(&client.file);
        } else {
            res = client_error_transmit(&client, ERROR_FILE_NOT_FOUND);
        }

        client_close(&client);
    }

    return (res);
}

static int handle_write_request(struct tftp_server_t *self_p,
                                uint8_t *buf_p,
                                size_t size,
                                struct inet_addr_t *remote_addr_p)
{
    int res;
    struct client_t client;

    res = -1;

    if (client_open(&client, self_p, buf_p, size, remote_addr_p) == 0) {
        if (fs_open(&client.file,
                    client.filename_p,
                    FS_WRITE | FS_CREAT | FS_TRUNC) == 0) {
            log_object_print(NULL,
                             LOG_INFO,
                             OSTR("writing to '%s'\r\n"),
                             client.filename_p);
            res = client_write_request_transfer_data(&client);
            (void)fs_close(&client.file);
        }

        client_close(&client);
    }

    return (res);
}

static int handle_request(struct tftp_server_t *self_p,
                          uint8_t *buf_p,
                          ssize_t size,
                          struct inet_addr_t *remote_addr_p)
{
    int opcode;
    int res;

    if (size < 2) {
        return (-1);
    }

    opcode = OPCODE(buf_p);

    switch (opcode) {

    case OPCODE_READ_REQUEST:
        res = handle_read_request(self_p, buf_p, size, remote_addr_p);
        break;

    case OPCODE_WRITE_REQUEST:
        res = handle_write_request(self_p, buf_p, size, remote_addr_p);
        break;

    default:
        log_object_print(NULL,
                         LOG_INFO,
                         OSTR("bad opcode %u\r\n"),
                         opcode);
        res = -1;
        break;
    }

    return (res);
}

static void *tftp_server_main(void *arg_p)
{
    struct tftp_server_t *self_p;
    uint8_t buf[BUFFER_SIZE];
    struct inet_addr_t addr;
    ssize_t size;
    char addrbuf[16];
    struct thrd_environment_variable_t env[1];

    self_p = arg_p;

    thrd_set_name(self_p->name_p);

    /* Set current working directory if given. */
    if (self_p->root_p != NULL) {
        thrd_init_env(&env[0], membersof(env));
        (void)thrd_set_env("CWD", self_p->root_p);
    }

    if (socket_open_udp(&self_p->listener) != 0) {
        return (NULL);
    }

    if (socket_bind(&self_p->listener, &self_p->addr) != 0) {
        return (NULL);
    }

    log_object_print(NULL,
                     LOG_INFO,
                     OSTR("serving TFTP on %s:%u\r\n"),
                     inet_ntoa(&self_p->addr.ip, &addrbuf[0]),
                     self_p->addr.port);

    /* Wait for a client. */
    while (1) {
        size = socket_recvfrom(&self_p->listener,
                               &buf[0],
                               sizeof(buf) - 1,
                               0,
                               &addr);

        if (size < 0) {
            continue;
        }

        log_object_print(NULL,
                         LOG_INFO,
                         OSTR("connection from %s:%u\r\n"),
                         inet_ntoa(&addr.ip, &addrbuf[0]),
                         addr.port);
        buf[size] = '\0';
        handle_request(self_p, &buf[0], size + 1, &addr);
    }

    return (NULL);
}

int tftp_server_init(struct tftp_server_t *self_p,
                     struct inet_addr_t *addr_p,
                     int timeout_ms,
                     const char *name_p,
                     const char *root_p,
                     void *stack_p,
                     size_t stack_size)
{
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(addr_p != NULL, EINVAL);
    ASSERTN(name_p != NULL, EINVAL);
    ASSERTN(stack_p != NULL, EINVAL);

    self_p->addr = *addr_p;
    self_p->timeout_ms = timeout_ms;
    self_p->name_p = name_p;
    self_p->root_p = root_p;
    self_p->stack_p = stack_p;
    self_p->stack_size = stack_size;

    return (0);
}

int tftp_server_start(struct tftp_server_t *self_p)
{
    ASSERTN(self_p != NULL, EINVAL);

    self_p->thrd_p = thrd_spawn(tftp_server_main,
                                self_p,
                                0,
                                self_p->stack_p,
                                self_p->stack_size);

    return (self_p->thrd_p != NULL ? 0 : -1);
}
