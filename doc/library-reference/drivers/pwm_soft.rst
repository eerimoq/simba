:mod:`pwm_soft` --- Software pulse width modulation
===================================================

.. module:: pwm_soft
   :synopsis: Software pulse width modulation.

This module implements software PWM on all digital pins. In general,
software PWM outputs an inaccurate, low frequency signal. Keep that in
mind designing your application.

If an accurate and/or high frequency PWM signal is required, a
:doc:`hardware PWM<pwm>` should be used instead.

----------------------------------------------

Source code: :github-blob:`src/drivers/pwm_soft.h`, :github-blob:`src/drivers/pwm_soft.c`

----------------------------------------------

.. doxygenfile:: drivers/pwm_soft.h
   :project: simba
