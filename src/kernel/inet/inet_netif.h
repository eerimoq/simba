/**
 * Interface implemented by network interfaces.
 * Required by ip stack to read and write packets.
 * The network interface must support simultanious
 * transmission and reception.
 */

#ifndef __KERNEL_INET_NETIF_H__
#define __KERNEL_INET_NETIF_H__

/**
 * Initialize network interface.
 * @param[in] cfg configuration
 */
void inet_netif_init(struct netifConfig *cfg);

/**
 * Output given packet.
 * @param[in] iovec zero terminated packet vector
 * @param[in] ioveclen io vector length
 * @param[in] daddr destination ip address
 * @return zero(0) or negative error code
 */
int inet_netif_output(struct iovec_t *iovec,
                      uint8_t ioveclen,
                      uint32_t daddr);

/**
 * Begin current packet.
 */
void inet_netif_input(void);

/**
 * Read bytes from current packet.
 * @param[in] buf buffer to read to
 * @param[in] size number of bytes to read
 * @return number of read bytes
 */
size_t inet_netif_read(void *buf_p, size_t size);

/**
 * Get network interfac eip address.
 * @return ip address
 */
uint32_t inet_netif_get_ip(void);

#endif
