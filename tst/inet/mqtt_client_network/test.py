#!/usr/bin/env python

from __future__ import print_function

import sys
import time
import subprocess

PORT = 10000


def test_mosquitto(server_ip_address,
                   dev,
                   timeout):
    """Perform the Mosquitto test.

    """

    sp = subprocess.Popen(['mosquitto', '-v', '-p', str(PORT)],
                          stdout=subprocess.PIPE,
                          stderr=subprocess.PIPE)
    
    # Wait the test to end.
    dev.expect(r"harness report", timeout=timeout)

    sp.terminate()
