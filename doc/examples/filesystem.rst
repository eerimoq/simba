Filesystem
==========

About
-----

Create the file ``counter.txt`` and write ``0`` to it. Everytime the
application is restarted the counter is incremented by one.

Source code
-----------

.. include:: filesystem/source-code.rst

The source code can also be found on Github in the
:github-tree:`examples/filesystem` folder.

Build and run
-------------

Build and run the application.

.. code-block:: text

   $ cd examples/filesystem
   $ make -s BOARD=arduino_due upload

The output in the terminal emulator:

.. code-block:: text

    Incrementing the counter in 'counter.txt'.
    Counter incremented to 1.
    <manually reset the board>
    Incrementing the counter in 'counter.txt'.
    Counter incremented to 2.
    <manually reset the board>
    Incrementing the counter in 'counter.txt'.
    Counter incremented to 3.
