#!/usr/bin/env python3

import argparse
import zlib
import struct
from collections import namedtuple


Header = namedtuple('Header', ['crc', 'revision', 'magic'])


MAGIC = 0xa5c3


def calc_chunk_addresses(blocks, chunk_size):
    '''Return a list of all chunk start addresses.

    '''

    chunk_addresses = []

    for block in blocks:
        # Find block information.
        address, size = block.split(',')
        address = int(address, 0)
        size = int(size, 0)
        number_of_chunks_in_block = (size // chunk_size)

        for chunk_index in range(number_of_chunks_in_block):
            chunk_addresses.append(address + chunk_index * chunk_size)

    return chunk_addresses


def decode_chunk_header(binary):
    return Header(*struct.unpack('>IHH', binary))


def is_valid_chunk(header, data):
    crc = zlib.crc32(data) & 0xffffffff

    if header.magic != MAGIC:
        print('Chunk magic number mismatch.')
        return False

    if header.crc != crc:
        print('Chunk CRC mismatch.')
        return False

    return True


def decode_chunk(iostream, chunk_address, chunk_size):
    '''Decode given chunk.

    '''

    iostream.seek(chunk_address)
    header = decode_chunk_header(iostream.read(8))
    data = iostream.read(chunk_size - 8)

    if is_valid_chunk(header, data):
        return header.revision, data


def is_later_revision(new, old):
    if old is None:
        return True
    elif new > old:
        return ((new - old) < 0x8000)
    else:
        return not ((old - new) < 0x8000)


def do_decode(args):
    '''Decode subcommand.

    '''

    chunk_size = int(args.chunk_size, 0)
    chunk_addresses = calc_chunk_addresses(args.block,
                                           chunk_size)
    decoded_chunks = []

    with open(args.binfile, 'rb') as fin:
        for chunk_address in chunk_addresses:
            decoded_chunk = decode_chunk(fin, chunk_address, chunk_size)

            if decoded_chunk is None:
                print('Failed to decode chunk at address 0x{:08x}.'.format(
                    chunk_address))
            else:
                decoded_chunks.append(decoded_chunk)

    latest_chunk_revision = None
    latest_chunk_data = None

    for revision, data in decoded_chunks:
        if is_later_revision(revision, latest_chunk_revision):
            latest_chunk_revision = revision
            latest_chunk_data = data

    print("Writing chunk with revision {} to '{}'.".format(
        latest_chunk_revision,
        args.output_file))

    with open(args.output_file, 'wb') as fout:
        fout.write(latest_chunk_data)


def main():
    parser = argparse.ArgumentParser()

    subparsers = parser.add_subparsers()

    decode_parser = subparsers.add_parser('decode')
    decode_parser.add_argument("--block", action="append")
    decode_parser.add_argument("--chunk-size")
    decode_parser.add_argument("--endianess")
    decode_parser.add_argument("--output-file")
    decode_parser.add_argument("binfile")
    decode_parser.set_defaults(func=do_decode)

    args = parser.parse_args()

    args.func(args)


if __name__ == "__main__":
    main()
