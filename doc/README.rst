Documentation
=============

The documentation is generated to ``.html`` from ``.rst`` files using
`Sphinx`_.

To perform a complete documentation build which also regenerate the
automatically generated documentation, such as default per-board flash
and RAM size, you need the full Simba toolchain installed as
documented on the `Getting Started`_ page.

To perform such a build run:

.. code-block:: text

   cd .. && make doc && firefox doc/_build/html/index.html

To just build the existing documentation:

.. code-block:: text

   make && firefox doc/_build/html/index.html

To install the tools needed to just build the Simba documentation on
an Ubuntu Linux system, run the following:

.. code-block:: text

   sudo apt-get install doxygen python-sphinx python-sphinx-rtd-theme \
     python-breathe

.. _Sphinx: http://www.sphinx-doc.org
.. _Getting Started: http://simba-os.readthedocs.io/en/latest/getting-started.html#simba-simba-build-system
