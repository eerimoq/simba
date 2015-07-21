#!/usr/bin/env python
#
# Usage: python pp.py -i <infile> -o <outfile>
#
# Example: python pp.py -i main.c -o gen/main.pp.c
#

import argparse
import re

re_package = re.compile('^package\(([^\)]+)\);')
re_module = re.compile('^module\(([^\)]+)\);')
re_implements = re.compile('^implements\(([^\)]+)\);')
re_uses = re.compile('^uses\(([^\)]+)\);')

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description = 'Preprocess a source file.')
    parser.add_argument('--infile', help = 'infile')
    parser.add_argument('--outfile', help = 'outfile')
    args = parser.parse_args()

    with open(args.outfile, 'w') as fout:
        with open(args.infile, 'r') as fin:
            for line in fin:
                mo = re_package.match(line)
                if mo:
                    continue
                mo = re_module.match(line)
                if mo:
                    continue
                mo = re_implements.match(line)
                if mo:
                    fout.write('#include "%s.h"\n' % mo.group(1).replace('.','/'))
                    continue
                mo = re_uses.match(line)
                if mo:
                    fout.write('#include "%s.h"\n' % mo.group(1).replace('.','/'))
                    continue
                fout.write(line)
