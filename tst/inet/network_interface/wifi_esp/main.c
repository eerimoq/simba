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

#if !defined(SSID)
#    pragma message "WiFi connection variable SSID is not set. Using default value MySSID"
#    define SSID ssid
#endif

#if !defined(PASSWORD)
#    pragma message "WiFi connection variable PASSWORD is not set. Using default value MyPassword"
#    define PASSWORD password
#endif

#if !defined(ESP_IP)
#    if defined(ARCH_ESP)
#        pragma message "WiFi connection variable ESP_IP is not set. Using default value 192.168.0.5"
#        define ESP_IP 192.168.0.8
#    else
#        pragma message "WiFi connection variable ESP_IP is not set. Using default value 192.168.0.7"
#        define ESP_IP 192.168.0.7
#    endif
#endif

#if !defined(SERVER_IP)
#    pragma message "WiFi connection variable SERVER_IP is not set. Using default value 192.168.0.4"
#    define SERVER_IP 192.168.0.4
#endif

/* Ports. */
#define UDP_PORT               30303
#define TCP_PORT               40404
#define TCP_PORT_WRITE_CLOSE   40405
#define TCP_PORT_SIZES         40406
#define TCP_PORT_NOT_USED      40407

static struct network_interface_wifi_t wifi_sta;
static struct network_interface_wifi_t wifi_ap;
static uint8_t buffer[5000];
static struct event_t event;

static THRD_STACK(udp_reader_stack, 2048);

/**
 * The UDP reader thread reads a packet and writes it back on the same
 * socket.
 */
static void *udp_reader(void *arg_p)
{
    struct socket_t *socket_p;
    struct inet_addr_t addr;
    char buf[16];
    ssize_t size;
    char addrbuf[20];
    uint32_t mask;

    socket_p = arg_p;

    std_printf(FSTR("reader recvfrom\r\n"));

    size = socket_recvfrom(socket_p, buf, sizeof(buf), 0, &addr);
    BTASSERTN(size == 9);
    buf[size] = '\0';
    std_printf(FSTR("reader received '%s' from %s:%d\r\n"),
               buf,
               inet_ntoa(&addr.ip, addrbuf),
               addr.port);

    std_printf(FSTR("reader sending '%s' to %s:%d\r\n"),
               buf,
               inet_ntoa(&addr.ip, addrbuf),
               addr.port);
    BTASSERTN(socket_sendto(socket_p, buf, size, 0, &addr) == size);

    mask = 0x1;
    event_write(&event, &mask, sizeof(mask));

    thrd_suspend(NULL);

    return (NULL);
}


static int test_init(struct harness_t *harness_p)
{
    struct inet_if_ip_info_t info;
    char buf[20];

    BTASSERT(esp_wifi_module_init() == 0);
    BTASSERT(network_interface_module_init() == 0);
    BTASSERT(socket_module_init() == 0);

    /* Initialize a WiFi in station mode with given SSID and
       password. */
    BTASSERT(network_interface_wifi_init(&wifi_sta,
                                         "esp-wlan-sta",
                                         &network_interface_wifi_driver_esp_station,
                                         NULL,
                                         STRINGIFY(SSID),
                                         STRINGIFY(PASSWORD)) == 0);
    BTASSERT(network_interface_add(&wifi_sta.network_interface) == 0);
    BTASSERT(network_interface_start(&wifi_sta.network_interface) == 0);

    /* Setup a SoftAP as well. */
    BTASSERT(network_interface_wifi_init(&wifi_ap,
                                         "esp-wlan-ap",
                                         &network_interface_wifi_driver_esp_softap,
                                         NULL,
                                         "Simba",
                                         "password") == 0);
    BTASSERT(network_interface_add(&wifi_ap.network_interface) == 0);
    BTASSERT(network_interface_start(&wifi_ap.network_interface) == 0);

    std_printf(FSTR("Connecting to SSID=%s\r\n"), STRINGIFY(SSID));

    /* Wait for a connection to the WiFi access point. */
    while (network_interface_is_up(&wifi_sta.network_interface) == 0) {
        std_printf(FSTR("."));
        thrd_sleep(1);
    }

    std_printf(FSTR("\r\n"));

    BTASSERT(network_interface_get_ip_info(&wifi_sta.network_interface,
                                           &info) == 0);

    std_printf(FSTR("Connected to Access Point (AP). Got IP %s.\r\n"),
               inet_ntoa(&info.address, buf));

    BTASSERT(network_interface_get_by_name("esp-wlan-sta")
             == &wifi_sta.network_interface);
    BTASSERT(network_interface_get_by_name("esp-wlan-ap")
             == &wifi_ap.network_interface);
    BTASSERT(network_interface_get_by_name("none") == NULL);

    return (0);
}

static int test_udp(struct harness_t *harness_p)
{
    struct socket_t sock;
    struct inet_addr_t addr;
    char buf[16];
    char addrbuf[20];
    size_t size;
    struct chan_list_t list;
    int workspace[16];
    uint32_t mask;

    event_init(&event);

    BTASSERT(chan_list_init(&list, workspace, sizeof(workspace)) == 0);

    std_printf(FSTR("UDP test\r\n"));

    std_printf(FSTR("opening socket\r\n"));
    BTASSERT(socket_open_udp(&sock) == 0);

    BTASSERT(chan_list_add(&list, &sock) == 0);

    std_printf(FSTR("binding to %d\r\n"), UDP_PORT);
    inet_aton(STRINGIFY(ESP_IP), &addr.ip);
    addr.port = UDP_PORT;
    BTASSERT(socket_bind(&sock, &addr) == 0);

    std_printf(FSTR("recvfrom\r\n"));

    size = socket_recvfrom(&sock, buf, sizeof(buf), 0, &addr);
    BTASSERT(size == 9);
    buf[size] = '\0';
    std_printf(FSTR("received '%s' from %s:%d\r\n"),
               buf,
               inet_ntoa(&addr.ip, addrbuf),
               addr.port);

    /* Spawn a thread that read a packet and resumes this thread. This
       tests that it's possible to send a UDP packet when another
       thread is reading from the same socket. */
    thrd_spawn(udp_reader,
               &sock,
               0,
               udp_reader_stack,
               sizeof(udp_reader_stack));

    thrd_sleep(0.3);

    std_printf(FSTR("sending '%s' to %s:%d\r\n"),
               buf,
               inet_ntoa(&addr.ip, addrbuf),
               addr.port);
    BTASSERT(socket_sendto(&sock, buf, size, 0, &addr) == size);

    mask = 0x1;
    event_read(&event, &mask, sizeof(mask));

    std_printf(FSTR("read event\r\n"));

    BTASSERT(chan_list_poll(&list, NULL) == &sock);

    std_printf(FSTR("packet available\r\n"));

    size = socket_recvfrom(&sock, buf, sizeof(buf), 0, &addr);
    BTASSERT(size == 9);
    buf[size] = '\0';
    std_printf(FSTR("received '%s' from %s:%d\r\n"),
               buf,
               inet_ntoa(&addr.ip, addrbuf),
               addr.port);

    std_printf(FSTR("sending '%s' to %s:%d\r\n"),
               buf,
               inet_ntoa(&addr.ip, addrbuf),
               addr.port);
    BTASSERT(socket_sendto(&sock, buf, size, 0, &addr) == size);

    std_printf(FSTR("closing socket\r\n"));
    BTASSERT(socket_close(&sock) == 0);

    return (0);
}

static int test_tcp(struct harness_t *harness_p)
{
    struct socket_t listener;
    struct socket_t client;
    struct inet_addr_t addr;
    char buf[16];
    char addrbuf[20];
    size_t size;
    struct chan_list_t list;
    int workspace[16];

    BTASSERT(chan_list_init(&list, workspace, sizeof(workspace)) == 0);

    std_printf(FSTR("TCP test\r\n"));

    std_printf(FSTR("opening listener socket\r\n"));
    BTASSERT(socket_open_tcp(&listener) == 0);

    std_printf(FSTR("binding to %d\r\n"), TCP_PORT);
    inet_aton(STRINGIFY(ESP_IP), &addr.ip);
    addr.port = TCP_PORT;
    BTASSERT(socket_bind(&listener, &addr) == 0);

    BTASSERT(socket_listen(&listener, 5) == 0);

    std_printf(FSTR("listening on %d\r\n"), TCP_PORT);

    BTASSERT(socket_accept(&listener, &client, &addr) == 0);
    std_printf(FSTR("accepted client %s:%d\r\n"),
               inet_ntoa(&addr.ip, addrbuf),
               addr.port);

    BTASSERT(chan_list_add(&list, &client) == 0);

    size = socket_read(&client, buf, 5);
    BTASSERT(size == 5);
    size += socket_read(&client, &buf[5], 4);
    BTASSERT(size == 9);
    buf[size] = '\0';
    std_printf(FSTR("read %d bytes: '%s'\r\n"), size, buf);

    std_printf(FSTR("writing '%s'\r\n"), buf);
    BTASSERT(socket_write(&client, buf, size) == size);

    BTASSERT(chan_list_poll(&list, NULL) == &client);

    size = socket_read(&client, buf, 5);
    BTASSERT(size == 5);
    size += socket_read(&client, &buf[5], 4);
    BTASSERT(size == 9);
    buf[size] = '\0';
    std_printf(FSTR("read %d bytes: '%s'\r\n"), size, buf);

    std_printf(FSTR("writing '%s'\r\n"), buf);
    BTASSERT(socket_write(&client, buf, size) == size);

    std_printf(FSTR("closing client socket\r\n"));
    BTASSERT(socket_close(&client) == 0);

    std_printf(FSTR("closing listener socket\r\n"));
    BTASSERT(socket_close(&listener) == 0);

    return (0);
}

static int test_tcp_write_close(struct harness_t *harness_p)
{
    struct socket_t listener;
    struct socket_t client;
    struct inet_addr_t addr;
    char addrbuf[20];

    std_printf(FSTR("TCP test\r\n"));

    std_printf(FSTR("opening listener socket\r\n"));
    BTASSERT(socket_open_tcp(&listener) == 0);

    std_printf(FSTR("binding to %d\r\n"), TCP_PORT_WRITE_CLOSE);
    inet_aton(STRINGIFY(ESP_IP), &addr.ip);
    addr.port = TCP_PORT_WRITE_CLOSE;
    BTASSERT(socket_bind(&listener, &addr) == 0);

    BTASSERT(socket_listen(&listener, 5) == 0);

    std_printf(FSTR("listening on %d\r\n"), TCP_PORT_WRITE_CLOSE);

    BTASSERT(socket_accept(&listener, &client, &addr) == 0);
    std_printf(FSTR("accepted client %s:%d\r\n"),
               inet_ntoa(&addr.ip, addrbuf),
               addr.port);

    thrd_sleep(1);

    BTASSERT(socket_read(&client, &buffer[0], 533) == 533);

    /* The socket is closed by the peer. */
    std_printf(FSTR("reading from closed socket\r\n"));
    BTASSERT(socket_read(&client, &buffer[533], 1) == 0);

    std_printf(FSTR("closing client socket\r\n"));
    BTASSERT(socket_close(&client) == 0);

    std_printf(FSTR("closing listener socket\r\n"));
    BTASSERT(socket_close(&listener) == 0);

    return (0);
}

static int test_tcp_sizes(struct harness_t *harness_p)
{
    struct socket_t listener;
    struct socket_t client;
    struct inet_addr_t addr;
    char addrbuf[20];
    ssize_t size;
    size_t offset;
    struct chan_list_t list;
    int workspace[16];

    std_printf(FSTR("TCP test\r\n"));

    BTASSERT(chan_list_init(&list, workspace, sizeof(workspace)) == 0);

    std_printf(FSTR("opening listener socket\r\n"));
    BTASSERT(socket_open_tcp(&listener) == 0);

    BTASSERT(chan_list_add(&list, &listener) == 0);

    std_printf(FSTR("binding to %d\r\n"), TCP_PORT_SIZES);
    inet_aton(STRINGIFY(ESP_IP), &addr.ip);
    addr.port = TCP_PORT_SIZES;
    BTASSERT(socket_bind(&listener, &addr) == 0);

    BTASSERT(socket_listen(&listener, 5) == 0);

    std_printf(FSTR("listening on %d\r\n"), TCP_PORT_SIZES);

    BTASSERT(chan_list_poll(&list, NULL) == &listener);

    BTASSERT(socket_accept(&listener, &client, &addr) == 0);
    std_printf(FSTR("accepted client %s:%d\r\n"),
               inet_ntoa(&addr.ip, addrbuf),
               addr.port);

    /* Range of packet sizes. */
    for (size = 1; size < sizeof(buffer); size += 128) {
        BTASSERT(socket_read(&client, buffer, size) == size);
        BTASSERT(socket_write(&client, buffer, size) == size);
    }

    /* Send a 1800 bytes packet and recieve small chunks of it. */
    size = 0;
    offset = 0;

    while (offset < 1800) {
        size = (1800 - offset);

        if (size > 128) {
            size = 128;
        }

        size = socket_read(&client, &buffer[offset], size);
        std_printf(FSTR("read %d bytes at offset %d\r\n"), size, offset);
        BTASSERT((size > 0) && (size <= 128));
        offset += size;
    }

    BTASSERT(socket_write(&client, buffer, offset) == offset);

    std_printf(FSTR("closing client socket\r\n"));
    BTASSERT(socket_close(&client) == 0);

    std_printf(FSTR("closing listener socket\r\n"));
    BTASSERT(socket_close(&listener) == 0);

    return (0);
}

static int test_tcp_connect(struct harness_t *harness_p)
{
    struct socket_t socket;
    struct inet_addr_t addr;

    BTASSERT(socket_open_tcp(&socket) == 0);

    std_printf(FSTR("connecting to %d\r\n"), TCP_PORT_NOT_USED);
    inet_aton(STRINGIFY(SERVER_IP), &addr.ip);
    addr.port = TCP_PORT_NOT_USED;
    BTASSERT(socket_connect(&socket, &addr) == -1);

    std_printf(FSTR("closing socket\r\n"));
    BTASSERT(socket_close(&socket) == 0);

    return (0);
}

static int test_print(struct harness_t *harness_p)
{
    char command[64];

    strcpy(command, "filesystems/fs/counters/list");
    BTASSERT(fs_call(command, NULL, sys_get_stdout(), NULL) == 0);

    strcpy(command, "drivers/esp_wifi/status");
    BTASSERT(fs_call(command, NULL, sys_get_stdout(), NULL) == 0);

    strcpy(command, "inet/network_interface/list");
    BTASSERT(fs_call(command, NULL, sys_get_stdout(), NULL) == 0);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_init, "test_init" },
        { test_udp, "test_udp" },
        { test_tcp, "test_tcp" },
        { test_tcp_write_close, "test_tcp_write_close" },
        { test_tcp_sizes, "test_tcp_sizes" },
        { test_tcp_connect, "test_tcp_connect" },
        { test_print, "test_print" },
        { NULL, NULL }
    };

    sys_start();
    inet_module_init();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
