Developer Guide
===============

File tree
---------

.. code-block:: text

    -- {root}                       - this directory
       +-- examples/                - example applications
       +-- make/                    - build and run files
       +-- README
       +-- src/                     - source code directory
       |   +-- boards/              - board configurations
       |   +-- drivers/             - drivers package source code
       |   +-- kernel/              - kernel package source code
       |   +-- slib/                - slib (Simba lib) package source code
       |   +-- mcus/                - mcu configurations
       |   +-- simba.h              - includes kernel, drivers and slib headers
       +-- tst/                     - test suites
           +-- drivers/             - drivers package test suites
           +-- kernel/              - kernel package test suites
           +-- slib/                - slib package test suites
