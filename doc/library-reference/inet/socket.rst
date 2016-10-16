:mod:`socket` --- Internet communication
========================================

.. module:: socket
   :synopsis: Internet communication.

Sockets are used to communicate over IP networks. TCP and UDP are the
most common transport protocols.

No more than one thread may read from a socket at any given
moment. The same applies when writing to a socket. The reader and
writer may be different threads, though. The behaviour is undefined if
more threads use the same socket simultaneously. The application will
likely crash. Add a semaphore to protect the socket if more threads
need access to a socket.

Below is a TCP client example that connects to a server and sends
data.

.. code-block:: c

   uint8_t buf[16];
   struct socket_t tcp;
   struct inet_addr_t local_addr, remote_addr;

   /* Set the local and remote addresses. */   
   inet_aton("192.168.1.103", &local_addr.ip);
   local_addr.port = 6000;
   inet_aton("192.168.1.106", &remote_addr.ip);
   remote_addr.port = 5000;

   /* Initialize the socket and connect to the server. */
   socket_open_tcp(&tcp);
   socket_bind(&tcp, &local_addr);
   socket_connect(&tcp, &remote_addr);

   /* Send the data. */
   memset(buf, 0, sizeof(buf));
   socket_write(&tcp, buf, sizeof(buf));

   /* Close the connection. */
   socket_close(&tcp);

And below is the same scenario for UDP.

.. code-block:: c

   uint8_t buf[16];
   struct socket_t udp;
   struct socket_addr_t local_addr, remote_addr;

   /* Set the local and remote addresses. */   
   inet_aton("192.168.1.103", &local_addr.ip);
   local_addr.port = 6000;
   inet_aton("192.168.1.106", &remote_addr.ip);
   remote_addr.port = 5000;

   /* Initialize the socket and connect to the server. */
   socket_open_udp(&udp);
   socket_bind(&udp, &local_addr);
   socket_connect(&udp, &remote_addr);

   /* Send the data. */
   memset(buf, 0, sizeof(buf));
   socket_send(&udp, buf, sizeof(buf));

   /* Close the connection. */
   socket_close(&udp);

----------------------------------------------

Source code: :github-blob:`src/inet/socket.h`, :github-blob:`src/inet/socket.c`
 
----------------------------------------------

.. doxygenfile:: inet/socket.h
   :project: simba
