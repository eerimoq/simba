:mod:`http_server` --- HTTP server
==================================

.. module:: http_server
   :synopsis: HTTP server.

A HTTP server serves HTTP client requests, typically from a web
browser.

A HTTP server can be wrapped in SSL, a secutiry layer, to create a
HTTPS server.

----------------------------------------------

Source code: :github-blob:`src/inet/http_server.h`, :github-blob:`src/inet/http_server.c`

Test code: :github-blob:`tst/inet/http_server/main.c`

Test coverage: :codecov:`src/inet/http_server.c`

Example code: :github-blob:`examples/http_server/main.c`, :github-blob:`examples/https_server/main.c`

----------------------------------------------

.. doxygenfile:: inet/http_server.h
   :project: simba
