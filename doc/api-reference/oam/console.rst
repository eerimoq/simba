:mod:`console` --- System console
=================================

.. module:: console
   :synopsis: Debug file system.

The system console is the default communication channel to an
application. The console input and output channels are often
terminated by a shell to enable the user to control and debug the
application.

Configure the console by changing the :doc:`configuration
 variables<../../user-guide/configuration>` called
 ``CONFIG_CONSOLE*``.

----------------------------------------------

Source code: :github-blob:`src/oam/console.h`

Test coverage: :codecov:`src/oam/console.c`

----------------------------------------------

.. doxygenfile:: oam/console.h
   :project: simba
