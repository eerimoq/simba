Signal Analyzer
===============

A minimalistic signal analyzer.

PWM
---

Measure duty cycle and frequency on eight pins in parallel with the
file system command ``/pwm/measure [iterations]``.

The leftmost column is the number of samples used to calculate the
duty cycles and frequencies. The sample interval is set to 1 ms.

.. code-block:: text

   $ pwm/measure
   20: [(0,0),(0,0),(100,1),(0,0),(0,0),(15,30),(0,0),(0,0)]
   $ pwm/measure 10
   20: [(0,0),(0,0),(100,0),(0,0),(0,0),(29,30),(0,0),(0,0)]
   40: [(0,0),(0,0),(100,0),(0,0),(0,0),(29,30),(0,0),(0,0)]
   60: [(0,0),(0,0),(100,0),(0,0),(0,0),(31,40),(0,0),(0,0)]
   80: [(0,0),(0,0),(100,0),(0,0),(0,0),(33,30),(0,0),(0,0)]
   100: [(0,0),(0,0),(100,0),(0,0),(0,0),(41,30),(0,0),(0,0)]
   120: [(0,0),(0,0),(100,0),(0,0),(0,0),(41,30),(0,0),(0,0)]
   140: [(0,0),(0,0),(100,0),(0,0),(0,0),(41,30),(0,0),(0,0)]
   160: [(0,0),(0,0),(100,0),(0,0),(0,0),(40,30),(0,0),(0,0)]
   180: [(0,0),(0,0),(100,0),(0,0),(0,0),(39,30),(0,0),(0,0)]
   200: [(0,0),(0,0),(100,0),(0,0),(0,0),(39,30),(0,0),(0,0)]
   $

Tips: Use the Python ``ast`` module to convert the measurement string
to a Python dictionary.

.. code-block:: python

   >>> import ast
   >>> measurement = "20: [(0,0),(0,0),(100,1),(0,0),(0,0),(15,30),(0,0),(0,0)]"
   >>> ast.literal_eval('{' + measurement + '}')
   {20: [(0, 0), (0, 0), (100, 1), (0, 0), (0, 0), (15, 30), (0, 0), (0, 0)]}
   >>>
