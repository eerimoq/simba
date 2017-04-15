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

#include "socket_device.h"

#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

/**
 * Setup message types.
 */
#define TYPE_UART_DEVICE_REQUEST                          (1)
#define TYPE_UART_DEVICE_RESPONSE                         (2)
#define TYPE_PIN_DEVICE_REQUEST                           (3)
#define TYPE_PIN_DEVICE_RESPONSE                          (4)

/**
 * Convert given device pointer to its index.
 */
#define UART_INDEX(dev_p) (dev_p - &uart_device[0])
#define PIN_INDEX(dev_p) (dev_p - &pin_device[0])

struct module_t {
    int initialized;
    pthread_t thrd;
};

struct header_t {
    uint32_t type;
    uint32_t size;
};

struct device_request_t {
    struct header_t header;
    uint8_t device[64];
};

struct device_response_t {
    struct header_t header;
    int32_t result;
};

struct uart_client_t {
    int socket;
    struct uart_device_t *dev_p;
    char name[64];
    pthread_t thrd;
};

struct pin_client_t {
    int socket;
    struct pin_device_t *dev_p;
    char name[64];
    pthread_t thrd;
};

static struct uart_client_t uart_clients[UART_DEVICE_MAX];
static struct pin_client_t pin_clients[PIN_DEVICE_MAX];
static struct module_t module;

/**
 * Handle a UART client connection.
 */
static void *uart_client_main(void *arg_p)
{
    struct uart_client_t *client_p;
    ssize_t size;
    uint8_t byte;

    client_p = arg_p;

    printf("socket_device: uart device %s connected\n",
           &client_p->name[0]);

    while (1) {
        size = read(client_p->socket, &byte, sizeof(byte));

        if (size != sizeof(byte)) {
            break;
        }

        sys_lock();

        if (client_p->dev_p->drv_p != NULL) {
            queue_write_isr(&client_p->dev_p->drv_p->chin,
                            &byte,
                            sizeof(byte));
        }

        sys_unlock();
    }

    close(client_p->socket);
    client_p->socket = -2;

    printf("socket_device: uart device %s disconnected\n",
           &client_p->name[0]);

    return (NULL);
}

/**
 * Handle a pin client connection.
 */
static void *pin_client_main(void *arg_p)
{
    struct pin_client_t *client_p;
    ssize_t size;
    uint8_t byte;

    client_p = arg_p;

    printf("socket_device: pin device %s connected\n",
           &client_p->name[0]);

    while (1) {
        size = read(client_p->socket, &byte, sizeof(byte));

        if (size != sizeof(byte)) {
            break;
        }

        /* Discard input data for now. */
    }

    close(client_p->socket);
    client_p->socket = -2;

    printf("socket_device: pin device %s disconnected\n",
           &client_p->name[0]);

    return (NULL);
}

static int handle_uart_device_request(struct device_request_t *request_p,
                                      int client)
{
    struct device_response_t response;
    int res;
    long index;
    char *device_p;

    res = 0;

    /* Parse the device name. */
    device_p = (char *)&request_p->device[0];

    if (std_strtol(device_p, &index) == NULL) {
        return (-1);
    }

    /* Prepare the response. */
    response.header.type = htonl(TYPE_UART_DEVICE_RESPONSE);
    response.header.size = htonl(4);

    if ((index >= UART_DEVICE_MAX) || (uart_clients[index].socket >= 0)) {
        response.result = -1;
    } else {
        if (uart_clients[index].socket == -2) {
            pthread_join(uart_clients[index].thrd, NULL);
            uart_clients[index].socket = -1;
        }

        response.result = 0;
    }

    /* Send the response. */
    response.result = htonl(response.result);
    res = write(client, &response, sizeof(response));

    /* Start the client thread if everything went well so far. */
    if (res == sizeof(response)) {
        uart_clients[index].socket = client;
        uart_clients[index].dev_p = &uart_device[index];
        strcpy(&uart_clients[index].name[0], device_p);
        res = pthread_create(&uart_clients[0].thrd,
                             NULL,
                             uart_client_main,
                             &uart_clients[index]);

        if (res != 0) {
            fprintf(stderr, "error: creating uart device thread\n");
        }
    }

    return (0);
}

static int handle_pin_device_request(struct device_request_t *request_p,
                                      int client)
{
    struct device_response_t response;
    int res;
    long index;
    char *device_p;

    res = 0;

    /* Parse the device name. */
    device_p = (char *)&request_p->device[0];

    index = board_pin_string_to_device_index(device_p);

    if (index < 0) {
        if (std_strtol(device_p, &index) == NULL) {
            return (-1);
        }
    }

    /* Prepare the response. */
    response.header.type = htonl(TYPE_PIN_DEVICE_RESPONSE);
    response.header.size = htonl(4);

    if ((index >= PIN_DEVICE_MAX) || (pin_clients[index].socket >= 0)) {
        response.result = -1;
    } else {
        if (pin_clients[index].socket == -2) {
            pthread_join(pin_clients[index].thrd, NULL);
            pin_clients[index].socket = -1;
        }

        response.result = 0;
    }

    /* Send the response. */
    response.result = htonl(response.result);
    res = write(client, &response, sizeof(response));

    /* Start the client thread if everything went well so far. */
    if (res == sizeof(response)) {
        pin_clients[index].socket = client;
        pin_clients[index].dev_p = &pin_device[index];
        strcpy(&pin_clients[index].name[0], device_p);
        res = pthread_create(&pin_clients[0].thrd,
                             NULL,
                             pin_client_main,
                             &pin_clients[index]);

        if (res != 0) {
            fprintf(stderr, "error: creating pin device thread\n");
        }
    }

    return (0);
}

/**
 * Setup the listener socket.
 */
static int setup_listener(void)
{
    int listener;
    int yes;
    int res;
    struct addrinfo hints, *addr_p;

    /* Use an IPv4 TCP socket. */
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    res = getaddrinfo(NULL, "47000", &hints, &addr_p);

    if (res != 0) {
        perror("uart_device: getaddrinfo");

        return (-1);
    }

    listener = socket(addr_p->ai_family,
                      addr_p->ai_socktype,
                      addr_p->ai_protocol);

    if (listener == -1) {
        perror("socket_device: socket");
        res = -1;

        goto free_addr_info;
    }

    yes = 1;
    res = setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

    if (res == -1) {
        perror("socket_device: setsockopt");

        goto close_socket;
    }

    res = bind(listener, addr_p->ai_addr, addr_p->ai_addrlen);

    if (res == -1) {
        perror("socket_device: bind");

        goto close_socket;
    }

    res = listen(listener, 3);

    if (res == -1) {
        perror("socket_device: listen");

        goto close_socket;
    }

    return (listener);

 close_socket:
    close(listener);

 free_addr_info:
    freeaddrinfo(addr_p);

    return (res);
}

/**
 * Entry function of the socket device listener thread.
 */
static void *listener_main(void *arg_p)
{
    int listener;
    int client;
    ssize_t size;
    struct device_request_t request;
    int res;

    listener = setup_listener();

    if (listener < 0) {
        printf("warning: failed to setup socket device listener socket\n");

        return (NULL);
    }

    while (1) {
        client = accept(listener, NULL, NULL);

        if (client == -1) {
            perror("socket_device: accept");
            continue;
        }

        /* Read the request header. */
        size = read(client, &request.header, sizeof(request.header));

        if (size != sizeof(request.header)) {
            perror("socket_device: read request");
            close(client);
            continue;
        }

        /* Host byte order. */
        request.header.type = ntohl(request.header.type);
        request.header.size = ntohl(request.header.size);

        /* Validate the size. */
        if (request.header.size >= sizeof(request.device)) {
            perror("socket_device: read request size");
            close(client);
            continue;
        }

        /* Read the device name. */
        size = read(client, &request.device[0], request.header.size);

        if (size != request.header.size) {
            perror("socket_device: read request device name size");
            close(client);
            continue;
        }

        request.device[request.header.size] = '\0';

        /* Handle the request type. */
        if (request.header.type == TYPE_UART_DEVICE_REQUEST) {
            res = handle_uart_device_request(&request, client);
        } else if (request.header.type == TYPE_PIN_DEVICE_REQUEST) {
            res = handle_pin_device_request(&request, client);
        } else {
            res = -1;
        }

        if (res != 0) {
            close(client);
        }
    }

    return (NULL);
}

int socket_device_module_init()
{
    int i;
    int res;

    if (module.initialized == 1) {
        return (0);
    }

    module.initialized = 1;

    for (i = 0; i < membersof(pin_clients); i++) {
        pin_clients[i].socket = -1;
    }

    for (i = 0; i < membersof(uart_clients); i++) {
        uart_clients[i].socket = -1;
    }

#if CONFIG_LINUX_SOCKET_DEVICE == 1

    res = pthread_create(&module.thrd, NULL, listener_main, NULL);

    if (res != 0) {
        fprintf(stderr, "error: creating uart device thread\n");
    }

#else

    (void)listener_main;
    res = 0;

#endif

    return (res);
}

int socket_device_is_uart_device_connected_isr(
    const struct uart_device_t *dev_p)
{
    int client;

    client = uart_clients[UART_INDEX(dev_p)].socket;

    return (client >= 0);
}

ssize_t socket_device_uart_device_write_isr(
    const struct uart_device_t *dev_p,
    const void *buf_p,
    size_t size)
{
    return (write(uart_clients[UART_INDEX(dev_p)].socket, buf_p, size));
}

int socket_device_is_pin_device_connected_isr(
    const struct pin_device_t *dev_p)
{
    int client;

    client = pin_clients[PIN_INDEX(dev_p)].socket;

    return (client >= 0);
}

ssize_t socket_device_pin_device_write_isr(const struct pin_device_t *dev_p,
                                           const void *buf_p,
                                           size_t size)
{
    return (write(pin_clients[PIN_INDEX(dev_p)].socket, buf_p, size));
}
