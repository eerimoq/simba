#!/usr/bin/env python2

import sys
import re
import serial

pc = serial.Serial(port='/dev/ttyUSB1', baudrate=9600)

input_buffer = ''

re_at_clac = re.compile(r"^at\+clac\r\n$", re.IGNORECASE)
re_at = re.compile(r"^at\r\n$", re.IGNORECASE)
re_at_reset = re.compile(r"^at\+reset\r\n$", re.IGNORECASE)
re_at_version = re.compile(r"^at\+version\?\r\n$", re.IGNORECASE)
re_at_name_set = re.compile(r"^at\+name=([^\r]+)\r\n$", re.IGNORECASE)
re_at_name_get = re.compile(r"^at\+name\?\r\n$", re.IGNORECASE)
re_at_pswd_set = re.compile(r"^at\+pswd=([^\r]+)\r\n$", re.IGNORECASE)
re_at_pswd_get = re.compile(r"^at\+pswd\?\r\n$", re.IGNORECASE)
re_at_uart_set = re.compile(r"^at\+uart=(\d+,\d+,\d+)\r\n$", re.IGNORECASE)
re_at_uart_get = re.compile(r"^at\+uart\?\r\n$", re.IGNORECASE)

# device parameters
VERSION = "2.0"
NAME = "HC-05"
PSWD = "1234"
UART = "9600,0,0"

def handle_at_command(input_buffer):
    global VERSION, NAME, PSWD, UART

    # CLAC
    mo = re_at_clac.match(input_buffer)
    if mo:
        pc.write("at\r\n"
                 "at+reset\r\n"
                 "at+version\r\n"
                 "at+name\r\n"
                 "at+pswd\r\n"
                 "at+uart\r\n"
                 "at+clac\r\n")
        return True

    # AT
    mo = re_at.match(input_buffer)
    if mo:
        return True

    # RESET
    mo = re_at_reset.match(input_buffer)
    if mo:
        return True

    #VERSION
    mo = re_at_version.match(input_buffer)
    if mo:
        pc.write("+VERSION:" + VERSION + "\r\n")
        return True

    # NAME
    mo = re_at_name_set.match(input_buffer)
    if mo:
        NAME = mo.group(1)
        return True

    mo = re_at_name_get.match(input_buffer)
    if mo:
        pc.write(NAME + "\r\n")
        return True

    # PSWD
    mo = re_at_pswd_set.match(input_buffer)
    if mo:
        PSWD = mo.group(1)
        return True

    mo = re_at_pswd_get.match(input_buffer)
    if mo:
        pc.write(PSWD + "\r\n")
        return True

    # UART
    mo = re_at_uart_set.match(input_buffer)
    if mo:
        UART = mo.group(1)
        return True

    mo = re_at_uart_get.match(input_buffer)
    if mo:
        pc.write(UART + "\r\n")
        return True

    return False


while True:
    data = pc.read()

    sys.stdout.write(data)
    sys.stdout.flush()

    # echo all read data
    pc.write(data)

    input_buffer += data

    if input_buffer.endswith("\r\n"):
        if handle_at_command(input_buffer) == True:
            pc.write("OK\r\n")
        else:
            pc.write("ERROR\r\n")
        input_buffer = ''
