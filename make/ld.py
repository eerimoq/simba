#!/usr/bin/env python
#
# Usage: python ld.py <modules>
#
# Example: python ld.py -f main.c ../../kernel/sys.c
#

import argparse
import re

re_package = re.compile('^package\([^\)]+\);')
re_module = re.compile('^module\([^\)]+\);')
re_implements = re.compile('^implements\([^\)]+\);')
re_uses = re.compile('^uses\([^\)]+\);')

if __name__ == '__main__':
    
    # read forced c source files
    
    # read non-forced c source files
