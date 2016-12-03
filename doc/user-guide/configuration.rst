Configuration
=============

Standard Library
----------------

The :doc:`../library-reference` is configured at compile time using
defines named ``CONFIG_*``. The default configuration includes most
functionality, as most application wants that. If an application has
special requirements, for example memory constraints, it has to be
configured to remove unnecessaray functionality.

Search order
^^^^^^^^^^^^

Highest priority first.

Simba build system
""""""""""""""""""

1. Command line as ``CDEFS_EXTRA="<configuration variable>=<value>"``.

2. A file named ``config.h`` in the application root folder.

3. The default configuration file, :github-blob:`src/config_default.h`.

PlatformIO
""""""""""

1. The variable ``build_flags`` in ``platformio.ini`` as
   ``build_flags = -D<configuration variable>=<value>``.

2. A file named ``config.h`` in the application source folder ``src``.

3. The default configuration file, :github-blob:`src/config_default.h`.

Arduino IDE
"""""""""""

1. A file (also called a `tab`) named ``config.h`` in the sketch.

2. The default configuration file, :github-blob:`src/config_default.h`.

Variables
^^^^^^^^^

All configuration variables are listed below. Their default values are
defined in :github-blob:`src/config_default.h`.

.. doxygenfile:: config_default.h
   :project: simba

lwIP
----

Use ``config.h`` to fully configure lwIP and all of its modules. You
do not need to define every option that lwIP provides; if you do not
define an option, a default value will be used. Therefore, your
``config.h`` provides a way to override much of the behavior of lwIP.

By default `Simba` overrides a few of the variables in
:github-blob:`src/inet/lwipopts.h`.

Module support (Code size)
^^^^^^^^^^^^^^^^^^^^^^^^^^

Enabling and disabling modules
""""""""""""""""""""""""""""""

You can tune your code size by only compiling the features you really
need. The following is a list of what gets compiled in "out of the
box" with lwIP.

Default inclusions:

- ARP (``LWIP_ARP``)
- IP and fragmentation (``IP_FRAG``) and reassembly (``IP_REASSEMBLY``)
- Raw IP PCB support (``LWIP_RAW``)
- UDP (``LWIP_UDP``) and UDP-Lite (``LWIP_UDPLITE``)
- TCP (``LWIP_TCP``) -- this is a big one!
- Statistics (``LWIP_STATS``)

Default exclusions:

- DHCP (``LWIP_DHCP``)
- AUTOIP (``LWIP_AUTOIP``)
- SNMP (``LWIP_SNMP``)
- IGMP (``LWIP_IGMP``)
- PPP (``PPP_SUPPORT``)

If you would like to change this, then you just need to set the
options listed below. For example, if you would like to disable UDP
and enable DHCP, the following ``config.h`` file would do it:

.. code:: c

   /* Disable UDP */
   #define LWIP_UDP 0

   /* Enable DHCP */
   #define LWIP_DHCP 1

Memory management (RAM usage)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Memory pools
""""""""""""

In an embedded environment, memory pools make for fast and efficient
memory allocation. lwIP provides a flexible way to manage memory pool
sizes and organization.

lwIP reserves a fixed-size static chunk of memory in the data segment,
which is subdivided into the various pools that lwip uses for the
various data structures. For example, there is a pool just for struct
tcp_pcb's, and another pool just for struct udp_pcb's. Each pool can
be configured to hold a fixed number of data structures; this number
can be changed in the ``config.h`` file by changing the various
``MEMP_NUM_*`` values. For example, ``MEMP_NUM_TCP_PCB`` and
``MEMP_NUM_UDP_PCB`` control the maximum number of tcp_pcb and udp_pcb
structures that can be active in the system at any given time.

It is also possible to create custom memory pools in addition to the
standard ones provided by lwIP.

Dynamic allocation: mem_malloc
""""""""""""""""""""""""""""""

lwIP uses a custom function mem_malloc for all dynamic allocation;
therefore, it is easy to change how lwIP uses its RAM. There are three
possibilities provided out-of-the-box:

1. (default) lwIP's custom heap-based mem_malloc. By default, lwIP
   uses a statically-allocated chunk of memory like a heap for all
   memory operations. Use ``MEM_SIZE`` to change the size of the lwIP
   heap.

2. C standard library malloc and free. If you wish to have lwIP use
   the standard library functions provided by your
   compiler/architecture, then define the option ``MEM_LIBC_MALLOC``.

3. Memory pools. lwIP can also emulate dynamic allocation using custom
   memory pools (see that chapter for more information). This involves
   the options ``MEM_USE_POOLS`` and ``MEMP_USE_CUSTOM_POOLS`` and a
   new custom file ``lwippools.h``.

Understanding/changing memory usage
"""""""""""""""""""""""""""""""""""

lwIP uses memory for:

- code (depending on your system, may use ROM instead of RAM)

- statically allocated variables (some initialized, some not
  initialized)

- task stack

- dynamically allocated memory

     - heap
     - memp pools

Unless you use a C library heap implementation (by defining
``MEM_LIBC_MALLOC`` to 1), dynamically allocated memory must be statically
allocated somewhere. This means you reserve a specific amount of
memory for the heap or the memp pools from which the code dynamically
allocates memory at runtime.

The size of this heap and memp pools can be adjusted to save RAM:

There are 3 types of pbufs:

- REF/ROM, RAM and POOL. ``PBUF_POOL_SIZE`` * ``PBUF_POOL_BUFSIZE``
  only refers to type POOL.

- RAM pbufs are allocated in the memory defined by ``MEM_SIZE`` (this
  memory is not used much aside from RAM pbufs) - this is the *heap*
  and it is allocated as mem_memory.

- REF/ROM pbufs as well as pcbs and some other stuff is allocated from
  dedicated pools per structure type. The amount of structures is
  defined by the various ``MEMP_NUM_`` defines. Together, this memory
  is allocated as memp_memory and it *includes* the pbuf POOL.

However, if you define ``MEMP_MEM_MALLOC`` to 1 in your ``config.h``,
*every* piece of dynamically allocated memory will come from the heap
(the size of which is defined by ``MEM_SIZE``). If you then even
define ``MEM_LIBC_MALLOC`` to 1, too, lwIP doesn't need extra memory
for dynamically allocated memory but only uses the C library heap
instead. However, you then have to make sure that this heap is big
enough to run your application.

To tweak the various ``MEMP_NUM_`` defines, define ``LWIP_STATS=1``
and ``LWIP_STATS_DISPLAY=1`` and call `stats_display()` to see how
many entries of each pool are used (or have a look at the global
variable ``lwip_stats`` instead).

Fine-tuning even more
^^^^^^^^^^^^^^^^^^^^^

To see the options that you can set, open
:github-blob:`3pp/lwip-1.4.1/src/include/lwip/opt.h`. This file is
fully commented and explains how many of the options are used.

.. |br| raw:: html

   <br />
