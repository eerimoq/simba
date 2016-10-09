#include "wifi_softap_espressif.h"

#if defined(ARCH_ESP)

static struct softap_config softap_config;
static struct ip_info ip_config;

static err_t init()
{
    return (ERR_OK);
}

static int start(struct network_interface_t *netif_p)
{
    struct network_interface_wifi_softap_espressif_t *self_p;

    self_p = container_of(netif_p,
                          struct network_interface_wifi_softap_espressif_t,
                          network_interface);

    return (network_interface_wifi_softap_espressif_start(self_p));
}

static int stop(struct network_interface_t *netif_p)
{
    return (0);
}

static int is_up(struct network_interface_t *netif_p)
{
    return (1);
}

int network_interface_wifi_softap_espressif_module_init(void)
{
    return (0);
}

int network_interface_wifi_softap_espressif_init(struct network_interface_wifi_softap_espressif_t *self_p,
                                                  const uint8_t *ssid_p,
                                                  const uint8_t *password_p)
{
    ASSERTN(self_p != NULL, -EINVAL);
    ASSERTN(ssid_p != NULL, -EINVAL);
    ASSERTN(password_p != NULL, -EINVAL);

    self_p->network_interface.name_p = "esp-softap";
    self_p->network_interface.init = init;
    self_p->network_interface.start = start;
    self_p->network_interface.stop = stop;
    self_p->network_interface.is_up = is_up;

    wifi_set_opmode_current(SOFTAP_MODE | wifi_get_opmode());

    memset(&softap_config, 0, sizeof(softap_config));
    wifi_softap_get_config(&softap_config);
    softap_config.authmode = AUTH_WPA2_PSK;
    softap_config.ssid_len = 0;
    softap_config.max_connection = 4;
    std_sprintf((char *)softap_config.ssid, "%s", ssid_p);
    std_sprintf((char *)softap_config.password, "%s", password_p);
    wifi_softap_set_config(&softap_config);

    wifi_softap_dhcps_start();

    std_printf("wifi_softap_start\r\n");

    return (0);
}

int network_interface_wifi_softap_espressif_start(struct network_interface_wifi_softap_espressif_t *self_p)
{   
    struct inet_ip_addr_t addr;
    
    do {
        wifi_get_ip_info(SOFTAP_IF, &ip_config);
    } while (ip_config.ip.addr == 0);
    
    addr.number = ip_config.ip.addr;
    network_interface_set_ip_address(&self_p->network_interface,
                                     &addr);
    
    return (0);
}

int network_interface_wifi_softap_espressif_stop(struct network_interface_wifi_softap_espressif_t *self_p)
{
    return (-1);
}

int network_interface_wifi_softap_espressif_is_up(struct network_interface_wifi_softap_espressif_t *self_p)
{
    ASSERTN(self_p != NULL, -EINVAL);
    
    return (is_up(&self_p->network_interface));
}

#endif