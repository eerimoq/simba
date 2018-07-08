{desc}
{desc_underline}

Pinout
------

.. image:: ../images/boards/{pinout}
   :width: 50%
   :target: ../_images/{pinout}

{include_extra}

Default system features
-----------------------

The default configuration includes those major features. They are all
initialized by ``sys_start()`` at the startup of the application.

{major_features}

Drivers
-------

Supported drivers for this board.

{drivers}

Library Reference
-----------------

Read more about board specific functionality in the :doc:`{desc}
<../library-reference/boards/{name}>` module documentation in the
Library Reference.

Memory usage
------------

Below is the memory usage of two applications:

- The
  :github-tree:`minimal-configuration<examples/minimal-configuration>`
  application is configured to only include the bare minimum of
  functionality for the low level kernel to run. That is, the
  thread scheduler and system tick.

- The
  :github-tree:`default-configuration<examples/default-configuration>`
  application is built with the default configuration, including a lot
  more functionality. See the list of `Default system features`_ above
  for a summary.

+--------------------------+-----------+-----------+
| Application              | Flash     | RAM       |
+==========================+===========+===========+
{memory_usage}
+--------------------------+-----------+-----------+

Default configuration
---------------------

The communication between the PC and the board is carried over
**{console_params[channel]}{console_params[settings]}**.

Default Standard Library configuration.

+--------------------------------------------------------+-----------------------------------------------------+
|  Name                                                  |  Value                                              |
+========================================================+=====================================================+
{default_configuration}

Homepage
--------

{homepage}

Mcu
---

:doc:`{mcu}<../library-reference/mcus/{mcu}>`

{targets}
