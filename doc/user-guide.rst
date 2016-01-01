User Guide
==========

This guide is intended for users of the Simba OS.

The Simba installation guide can be found on the :doc:`installation`
page.

**Contents:**

.. contents::
   :local:

Environment setup
-----------------

The first step is always to setup the `Simba` environment. It's a
simple matter of sourcing a setup-script in the simba root folder.

.. code-block:: text

   $ cd simba/simba
   $ source setup.sh

Hello World application
-----------------------

Let's start with the `Simba` "Hello World" application. It examplifies
what an application is and how to build and run it.

It consists of two files; ``main.c`` and ``Makefile``.

main.c
~~~~~~

`hello_world/main.c`_ defines the application entry function
``main()``.

.. code-block:: c

   #include "simba.h"

   int main()
   {
       sys_start();

       std_printf(FSTR("Hello world!\n"));

       return (0);
   }

Makefile
~~~~~~~~

`hello_world/Makefile`_ contains build configuration of the
application.

.. code-block:: makefile

   NAME = hello_world
   BOARD ?= linux

   RUN_END_PATTERN = "Hello world!"
   RUN_END_PATTERN_SUCCESS = "Hello world!"

   SIMBA_ROOT = ../..
   include $(SIMBA_ROOT)/make/app.mk

Build and run
~~~~~~~~~~~~~

Compile, link and run it by typing the commands below in a shell:

.. code-block:: text

   $ cd examples/hello_world
   $ make -s clean run
   <build system output>
   Hello world!
   $

Cross-compile, link and then run on an Arduino Due:

.. code-block:: text

   $ cd examples/hello_world
   $ make -s BOARD=arduino_due clean run
   <build system output>
   Hello world!
   $

Applications, packages and modules
----------------------------------

`Simba` has three software components; the application, the package
and the module.

Application
~~~~~~~~~~~

An application is an executable consisting of zero or more packages.

An application file tree can either be created manually or by using
the tool `simba`_. See `How to create an application`_ for a guide of
how to create an application using `simba`_.

.. code-block:: text

   myapp
   ├── main.c
   └── Makefile

Development workflow
^^^^^^^^^^^^^^^^^^^^

Build and run often! More to be added, hopefully.

Package
~~~~~~~

A package is a container of modules.

A package file tree can either be created manually or by using the
tool `simba`_. See `How to create a package`_ for a guide of how to
create a package using `simba`_.

A package file tree **must** be organized as seen below. This is
required by the build framework and `Simba` tools.

See the inline comments for details about the files and folders
contents.

.. code-block:: text

   mypkg
   ├── mypkg
   │   ├── doc                   # package documentation
   │   ├── __init__.py
   │   ├── src                   # package source code
   │   │   ├── module1.c
   │   │   ├── mypkg             # module header files
   │   │   │   └── module1.h
   │   │   ├── mypkg.h           # package header file
   │   │   └── mypkg.mk          # package makefile
   │   └── tst                   # package test code
   │       └── module1
   │           ├── main.c
   │           └── Makefile
   └── setup.py

Development workflow
^^^^^^^^^^^^^^^^^^^^

The package development workflow is fairly straight forward. Suppose
we want to add a new module to the file tree above. Create
``src/mypkg/module2.h`` and ``src/module2.c``, then include
``mypkg/module2.h`` in ``src/mypkg.h`` and add ``module2.c`` to the
list of source files in ``src/mypkg.mk``. Create a test suite for the
module. It consists of the two files ``tst/module2/main.c`` and
``tst/module2/Makefile``.

It's often conveniant to use an existing modules' files as skeleton
for the new module.

After adding the module ``module2`` the file tree looks like this.

.. code-block:: text

   mypkg
   ├── mypkg
   │   ├── doc
   │   ├── __init__.py
   │   ├── src
   │   │   ├── module1.c
   │   │   ├── module2.c
   │   │   ├── mypkg
   │   │   │   ├── module1.h
   │   │   │   └── module2.h
   │   │   ├── mypkg.h
   │   │   └── mypkg.mk
   │   └── tst
   │       ├── module1
   │       │   ├── main.c
   │       │   └── Makefile
   │       └── module2
   │           ├── main.c
   │           └── Makefile
   └── setup.py

Now, build and run the test suite to make sure the empty module
implementation compiles and can be executed.

.. code-block:: text

   $ cd tst/module2
   $ make -s run

Often the module development is started by implementing the module
header file and at the same time write test cases. Test cases are not
only useful to make sure the implementation works, but also to see how
the module is intended to be used. The module interface becomes
cleaner and easier to use it you actually start to use it yourself by
writing test cases! All users of your module will benefit from this!

So, now we have an interface and a test suite. It's time to start the
implementation of the module. Usually you write some code, then run
the test suite, then fix the code, then run the tests again, then you
realize the interface is bad, change it, change the implementation,
change the test, change, change... and so it goes on until you are
satisfied with the module.

Try to update the comments and documentation during the development
process so you don't have to do it all in the end. It's actually quite
useful for yourself to have comments. You know, you forget how to use
your module too!

The documentation generation framework uses doxygen, breathe and
sphinx. That means, all comments in the source code should be written
for doxygen. Breathe takes the doxygen output as input and creates
input for sphinx. Sphinx then generates the html documentation.

Just run ``make`` in the ``doc`` folder to generate the html
documentation.

.. code-block:: text

   $ cd doc
   $ make
   $ firefox _build/html/index.html    # open the docs in firefox

Namespaces
^^^^^^^^^^

All exported symbols in a package must have the prefix
``<package>_<module>_``. This is needed to avoid namespace clashes
between modules with the same name in different packages.

There cannot be two packages with the same name, for the namespace
reason. All packages must have unique names! There is one exception
though, the three `Simba` packages; kernel, drivers and slib. Those
packages does *not* have the package name as prefix on exported
symbols.

.. code-block:: c

    int mypackage_module1_foo(void);

    int mypackage_module2_bar(void);

Module
~~~~~~

A module is normally a header and a source file.

simba
-----

The program `simba` is used to manage `Simba` packages and
applications.

The main purpose of `simba` is to distribute software in the `Simba`
community, just like `pip` for Python.

How to create an application skeletorn
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The code block below shows how to create an new application using
`simba`. After the application has been created, it is built and
executed.

.. code-block:: text

   $ mkdir myapp
   $ cd myapp
   $ simba application init
   Application name [foo]: <Enter>
   Author [erik]:  <Enter>
   Version [0.1.0]: <Enter>
   $ tree .
   .
   ├── main.c
   └── Makefile
   $ make -s run

How to create a package
~~~~~~~~~~~~~~~~~~~~~~~

The code block below shows how to create a new package using
`simba`. After the package has been created, the generated test suite
is built and executed.

.. code-block:: text

   $ mkdir mypkg
   $ cd mkpkg
   $ simba package init
   Package name [foo]: <Enter>
   Author [erik]:  <Enter>
   Version [0.1.0]: <Enter>
   $ tree
   .
   ├── mkpkg
   │   ├── doc
   │   │   ├── about.rst
   │   │   ├── api-reference.rst
   │   │   ├── conf.py
   │   │   ├── doxygen.cfg
   │   │   ├── index.rst
   │   │   ├── Makefile
   │   │   ├── mkpkg
   │   │   │   └── hello.rst
   │   │   ├── requirements.txt
   │   │   └── sphinx.mk
   │   ├── __init__.py
   │   ├── src
   │   │   ├── hello.c
   │   │   ├── mkpkg
   │   │   │   └── hello.h
   │   │   ├── mkpkg.h
   │   │   └── mkpkg.mk
   │   └── tst
   │       └── hello
   │           ├── main.c
   │           └── Makefile
   └── setup.py
   $ cd mypkg/tst/hello
   $ make -s test

In the output from ``tree`` below, two files may catch your
eyes; setup.py and __init__.py. Those are Python files and are often
seen in Python packages. They are present in a `Simba` package because
`Simba` uses the Python tool `pip` to release and install
packages. The idea is that everyone that implements a useful package
should release it and make it available for other users to install,
just as Python!

How to release a package
~~~~~~~~~~~~~~~~~~~~~~~~

This is how to release a package. Two files are created, one with the
suffix ``tar.gz`` and one with the suffix ``.whl``. The ``.whl``-file
is input to the installation command, described in the next section.

.. code-block:: text

   $ cd ../../..
   $ simba package release
   $ tree dist
   dist
   ├── mypkg-0.1-py2.py3-none-any.whl
   └── mypkg-0.1.tar.gz

How to install a package
~~~~~~~~~~~~~~~~~~~~~~~~

This is how to install a package in ``${SIMBA_ROOT}/dist-packages``.

.. code-block:: text

   $ simba package install dist/mypkg-0.1-py2.py3-none-any.whl

Make variables
--------------

BOARD
~~~~~

``BOARD`` is used to select which board to build for. It can be
assigned to one of the boards listed :doc:`here <boards>`. For
example, the command to build for :doc:`Arduino Due
<boards/arduino_due>` is ``make BOARD=arduino_due release``.

Further reading
---------------

Please have a look at the :doc:`api-reference` for more juicy details
about the functionality that the `Simba` microkernel provides.

.. _hello_world/main.c: https://github.com/eerimoq/simba/tree/master/examples/hello_world/main.c
.. _hello_world/Makefile: https://github.com/eerimoq/simba/tree/master/examples/hello_world/Makefile
