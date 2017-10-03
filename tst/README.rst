Unit testing
============

This folder contains unit test suites of Simba modules.

Each module shall have unit tests to verify that the implementation
works as expected and that future refactoring does not break legacy.

The ``stubs`` folder does not contain test suites, but stubs of all
simba modules. Expected input and output data to/from each stubbed
function is controlled from the test suite by calling
``mock_write_*()``.
