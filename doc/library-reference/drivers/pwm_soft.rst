:mod:`pwm_soft` --- Software pulse width modulation
===================================================

.. module:: pwm_soft
   :synopsis: Software pulse width modulation.

This module implements software PWM on all digital pins. In general,
software PWM outputs an inaccurate, low frequency signal. Keep that in
mind designing your application.

If an accurate and/or high frequency PWM signal is required, a
:doc:`hardware PWM<pwm>` should be used instead.

Here is a short example of how to use this module. A software PWM
driver is initialized for digital pin 3 (D3). A software PWM signal
with duty cycle 10% is outputted on D3 after the calling
`pwm_soft_start()`.

.. code-block:: c

   struct pwm_soft_driver_t pwm_soft;

   pwm_soft_module_init(500);
   pwm_soft_init(&pwm_soft, &pin_d3_dev, pwm_soft_duty_cycle(10));
   pwm_soft_start(&pwm_soft);

Change the duty cycle to 85% by calling `pwm_soft_set_duty_cycle()`.

.. code-block:: c

   pwm_soft_set_duty_cycle(&pwm_soft, pwm_soft_duty_cycle(85));

Stop outputting the software PWM signal to D3 by calling
`pwm_soft_stop()`.

.. code-block:: c

   pwm_soft_stop(&pwm_soft);

----------------------------------------------

Source code: :github-blob:`src/drivers/pwm_soft.h`, :github-blob:`src/drivers/pwm_soft.c`

Test code: :github-blob:`tst/drivers/pwm_soft/main.c`

----------------------------------------------

.. doxygenfile:: drivers/pwm_soft.h
   :project: simba
