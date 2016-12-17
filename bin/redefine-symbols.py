#!/usr/bin/env python
#

import sys
import os
import argparse
import re
import subprocess


def compile_re_redefine(symbols):
    return re.compile(r'([^\w"/])(' + r"|".join(symbols) + r')([^\w/"])',
                      flags=re.DOTALL)


def redefine_source(infile, outfile, args):
    """Perform the redefinition in source files

    """

    print infile

    symbols = []

    for symbol in args.symbol:
        symbols.append(symbol.split("=")[0])

    for filename in args.symbols:
        with open(filename) as f:
            for symbol in f.readlines():
                symbols.append(symbol.strip().split(" ")[0])

    re_redefine = compile_re_redefine(symbols)

    with open(infile) as fin:
        original = fin.read()

    redefined, number_of_replacements = re_redefine.subn(r"\1esp_\2\3",
                                                         original)

    with open(outfile, "w") as fout:
        fout.write(redefined)

    print "Number of replacements:", number_of_replacements

def redefine_object(infile, outfile, args):
    """Preform the redefinition in object files

    """

    command = [args.toolchain_prefix + "objcopy"]
    for symbol in args.symbol:
        command += ["--redefine-sym", symbol]
    for symbols in args.symbols:
        command += ["--redefine-syms", symbols]
    command += [infile]
    command += [outfile]

    print ' '.join(command)
    subprocess.check_call(command)


def main(args):
    """Prefix given symbols in given source and object files.

    """

    if args.outfile:
        if len(args.infiles) > 1:
            sys.stderr.write("Only one input file allowed when "
                             "--outfile is given.\n")
            sys.exit(1)

    for infile in args.infiles:
        if args.outfile:
            outfile = args.outfile
        else:
            outfile = infile
        suffix = os.path.splitext(infile)[1]
        filetype = suffix[1:]
        if filetype in ["h", "c", "hpp", "cpp", "i"]:
            redefine_source(infile, outfile, args)
        else:
            redefine_object(infile, outfile, args)

if __name__ == "__main__":
    desc=("Redefine symbols in given file. Regular expression substitution "
          "is used for source files and objcopy is used for ELF files.")
    parser = argparse.ArgumentParser(description=desc)

    help = ('Change the name of a symbol old, to new. This can be useful '
            'when there are name collisions. This option may be given '
            'more than once.')
    parser.add_argument("--symbol",
                        action="append",
                        metavar="OLD=NEW",
                        default=[],
                        help=help)

    help = ('Apply --symbol to each symbol pair "old new" listed '
            'in the file filename. filename is simply a flat file, with '
            'one symbol pair per line. Line comments may be introduced '
            'by the hash character. This option may be given more than '
            'once.')
    parser.add_argument("--symbols",
                        action="append",
                        metavar="FILE",
                        default=[],
                        help=help)

    help = ('Toolchain prefix used as prefix to objcopy. Often specified '
            'as CROSS_COMPILE in build systems. For example "avr-" or '
            '"xtensa-lx106-elf-".')
    parser.add_argument("--toolchain-prefix",
                        default="",
                        help=help)

    parser.add_argument("--outfile",
                        metavar="FILE",
                        help=("Output source or object file. May only be "
                              "used if one input file is given."))

    parser.add_argument("infiles",
                        nargs="+",
                        metavar="FILE",
                        help="Input source or object files.")

    args = parser.parse_args()
    main(args)
