:mod:`harness` --- Test harness
===============================

.. module:: harness
   :synopsis: Test harness.

In software testing, a test harness or automated test framework is a
collection of software and test data configured to test a program unit
by running it under varying conditions and monitoring its behavior and
outputs. It has two main parts: the test execution engine and the test
script repository.

This module implements the test execution engine.

The test scripts are part of the build system.

Example test suite
------------------

Below is an example of a test suite using the harness. It has three
test cases; ``test_passed``, ``test_failed`` and ``test_skipped``.

The test macro ``BTASSERT(condition)`` should be used to validate
conditions.

.. code-block:: c

   #include "simba.h"

   static int test_passed(struct harness_t *harness_p)
   {
       /* Return zero(0) when a test case passes. */
       return (0);
   }

   static int test_failed(struct harness_t *harness_p)
   {
       /* Return a negative integer when a test case fails. BTASSERT
          will return -1 when the condition is false. */
       BTASSERT(0);

       return (0);
   }

   static int test_skipped(struct harness_t *harness_p)
   {
       /* Return a positive integer when a test case is skipped. */
       return (1);
   }

   int main()
   {
       /* Test harness and NULL terminated list of test cases.*/
       struct harness_t harness;
       struct harness_testcase_t harness_testcases[] = {
           { test_passed, "test_passed" },
           { test_failed, "test_failed" },
           { test_skipped, "test_skipped" },
           { NULL, NULL }
       };

       sys_start();

       harness_init(&harness);
       harness_run(&harness, harness_testcases);

       return (0);
   }

The output from the test suite is:

.. code-block:: text

   app:    test_suite-7.0.0 built 2016-07-25 17:38 CEST by erik.
   board:  Linux
   mcu:    Linux

   enter: test_passed
   exit: test_passed: PASSED

   enter: test_failed
   exit: test_failed: FAILED

   enter: test_skipped
   exit: test_skipped: SKIPPED

               NAME        STATE  PRIO   CPU  LOGMASK
               main      current     0    0%     0x0f
                           ready   127    0%     0x0f
   harness report: total(3), passed(1), failed(1), skipped(1)

There are plenty of test suites in the :github-tree:`tst<tst>` folder
on Github.

---------------------------------------------------

Source code: :github-blob:`src/debug/harness.h`, :github-blob:`src/debug/harness.c`

---------------------------------------------------

.. doxygenfile:: debug/harness.h
   :project: simba
