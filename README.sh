#!/usr/bin/env bash

cat<<EOF > README.rst
|buildstatus|_

.. |buildstatus| image:: https://travis-ci.org/eerimoq/simba.svg
.. _buildstatus: https://travis-ci.org/eerimoq/simba

EOF

cat doc/about.rst \
    doc/installation.rst \
    doc/user-guide.rst \
    doc/developer-guide.rst >> README.rst
