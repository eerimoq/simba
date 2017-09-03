#!/usr/bin/env python3

import sys
import os
import argparse
import subprocess
import re


__version__ = '0.1'


STUB_H_FMT = '''\
{header}

#ifndef __{header_name}_MOCK_H__
#define __{header_name}_MOCK_H__

#include "simba.h"

{functions}
#endif
'''


STUB_C_FMT = '''\
{header}

#include "simba.h"
#include "{header_name}_mock.h"

{functions}\
'''


MOCK_WRITE_FUNCTION_FMT = '''\
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

MOCK_WRITE_ARGUMENT_IN_P_FMT = '''\
    harness_mock_write("{function_name}({name})",
                       {name},
                       {size});
'''

MOCK_WRITE_ARGUMENT_OUT_P_FMT = '''\
    harness_mock_write("{function_name}(): return ({name})",
                       {name},
                       {size});
'''

MOCK_WRITE_ARGUMENT_IN_FMT = '''\
    harness_mock_write("{function_name}({name})",
                       &{name},
                       {size});
'''

MOCK_WRITE_ARGUMENT_OUT_FMT = '''\
    harness_mock_write("{function_name}(): return ({name})",
                       &{name},
                       {size});
'''

MOCK_ARGUMENT_IN_P_FMT = '''\
    harness_mock_assert("{function_name}({name})",
                        {name});
'''

MOCK_ARGUMENT_OUT_P_FMT = '''\
    harness_mock_read("{function_name}(): return ({name})",
                      {name},
                      size);
'''

MOCK_ARGUMENT_IN_FMT = '''\
    harness_mock_assert("{function_name}({name})",
                        &{name});
'''

MOCK_ARGUMENT_OUT_FMT = '''\
    harness_mock_read("{function_name}(): return ({name})",
                      &{name},
                      sizeof({name}));
'''


class Argument(object):

    def __init__(self, name, type_, direction):
        self.name = name
        self.type_ = type_
        self.direction = direction

    def __repr__(self):
        return str(vars(self))

    def __str__(self):
        return '{}{}'.format(self.type_, self.name)


def parse_args(args, comment):
    if args == ['void']:
        return []

    arguments = []

    for arg in args:
        type_, name = re.match(r'(.+[^\w])(\w+)', arg).groups()
        mo = re.search(r'\[(\w+)\]\s+{}\s'.format(name),
                       comment,
                       re.DOTALL)

        if mo:
            direction = mo.group(1).split(',')
        else:
            direction = []

        arguments.append(Argument(name, type_, direction))

    return arguments


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


def generate_function_mock_write(return_type,
                                 name,
                                 arguments):
    '''Generate a mock write definition of given function.

    '''

    body = []

    for argument in arguments:
        if '*' in argument.type_:
            if 'char' in argument.type_:
                size = 'strlen({name}) + 1'.format(name=argument.name)
            elif 'void' in argument.type_:
                size = 'size'
            else:
                size = 'sizeof(*{name})'.format(name=argument.name)

            if 'in' in argument.direction:
                body.append(MOCK_WRITE_ARGUMENT_IN_P_FMT.format(
                    function_name=name,
                    name=argument.name,
                    size=size))
            else:
                body.append(MOCK_WRITE_ARGUMENT_OUT_P_FMT.format(
                    function_name=name,
                    name=argument.name,
                    size=size))
        else:
            size = 'sizeof({name})'.format(name=argument.name)
            body.append(MOCK_WRITE_ARGUMENT_IN_FMT.format(
                function_name=name,
                name=argument.name,
                size=size))

    if return_type == 'void':
        return_argument = []
    else:
        body.append(MOCK_WRITE_ARGUMENT_OUT_FMT.format(
            function_name=name,
            name='res',
            size='sizeof(res)'))
        return_argument = [Argument('res', return_type, [])]

    if len(body) > 0:
        body.append('')

    function_open = 'int mock_write_{}('.format(name)
    padding = ' ' * len(function_open)
    declaration = function_open
    declaration += (',\n' + padding).join([str(argument)
                                           for argument in arguments + return_argument])
    declaration += ')'

    mock_h = declaration + ';\n'
    mock_c = MOCK_WRITE_FUNCTION_FMT.format(declaration=declaration,
                                            body='\n'.join(body))

    return mock_h, mock_c


def generate_function_stub(return_type,
                           name,
                           arguments):
    '''Generate a stub definition of given function.

    '''

    body = []

    if return_type == 'void':
        return_argument = []
    else:
        body.append('    {}res;\n'.format(return_type))
        body.append(MOCK_ARGUMENT_OUT_FMT.format(
            function_name=name,
            name='res',
            size='sizeof(res)'))
        return_argument = [Argument('res', return_type, [])]

    for argument in arguments:
        if '*' in argument.type_:
            if 'in' in argument.direction:
                size = 'sizeof(*{name})'.format(name=argument.name)
                body.append(MOCK_ARGUMENT_IN_P_FMT.format(function_name=name,
                                                          name=argument.name,
                                                          size=size))
            else:
                size = 'sizeof(*{name})'.format(name=argument.name)
                body.append(MOCK_ARGUMENT_OUT_P_FMT.format(function_name=name,
                                                           name=argument.name,
                                                           size=size))
        else:
            size = 'sizeof({name})'.format(name=argument.name)
            body.append(MOCK_ARGUMENT_IN_FMT.format(function_name=name,
                                                 name=argument.name,
                                                 size=size))

    if len(body) > 0:
        body += ['']

    function_open = '{}__attribute__ ((weak)) STUB({})('.format(return_type, name)
    padding = ' ' * len(function_open)
    declaration = function_open
    declaration += (',\n' + padding).join([str(argument)
                                           for argument in arguments])
    declaration += ')'

    return STUB_FUNCTION_FMT.format(declaration=declaration,
                                    body='\n'.join(body))


def generate_function_stubs(declaration, comment):
    '''Generate the stub definitions of given function.

    '''

    re_declaration = re.compile(r'([\w \r\n\*]+?)(\w+)\((.*?)\)',
                                re.MULTILINE | re.DOTALL)

    return_type, name, args = re_declaration.match(declaration).groups()
    args = [arg.strip() for arg in args.split(',')]
    arguments = parse_args(args, comment)

    stub_h, stub_c = generate_function_mock_write(return_type,
                                                  name,
                                                  arguments)
    stub_c += '\n'
    stub_c += generate_function_stub(return_type,
                                     name,
                                     arguments)

    return stub_h, stub_c


def do_generate(args):
    '''Generate stubs of given header files.

    '''

    re_function = re.compile(r'/\*\*(.+?)\*/([\w \r\n\*]+?\w+\(.*?\));',
                             re.MULTILINE | re.DOTALL)
    re_file_header = re.compile(r'(/\*\*(.+?)\*/)',
                                re.MULTILINE | re.DOTALL)

    for header in args.headers:
        header_name = os.path.split(header)[1][:-2]
        mock_h_path = header_name + '_mock.h'
        mock_c_path = header_name + '_mock.c'
        print("Generating '{}' and '{}' from '{}'.".format(
            mock_h_path,
            mock_c_path,
            header))

        with open(header, 'r') as fin:
            header_contents = fin.read()

        functions = re_function.findall(header_contents)
        file_header = re_file_header.search(header_contents)

        mock_h = []
        mock_c = []

        for comment, declaration in functions:
            func_h, func_c = generate_function_stubs(declaration.strip(),
                                                     comment)
            mock_h.append(func_h)
            mock_c.append(func_c)

        with open(mock_h_path, 'w') as fout:
            fout.write(STUB_H_FMT.format(header=file_header.group(1),
                                         header_name=header_name.upper(),
                                         functions='\n'.join(mock_h)))

        with open(mock_c_path, 'w') as fout:
            fout.write(STUB_C_FMT.format(header=file_header.group(1),
                                         header_name=header_name,
                                         functions='\n'.join(mock_c)))


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
