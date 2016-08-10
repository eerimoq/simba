:mod:`console` --- System console
=================================

.. module:: console
   :synopsis: System console.

The system console is the default communication channel to an
application. The console input and output channels are often
terminated by a shell to enable the user to control and debug the
application.

Configure the console by changing the :doc:`configuration
variables<../../user-guide/configuration>` called ``CONFIG_START_CONSOLE*``.

----------------------------------------------

Source code: :github-blob:`src/oam/console.h`, :github-blob:`src/oam/console.c`

Test coverage: :codecov:`src/oam/console.c`

----------------------------------------------

.. doxygenfile:: oam/console.h
   :project: simba
