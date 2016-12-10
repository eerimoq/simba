:mod:`pwm_soft` --- Software pulse width modulation
===================================================

.. module:: pwm_soft
   :synopsis: Software pulse width modulation.

This module implements software PWM, often using a hardware timer and
pin devices. The properties of software PWM depends on the
application, and are often inaccurate. Use a :doc:`hardware PWM<pwm>`
if an accurate and/or high frequency PWM signal is required.

----------------------------------------------

Source code: :github-blob:`src/drivers/pwm_soft.h`, :github-blob:`src/drivers/pwm_soft.c`

----------------------------------------------

.. doxygenfile:: drivers/pwm_soft.h
   :project: simba
