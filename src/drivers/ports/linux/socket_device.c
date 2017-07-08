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
 * Message types.
 */
#define TYPE_UNSUPPORTED_TYPE                             (0)
#define TYPE_UART_DEVICE_REQUEST                          (1)
#define TYPE_UART_DEVICE_RESPONSE                         (2)
#define TYPE_PIN_DEVICE_REQUEST                           (3)
#define TYPE_PIN_DEVICE_RESPONSE                          (4)
#define TYPE_PWM_DEVICE_REQUEST                           (5)
#define TYPE_PWM_DEVICE_RESPONSE                          (6)
#define TYPE_CAN_DEVICE_REQUEST                           (7)
#define TYPE_CAN_DEVICE_RESPONSE                          (8)
#define TYPE_I2C_DEVICE_REQUEST                           (9)
#define TYPE_I2C_DEVICE_RESPONSE                         (10)

/**
 * Convert given device pointer to its index.
 */
#define UART_INDEX(dev_p) (dev_p - &uart_device[0])
#define PIN_INDEX(dev_p) (dev_p - &pin_device[0])
#define PWM_INDEX(dev_p) (dev_p - &pwm_device[0])
#define CAN_INDEX(dev_p) (dev_p - &can_device[0])
#define I2C_INDEX(dev_p) (dev_p - &i2c_device[0])

struct module_t {
    int initialized;
    pthread_t thrd;
};

/**
 * A setup message header.
 */
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

/**
 * The client types.
 */
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

struct pwm_client_t {
    int socket;
    struct pwm_device_t *dev_p;
    char name[64];
    pthread_t thrd;
};

struct can_client_t {
    int socket;
    struct can_device_t *dev_p;
    char name[64];
    pthread_t thrd;
};

struct i2c_client_t {
    int socket;
    struct i2c_device_t *dev_p;
    char name[64];
    pthread_t thrd;
};

static struct uart_client_t uart_clients[UART_DEVICE_MAX];
static struct pin_client_t pin_clients[PIN_DEVICE_MAX];
static struct pwm_client_t pwm_clients[PWM_DEVICE_MAX];
static struct can_client_t can_clients[CAN_DEVICE_MAX];
static struct i2c_client_t i2c_clients[I2C_DEVICE_MAX];
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
    fflush(stdout);

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
    fflush(stdout);

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
        index = -1;
    }

    /* Prepare the response. */
    response.header.type = htonl(TYPE_UART_DEVICE_RESPONSE);
    response.header.size = htonl(4);

    if ((index < 0) || (index >= UART_DEVICE_MAX)) {
        response.result = -ENODEV;
    } else if (uart_clients[index].socket >= 0) {
        response.result = -EADDRINUSE;
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

    if (response.result != 0) {
        return (-1);
    }

    /* Start the client thread if everything went well so far. */
    if (res == sizeof(response)) {
        uart_clients[index].socket = client;
        uart_clients[index].dev_p = &uart_device[index];
        strcpy(&uart_clients[index].name[0], device_p);
        res = pthread_create(&uart_clients[index].thrd,
                             NULL,
                             uart_client_main,
                             &uart_clients[index]);

        if (res != 0) {
            fprintf(stderr, "error: creating uart device thread\n");
        }
    }

    return (0);
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
    fflush(stdout);

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
    fflush(stdout);

    return (NULL);
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
            index = -1;
        }
    }

    /* Prepare the response. */
    response.header.type = htonl(TYPE_PIN_DEVICE_RESPONSE);
    response.header.size = htonl(4);

    if ((index < 0) || (index >= PIN_DEVICE_MAX)) {
        response.result = -ENODEV;
    } else if (pin_clients[index].socket >= 0) {
        response.result = -EADDRINUSE;
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

    if (response.result != 0) {
        return (-1);
    }

    /* Start the client thread if everything went well so far. */
    if (res == sizeof(response)) {
        pin_clients[index].socket = client;
        pin_clients[index].dev_p = &pin_device[index];
        strcpy(&pin_clients[index].name[0], device_p);
        res = pthread_create(&pin_clients[index].thrd,
                             NULL,
                             pin_client_main,
                             &pin_clients[index]);

        if (res != 0) {
            fprintf(stderr, "error: creating pin device thread\n");
        }
    }

    return (0);
}

#if CONFIG_PWM == 1

/**
 * Handle a pwm client connection.
 */
static void *pwm_client_main(void *arg_p)
{
    struct pwm_client_t *client_p;
    ssize_t size;
    uint8_t byte;

    client_p = arg_p;

    printf("socket_device: pwm device %s connected\n",
           &client_p->name[0]);
    fflush(stdout);

    while (1) {
        size = read(client_p->socket, &byte, sizeof(byte));

        if (size != sizeof(byte)) {
            break;
        }

        /* Discard input data for now. */
    }

    close(client_p->socket);
    client_p->socket = -2;

    printf("socket_device: pwm device %s disconnected\n",
           &client_p->name[0]);
    fflush(stdout);

    return (NULL);
}

static int handle_pwm_device_request(struct device_request_t *request_p,
                                      int client)
{
    struct device_response_t response;
    int res;
    long index;
    char *device_p;
    struct pwm_device_t *dev_p;

    res = 0;

    /* Parse the device name. */
    device_p = (char *)&request_p->device[0];

    index = board_pin_string_to_device_index(device_p);

    if (index >= 0) {
        dev_p = pwm_pin_to_device(&pin_device[index]);

        if (dev_p == NULL) {
            index = -1;
        } else {
            index = PWM_INDEX(dev_p);
        }
    } else {
        if (std_strtol(device_p, &index) == NULL) {
            index = -1;
        }
    }

    /* Prepare the response. */
    response.header.type = htonl(TYPE_PWM_DEVICE_RESPONSE);
    response.header.size = htonl(4);

    if ((index < 0) || (index >= PWM_DEVICE_MAX)) {
        response.result = -ENODEV;
    } else if (pwm_clients[index].socket >= 0) {
        response.result = -EADDRINUSE;
    } else {
        if (pwm_clients[index].socket == -2) {
            pthread_join(pwm_clients[index].thrd, NULL);
            pwm_clients[index].socket = -1;
        }

        response.result = 0;
    }

    /* Send the response. */
    response.result = htonl(response.result);
    res = write(client, &response, sizeof(response));

    if (response.result != 0) {
        return (-1);
    }

    /* Start the client thread if everything went well so far. */
    if (res == sizeof(response)) {
        pwm_clients[index].socket = client;
        pwm_clients[index].dev_p = &pwm_device[index];
        strcpy(&pwm_clients[index].name[0], device_p);
        res = pthread_create(&pwm_clients[index].thrd,
                             NULL,
                             pwm_client_main,
                             &pwm_clients[index]);

        if (res != 0) {
            fprintf(stderr, "error: creating pwm device thread\n");
        }
    }

    return (0);
}

#endif

/**
 * Handle a can client connection.
 */
static void *can_client_main(void *arg_p)
{
    struct can_client_t *client_p;
    ssize_t size;
    char buf[64];
    struct can_frame_t frame;
    int frame_size;
    long value;
    size_t i;
    int extended_frame;

    client_p = arg_p;

    printf("socket_device: can device %s connected\n",
           &client_p->name[0]);
    fflush(stdout);

    while (1) {
        size = read(client_p->socket, &buf[0], 35);

        if (size == 0) {
            break;
        } else if (size != 35) {
            printf("warning: bad can line size %d\n", (int)size);
            fflush(stdout);
            continue;
        }

        size = sscanf(&buf[0],
                      "id=%08x,extended=%d,size=%d,data=",
                      &frame.id,
                      &extended_frame,
                      &frame_size);

        frame.extended_frame = extended_frame;

        if (size != 3) {
            printf("warning: bad can message received: %s\n", &buf[0]);
            fflush(stdout);
            continue;
        }

        if ((frame.extended_frame != 0) && (frame.extended_frame != 1)) {
            printf("warning: bad can message exteneded frame: %d\n",
                   frame.extended_frame);
            fflush(stdout);
            continue;
        }

        if (frame_size > 8) {
            printf("warning: bad can message size: %d\n", frame_size);
            fflush(stdout);
            continue;
        }

        frame.size = frame_size;

        /* Read the data, one byte at a time. */
        buf[0] = '0';
        buf[1] = 'x';
        buf[4] = '\0';

        for (i = 0; i < frame_size; i++) {
            size = read(client_p->socket, &buf[2], 2);

            if (size == 0) {
                break;
            } else if (size != 2) {
                printf("warning: bad can line size %d\n", (int)size);
                fflush(stdout);
                size = -1;
                break;
            }

            if (std_strtol(&buf[0], &value) == NULL) {
                size = -1;
                break;
            }

            frame.data.u8[i] = value;
        }

        if (size == 0) {
            break;
        } else if (size == -1) {
            continue;
        }

        /* Read the line terminator. */
        read(client_p->socket, &buf[0], 2);

        sys_lock();

        if (client_p->dev_p->drv_p != NULL) {
            queue_write_isr(&client_p->dev_p->drv_p->chin,
                            &frame,
                            sizeof(frame));
        }

        sys_unlock();
    }

    close(client_p->socket);
    client_p->socket = -2;

    printf("socket_device: can device %s disconnected\n",
           &client_p->name[0]);
    fflush(stdout);

    return (NULL);
}

static int handle_can_device_request(struct device_request_t *request_p,
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
        index = -1;
    }

    /* Prepare the response. */
    response.header.type = htonl(TYPE_CAN_DEVICE_RESPONSE);
    response.header.size = htonl(4);

    if ((index < 0) || (index >= CAN_DEVICE_MAX)) {
        response.result = -ENODEV;
    } else if (can_clients[index].socket >= 0) {
        response.result = -EADDRINUSE;
    } else {
        if (can_clients[index].socket == -2) {
            pthread_join(can_clients[index].thrd, NULL);
            can_clients[index].socket = -1;
        }

        response.result = 0;
    }

    /* Send the response. */
    response.result = htonl(response.result);
    res = write(client, &response, sizeof(response));

    if (response.result != 0) {
        return (-1);
    }

    /* Start the client thread if everything went well so far. */
    if (res == sizeof(response)) {
        can_clients[index].socket = client;
        can_clients[index].dev_p = &can_device[index];
        strcpy(&can_clients[index].name[0], device_p);
        res = pthread_create(&can_clients[index].thrd,
                             NULL,
                             can_client_main,
                             &can_clients[index]);

        if (res != 0) {
            fprintf(stderr, "error: creating can device thread\n");
        }
    }

    return (0);
}

/**
 * Handle a i2c client connection.
 */
static void *i2c_client_main(void *arg_p)
{
    struct i2c_client_t *client_p;
    ssize_t size;
    uint8_t byte;

    client_p = arg_p;

    printf("socket_device: i2c device %s connected\n",
           &client_p->name[0]);
    fflush(stdout);

    while (1) {
        size = read(client_p->socket, &byte, sizeof(byte));

        if (size == 0) {
            break;
        }

        /* Discard input data for now. */
    }

    close(client_p->socket);
    client_p->socket = -2;

    printf("socket_device: i2c device %s disconnected\n",
           &client_p->name[0]);
    fflush(stdout);

    return (NULL);
}

static int handle_i2c_device_request(struct device_request_t *request_p,
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
        index = -1;
    }

    /* Prepare the response. */
    response.header.type = htonl(TYPE_I2C_DEVICE_RESPONSE);
    response.header.size = htonl(4);

    if ((index < 0) || (index >= I2C_DEVICE_MAX)) {
        response.result = -ENODEV;
    } else if (i2c_clients[index].socket >= 0) {
        response.result = -EADDRINUSE;
    } else {
        if (i2c_clients[index].socket == -2) {
            pthread_join(i2c_clients[index].thrd, NULL);
            i2c_clients[index].socket = -1;
        }

        response.result = 0;
    }

    /* Send the response. */
    response.result = htonl(response.result);
    res = write(client, &response, sizeof(response));

    if (response.result != 0) {
        return (-1);
    }

    /* Start the client thread if everything went well so far. */
    if (res == sizeof(response)) {
        i2c_clients[index].socket = client;
        i2c_clients[index].dev_p = &i2c_device[index];
        strcpy(&i2c_clients[index].name[0], device_p);
        res = pthread_create(&i2c_clients[index].thrd,
                             NULL,
                             i2c_client_main,
                             &i2c_clients[index]);

        if (res != 0) {
            fprintf(stderr, "error: creating i2c device thread\n");
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

    freeaddrinfo(addr_p);

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
    struct header_t response;
    int res;

    listener = setup_listener();

    if (listener < 0) {
        printf("warning: socket_device: failed to setup listener socket\n");
        fflush(stdout);

        return (NULL);
    }

    printf("info: socket_device: listening for clients on TCP port 47000\n");
    fflush(stdout);

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
#if CONFIG_PWM == 1
        } else if (request.header.type == TYPE_PWM_DEVICE_REQUEST) {
            res = handle_pwm_device_request(&request, client);
#endif
        } else if (request.header.type == TYPE_CAN_DEVICE_REQUEST) {
            res = handle_can_device_request(&request, client);
        } else if (request.header.type == TYPE_I2C_DEVICE_REQUEST) {
            res = handle_i2c_device_request(&request, client);
        } else {
            /* Send the response. */
            response.type = htonl(TYPE_UNSUPPORTED_TYPE);
            response.size = htonl(0);
            write(client, &response, sizeof(response));
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

    for (i = 0; i < membersof(pwm_clients); i++) {
        pwm_clients[i].socket = -1;
    }

    for (i = 0; i < membersof(can_clients); i++) {
        can_clients[i].socket = -1;
    }

    for (i = 0; i < membersof(i2c_clients); i++) {
        i2c_clients[i].socket = -1;
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

int socket_device_is_pwm_device_connected_isr(
    const struct pwm_device_t *dev_p)
{
    int client;

    client = pwm_clients[PWM_INDEX(dev_p)].socket;

    return (client >= 0);
}

ssize_t socket_device_pwm_device_write_isr(const struct pwm_device_t *dev_p,
                                           const void *buf_p,
                                           size_t size)
{
    return (write(pwm_clients[PWM_INDEX(dev_p)].socket, buf_p, size));
}

int socket_device_is_can_device_connected_isr(
    const struct can_device_t *dev_p)
{
    int client;

    client = can_clients[CAN_INDEX(dev_p)].socket;

    return (client >= 0);
}

ssize_t socket_device_can_device_write_isr(const struct can_device_t *dev_p,
                                           const void *buf_p,
                                           size_t size)
{
    const struct can_frame_t *frame_p;
    char buf[64];
    ssize_t res;
    int socket;
    size_t i;

    socket = can_clients[CAN_INDEX(dev_p)].socket;

    frame_p = (struct can_frame_t *)buf_p;

    /* Format the line to send to the client. */
    sprintf(&buf[0],
            "id=%08x,extended=%d,size=%d,data=",
            frame_p->id,
            (int)frame_p->extended_frame,
            (int)frame_p->size);

    res = write(socket, &buf[0], strlen(&buf[0]));

    if (res != strlen(&buf[0])) {
        return (-1);
    }

    /* Write the data, one byte at a time. */
    for (i = 0; i < frame_p->size; i++) {
        sprintf(&buf[0], "%02x", frame_p->data.u8[i]);

        res = write(socket, &buf[0], 2);

        if (res != 2) {
            return (-1);
        }
    }

    res = write(socket, "\r\n", 2);

    if (res != 2) {
        return (-1);
    }

    return (size);
}

int socket_device_is_i2c_device_connected_isr(
    const struct i2c_device_t *dev_p)
{
    int client;

    client = i2c_clients[I2C_INDEX(dev_p)].socket;

    return (client >= 0);
}

ssize_t socket_device_i2c_device_write_isr(const struct i2c_device_t *dev_p,
                                           int address,
                                           const void *buf_p,
                                           size_t size)
{
    char buf[64];
    ssize_t res;
    int socket;
    size_t i;
    const uint8_t *byte_p;

    socket = i2c_clients[I2C_INDEX(dev_p)].socket;

    /* Write the address. */
    sprintf(&buf[0], "address=%04x,size=%04lx,data=", address, size);

    res = write(socket, &buf[0], strlen(&buf[0]));

    if (res != strlen(&buf[0])) {
        return (-1);
    }

    /* Write the data, one byte at a time. */
    byte_p = buf_p;

    for (i = 0; i < size; i++) {
        sprintf(&buf[0], "%02x", byte_p[i]);

        res = write(socket, &buf[0], 2);

        if (res != 2) {
            return (-1);
        }
    }

    res = write(socket, "\r\n", 2);

    if (res != 2) {
        return (-1);
    }

    return (size);
}
