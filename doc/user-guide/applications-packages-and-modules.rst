Applications, packages and modules
==================================

`Simba` has three software components; the application, the package
and the module.

Application
-----------

An application is an executable consisting of zero or more packages.

An application file tree can either be created manually or by using
the tool :doc:`simba`.

.. code-block:: text

   myapp
   ├── main.c
   └── Makefile

Development workflow
~~~~~~~~~~~~~~~~~~~~

Build and run often! More to be added, hopefully.

Package
-------

A package is a container of modules.

A package file tree can either be created manually or by using the
tool :doc:`simba`.

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
   │   │   ├── mypkg
   │   │   │   ├── module1.c
   │   │   │   └── module1.h
   │   │   ├── mypkg.h           # package header file
   │   │   └── mypkg.mk          # package makefile
   │   └── tst                   # package test code
   │       └── module1
   │           ├── main.c
   │           └── Makefile
   └── setup.py

Development workflow
~~~~~~~~~~~~~~~~~~~~

The package development workflow is fairly straight forward. Suppose
we want to add a new module to the file tree above. Create
``src/mypkg/module2.h`` and ``src/mypkg/module2.c``, then include
``mypkg/module2.h`` in ``src/mypkg.h`` and add ``mypkg/module2.c`` to
the list of source files in ``src/mypkg.mk``. Create a test suite for
the module. It consists of the two files ``tst/module2/main.c`` and
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
   │   │   ├── mypkg
   │   │   │   ├── module1.c
   │   │   │   ├── module1.h
   │   │   │   ├── module2.c
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
~~~~~~~~~~

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
------

A module is normally a header and a source file.
