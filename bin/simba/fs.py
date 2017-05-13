#
# File system helper functions.
#

import re


def parse_kernel_thrd_list(output, cpu=True, scheduled=True, stack=True):
    """Parse the output of the file system command kernel/thrd/list and
    return the parsed data as a dictionary with thread names as keys.

    """

    pattern = r'(.+)\s+(?P<state>\w+)\s+(?P<prio>-?\d+)\s+'
    if cpu:
        pattern += r'(?P<cpu>[\d\.]+)%\s+'
    if scheduled:
        pattern += r'(?P<scheduled>\d+)\s+'
    if stack:
        pattern += r'(?P<max_stack_usage>\d+)\s*/\s*(?P<stack_max>\d+)\s+'
    pattern += r'(?P<logmask>0x[0-9a-f]{2})'
    
    re_thread = re.compile(pattern)

    threads = {}

    for line in output.splitlines():
        mo = re_thread.match(line)

        if mo:
            name = mo.group(1).strip()
            values = {}
            
            for key, value in mo.groupdict().items():
                if key in ['prio', 'scheduled', 'max_stack_usage',
                           'stack_max', 'logmask']:
                    values[key] = int(value, 0)
                elif key in ['cpu']:
                    values[key] = float(value)
                elif key in ['state']:
                    values[key] = value.strip()
                
            threads[name] = values

    return threads


def parse_kernel_monitor(output):
    """Parse the thread monitor output and return the parsed data as a
    dictionary with thread names as keys.

    """

    re_thread = re.compile(r'(.*)\s+([\d\.]+)')

    threads = {}

    for line in output.splitlines():
        mo = re_thread.match(line)

        if mo:
            name = mo.group(1).strip()
            threads[name] = float(mo.group(2))

    return threads

