simba
=====

The program `simba` is used to manage `Simba` packages and
applications.

The main purpose of `simba` is to distribute software in the `Simba`
community, just like `pip` for Python.

How to create an application skeletorn
--------------------------------------

The code block below shows how to create an new application using
`simba`. After the application has been created, it is built and
executed.

.. code-block:: text

   $ mkdir myapp
   $ cd myapp
   $ simba application init
   Application name [foo]: <Enter>
   Author [erik]:  <Enter>
   Version [0.3.0]: <Enter>
   $ tree .
   .
   ├── main.c
   └── Makefile
   $ make -s run

How to create a package
-----------------------

The code block below shows how to create a new package using
`simba`. After the package has been created, the generated test suite
is built and executed.

.. code-block:: text

   $ mkdir mypkg
   $ cd mypkg
   $ simba package init
   Package name [foo]: <Enter>
   Author [erik]:  <Enter>
   Version [0.3.0]: <Enter>
   $ tree
   .
   ├── mypkg
   │   ├── doc
   │   │   ├── about.rst
   │   │   ├── api-reference.rst
   │   │   ├── conf.py
   │   │   ├── doxygen.cfg
   │   │   ├── index.rst
   │   │   ├── Makefile
   │   │   ├── mypkg
   │   │   │   └── hello.rst
   │   │   ├── requirements.txt
   │   │   └── sphinx.mk
   │   ├── __init__.py
   │   ├── src
   │   │   ├── mypkg
   │   │   │   ├── hello.c
   │   │   │   └── hello.h
   │   │   ├── mypkg.h
   │   │   └── mypkg.mk
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
------------------------

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
------------------------

This is how to install a package in ``${SIMBA_ROOT}/dist-packages``.

.. code-block:: text

   $ simba package install dist/mypkg-0.1-py2.py3-none-any.whl
