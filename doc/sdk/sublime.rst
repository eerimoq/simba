Sublime
=======

Installation
------------

Copy all files in the folder `sdk/sublime`_ to
``${SUBLIME_ROOT}/Packages/User``.

The Simba menu
--------------

A menu called Simba is added to the menubar in Sublime. It can create,
release and install applications and packages.

.. code-block:: text

   Simba
   ├── Application
   │   └── New
   └── Package
       ├── New
       ├── Release
       └── Install

Build and run
-------------

Build and run commands are listed with ``Ctrl-Shift-P``.

The available commands are:

- simba-native-{all,run,clean}
- simba-arduino-due-{release,run,clean}
- simba-arduino-mega-{release,run,clean}
- simba-arduino-nano-{release,run,clean}

.. _sdk/sublime: https://github.com/eerimoq/simba/tree/master/sdk/sublime
