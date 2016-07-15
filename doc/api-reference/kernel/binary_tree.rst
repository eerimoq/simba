:mod:`binary_tree` --- Binary tree
==================================

.. module:: binary_tree
   :synopsis: Binary tree.

A binary search tree consists of nodes, where each node has zero, one
or two siblings. The left sibling has a lower value and the right
sibling has a higher value than the parent.

Insert, delete and search operations all have the time complexity of
O(log n).

.. image:: ../../images/binary_tree.png
   :width: 40%
   :target: ../../_images/binary_tree.png

----------------------------------------------

Source code: :github-blob:`src/kernel/kernel/binary_tree.h`

Test code: :github-blob:`tst/kernel/binary_tree/main.c`

----------------------------------------------

.. doxygenfile:: kernel/binary_tree.h
   :project: simba
