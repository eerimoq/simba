#!/usr/bin/env python

import glob
import shutil
import os


def main():
    paths = glob.glob('mbedtls/library/*.c')
    for path in paths:
        directory, filename = os.path.split(path)
        shutil.copy(path, os.path.join('compat', directory, 'mbedtls_' + filename))
    
if __name__ == "__main__":
    main()
