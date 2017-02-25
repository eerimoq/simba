Documentation
=============

The documentation is generated to ``.html`` from ``.rst`` files using
`Sphinx<http://www.sphinx-doc.org>`.

.. code-block:: text

   cd .. && make doc && firefox doc/_build/html/index.html

To get the software required to build the documentation on Ubuntu
systems run:

.. code-block:: text

   sudo apt-get install doxygen python3-sphinx python3-sphinx-rtd-theme \
     python3-breathe
