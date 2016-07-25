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
test cases; test_passed, test_failed and test_skipped.

The test macro ``BTASSERT(condition)`` should be used to validate
conditions.

.. code:: c

   #include "simba.h"

   int test_passed(struct harness_t *harness_p)
   {
       return (0);
   }

   int test_failed(struct harness_t *harness_p)
   {
       return (-1);
   }

   int test_skipped(struct harness_t *harness_p)
   {
       return (1);
   }

   int main()
   {
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

There are plenty of test suites in the :github-tree:`tst<tst>` folder
on Github.

---------------------------------------------------

Source code: :github-blob:`src/debug/harness.h`

---------------------------------------------------

.. doxygenfile:: debug/harness.h
   :project: simba
