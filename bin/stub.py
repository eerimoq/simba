#!/usr/bin/env python3

import sys
import os
import argparse
import subprocess
import re


__version__ = '0.1'


STUB_FMT = '''\
{header}

#include "simba.h"

{functions}
'''

STUB_WRITE_FUNCTION_FMT = '''\
{declaration}
{{
{body}\
    return (0);
}}
'''

STUB_FUNCTION_FMT = '''\
{declaration}
{{
{body}\
    return (res);
}}
'''

STUB_WRITE_ARGUMENT_OUTPUT_P_FMT = '''\
    harness_mock_write("{function_name}({name})",
                       {name},
                       {size});
'''

STUB_WRITE_ARGUMENT_FMT = '''\
    harness_mock_write("{function_name}({name})",
                       &{name},
                       {size});
'''

STUB_ARGUMENT_OUTPUT_P_FMT = '''\
    harness_mock_assert("{function_name}({name})",
                        {name});
'''

STUB_ARGUMENT_FMT = '''\
    harness_mock_assert("{function_name}({name})",
                        &{name});
'''

def get_pattern_symbols(pattern):
    try:
        symbols = pattern.split(':')[1]
    except IndexError:
        sys.exit("error: bad stub pattern '{}'".format(pattern))
    else:
        if not symbols:
            return []
        else:
            return symbols.split(',')


def do_patch(args):
    '''Repalce symbols in an object file with their stubbed version.

    '''

    # Find symbols to stub in given source file (if any).
    symbols = []

    for pattern in args.patterns:
        if pattern.startswith(args.sourcefile):
            symbols += get_pattern_symbols(pattern)

    if not symbols:
        return

    # Find all symbols in the object file.
    command = [args.crosscompile + 'readelf']
    command += ['-s']
    command += ['-W']
    command += [args.objectfile]

    readelf_output = subprocess.check_output(command)
    re_symbol = re.compile(r"\s+\d+: \w+\s+\d+\s+(NOTYPE|OBJECT|FUNC)"
                           "\s+\w+\s+\w+\s+\w+\s+(.+)")
    symbols_in_objectfile = []

    for line in readelf_output.decode('ascii').splitlines():
        mo = re_symbol.match(line)

        if mo:
            symbols_in_objectfile.append(mo.group(2).strip())

    # Redefine given symbols in the object file.
    command = [args.crosscompile + 'objcopy']

    for symbol in symbols:
        if symbol not in symbols_in_objectfile:
            sys.exit("error: cannot stub missing symbol {}".format(symbol))

        print("Stubbing symbol '{}' in '{}'.".format(symbol, args.sourcefile))

        command += [
            '--redefine-sym', '{symbol}=__stub_{symbol}'.format(symbol=symbol)
        ]

    command += [args.objectfile, args.objectfile]

    subprocess.check_call(command)


def generate_function_stub_write(return_type, name, args, comment):
    '''Generate a stub write definition of given function.

    '''

    body = []

    for arg in args:
        arg_name = arg.split(' ')[-1].strip('*')

        if arg == 'void':
            continue
        elif '*' in arg:
            size = 'sizeof(*{name})'.format(name=arg_name)
            body.append(STUB_WRITE_ARGUMENT_OUTPUT_P_FMT.format(
                function_name=name,
                name=arg_name,
                size=size))
        else:
            size = 'sizeof({name})'.format(name=arg_name)
            body.append(STUB_WRITE_ARGUMENT_FMT.format(
                function_name=name,
                name=arg_name,
                size=size))

    if len(body) > 0:
        body += ['']

    function_start = '{}stub_write_{}('.format(return_type, name)
    padding = ' ' * len(function_start)
    declaration = function_start + '{})'.format((',\n' + padding).join(args))

    return STUB_WRITE_FUNCTION_FMT.format(declaration=declaration,
                                          body='\n'.join(body))


def generate_function_stub(return_type, name, args, comment):
    '''Generate a stub definition of given function.

    '''

    body = []

    for arg in args:
        arg_name = arg.split(' ')[-1].strip('*')

        if arg == 'void':
            continue
        elif '*' in arg:
            size = 'sizeof(*{name})'.format(name=arg_name)
            body.append(STUB_ARGUMENT_OUTPUT_P_FMT.format(function_name=name,
                                                          name=arg_name,
                                                          size=size))
        else:
            size = 'sizeof({name})'.format(name=arg_name)
            body.append(STUB_ARGUMENT_FMT.format(function_name=name,
                                                 name=arg_name,
                                                 size=size))

    if len(body) > 0:
        body += ['']

    function_start = '{}STUB({})('.format(return_type, name)
    padding = ' ' * len(function_start)
    declaration = function_start + '{})'.format((',\n' + padding).join(args))

    return STUB_FUNCTION_FMT.format(declaration=declaration,
                                    body='\n'.join(body))


def generate_function_stubs(declaration, comment):
    '''Generate the stub definitions of given function.

    '''

    re_declaration = re.compile(r'([\w \r\n\*]+?)(\w+)\((.*?)\)',
                                re.MULTILINE | re.DOTALL)

    return_type, name, args = re_declaration.match(declaration).groups()
    args = [arg.strip() for arg in args.split(',')]

    stub = generate_function_stub_write(return_type,
                                        name,
                                        args,
                                        comment)
    stub += '\n'
    stub += generate_function_stub(return_type,
                                   name,
                                   args,
                                   comment)

    return stub


def do_generate(args):
    '''Generate stubs of given header files.

    '''

    re_function = re.compile(r'/\*\*(.+?)\*/([\w \r\n\*]+?\w+\(.*?\));',
                             re.MULTILINE | re.DOTALL)
    re_file_header = re.compile(r'(/\*\*(.+?)\*/)',
                                re.MULTILINE | re.DOTALL)

    for header in args.headers:
        stub_path = header[:-2] + '_mock.c'
        print("Generating stub '{}' from '{}'.".format(stub_path,
                                                       header))
        stub = []

        with open(header, 'r') as fin:
            header_contents = fin.read()

        functions = re_function.findall(header_contents)
        file_header = re_file_header.search(header_contents)

        for comment, declaration in functions:
            stub.append(generate_function_stubs(declaration.strip(),
                                                comment))

        with open(stub_path, 'w') as fout:
            fout.write(STUB_FMT.format(header=file_header.group(1),
                                       functions='\n'.join(stub)))


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('-d', '--debug', action='store_true')
    parser.add_argument('--version',
                        action='version',
                        version=__version__,
                        help='Print version information and exit.')

    # Workaround to make the subparser required in Python 3.
    subparsers = parser.add_subparsers(title='subcommands',
                                       dest='subcommand')
    subparsers.required = True

    patch_parser = subparsers.add_parser('patch')
    patch_parser.add_argument('crosscompile')
    patch_parser.add_argument('objectfile')
    patch_parser.add_argument('sourcefile')
    patch_parser.add_argument('patterns', nargs='+')
    patch_parser.set_defaults(func=do_patch)

    generate_parser = subparsers.add_parser('generate')
    generate_parser.add_argument('headers', nargs='+')
    generate_parser.set_defaults(func=do_generate)

    args = parser.parse_args()

    if args.debug:
        args.func(args)
    else:
        try:
            args.func(args)
        except BaseException as e:
            sys.exit(str(e))


if __name__ == '__main__':
    main()
