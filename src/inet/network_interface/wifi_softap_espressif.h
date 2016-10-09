#ifndef __INET_NETWORK_INTERFACE_WIFI_SOFTAP_ESPRESSIF_H__
#define __INET_NETWORK_INTERFACE_WIFI_SOFTAP_ESPRESSIF_H__

#include "simba.h"

#include "esp_misc.h"
#include "esp_wifi.h"
#include "esp_softap.h"

struct network_interface_wifi_softap_espressif_t {
    struct network_interface_t network_interface;
};


int network_interface_wifi_softap_espressif_module_init(void);

int network_interface_wifi_softap_espressif_init(struct network_interface_wifi_softap_espressif_t *self_p,
                                                  const uint8_t *ssid_p,
                                                  const uint8_t *password_p);

int network_interface_wifi_softap_espressif_start(struct network_interface_wifi_softap_espressif_t *self_p);

#endif
