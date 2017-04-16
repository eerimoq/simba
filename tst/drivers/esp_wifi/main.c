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

static int test_init(struct harness_t *harness_p)
{
    BTASSERT(esp_wifi_module_init() == 0);
    BTASSERT(esp_wifi_set_op_mode(esp_wifi_op_mode_null_t) == 0);

    esp_wifi_print(sys_get_stdout());

    return (0);
}

static int test_softap(struct harness_t *harness_p)
{
    int number_of_stations;
    int i;
    
    BTASSERT(esp_wifi_set_op_mode(esp_wifi_op_mode_softap_t) == 0);
    BTASSERT(esp_wifi_softap_init("Simba",
                                  "password") == 0);
    BTASSERT(esp_wifi_softap_dhcp_server_status()
             == esp_wifi_dhcp_status_running_t);

    esp_wifi_print(sys_get_stdout());
    
    /* Wait for a station to connect, or at most 30 seconds. */
    std_printf(FSTR("Connect your wireless device to this access point.\r\n"));
    i = 1;
    number_of_stations = 0;

    while ((number_of_stations <= 0) && (i <= 30)) {
        number_of_stations = esp_wifi_softap_get_number_of_connected_stations();
        std_printf(FSTR("%d: %d stations connected.\r\n"), i, number_of_stations);
        thrd_sleep(1);
        i++;
    }

    BTASSERT(esp_wifi_set_op_mode(esp_wifi_op_mode_null_t) == 0);

    return (0);
}

static int test_station(struct harness_t *harness_p)
{
    enum esp_wifi_station_status_t status;

    BTASSERT(esp_wifi_set_op_mode(esp_wifi_op_mode_station_t) == 0);
    BTASSERT(esp_wifi_station_init(STRINGIFY(SSID),
                                   STRINGIFY(PASSWORD),
                                   NULL,
                                   NULL) == 0);
    BTASSERT(esp_wifi_station_connect() == 0);

    esp_wifi_print(sys_get_stdout());

    /* Wait until connected. */
    status = esp_wifi_station_status_idle_t;

    while (status != esp_wifi_station_status_got_ip_t) {
        status = esp_wifi_station_get_status();
        std_printf(FSTR("status: %d\r\n"), status);
        thrd_sleep(1);
    }

    esp_wifi_print(sys_get_stdout());

    BTASSERT(esp_wifi_set_op_mode(esp_wifi_op_mode_null_t) == 0);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_init, "test_init" },
        { test_softap, "test_softap" },
        { test_station, "test_station" },
        { NULL, NULL }
    };

    sys_start();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
