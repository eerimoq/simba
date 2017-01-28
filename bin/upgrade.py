#!/usr/bin/env python
#
# Append the upgrade binary header to given binart file.
#

import argparse
import hashlib
import struct
import zlib


def create_header(binary, description):
    """Create the upgrade binary header for given binary data.

   SIZE       TYPE  DESCRIPTION
      4   uint32_t  header version (1)
      4   uint32_t  header size in bytes
      4   uint32_t  data size in bytes
     20  uint8_t[]  SHA1 of the data
     1+   c-string  data description
      4   uint32_t  CRC32 of the header (not including this field)
     0+  uint8_t[]  data

    """

    description += '\0'

    if len(description) % 4 != 0:
        description += (4 - (len(description) % 4)) * '\0'

    header = struct.pack('>III',
                         1,
                         36 + len(description),
                         len(binary))
    header += hashlib.sha1(binary).digest()
    header += description
    header += struct.pack('>I', zlib.crc32(header) & 0xffffffff)

    return header

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('-o', '--output')
    parser.add_argument('-d', '--description', default="")
    parser.add_argument('binary')
    args = parser.parse_args()

    with open(args.binary) as fin:
        binary = fin.read()

    header = create_header(binary, args.description)

    with open(args.output, 'wb') as fout:
        fout.write(header)
        fout.write(binary)



if __name__ == "__main__":
    main()
