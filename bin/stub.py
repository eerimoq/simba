#!/usr/bin/env python3

import sys
import os
import argparse
import subprocess
import re


__version__ = '0.2'


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
}}
'''

MOCK_WRITE_ARGUMENT_IN_PTR_FMT = '''\
    harness_mock_write("{function_name}({name})",
                       {name},
                       {size});
'''

MOCK_WRITE_ARGUMENT_OUT_PTR_FMT = '''\
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

MOCK_WRITE_ARGUMENT_VOID_FMT = '''\
    harness_mock_write("{function_name}()",
                       NULL,
                       0);
'''

MOCK_ARGUMENT_IN_PTR_FMT = '''\
    harness_mock_assert("{function_name}({name})",
                        {name});
'''

MOCK_ARGUMENT_OUT_PTR_FMT = '''\
    harness_mock_read("{function_name}(): return ({name})",
                      {name},
                      -1);
'''

MOCK_ARGUMENT_IN_FMT = '''\
    harness_mock_assert("{function_name}({name})",
                        &{name});
'''

MOCK_ARGUMENT_VOID_FMT = '''\
    harness_mock_assert("{function_name}()",
                        NULL);
'''

MOCK_ARGUMENT_OUT_FMT = '''\
    harness_mock_read("{function_name}(): return ({name})",
                      &{name},
                      sizeof({name}));
'''


class Argument(object):
    '''A function argument.

    '''

    def __init__(self, name, type_, direction):
        self.name = name
        self.type_ = type_
        self.direction = direction

    def __repr__(self):
        return str(vars(self))

    def __str__(self):
        return '{}{}'.format(self.type_, self.name)


def find_symbols_for_file(sourcefile, patterns):
    '''Convert given pattern 'foo.c:bar,fum' to a list of the symbols for
    given file.

    '''

    symbols = []

    for pattern in patterns:
        if ':' not in pattern:
            sys.exit("error: stub pattern '{}' does not contain a ':'".format(
                pattern))

        pattern_file, pattern_symbols = pattern.split(':')

        if pattern_file == sourcefile:
            symbols += pattern_symbols.split(',')

    return symbols


def parse_args(args, comment):
    '''Parse given function argument string and return a list of argument
    object.

    '''

    # Functions with 'void' as the only argument does not take any
    # arguments.
    if args == ['void']:
        return []

    arguments = []

    for arg in args:
        if arg in ['...', '']:
            continue

        mo = re.match(r'(.+[^\w\[\]])([\w\[\]]+)', arg)

        if not mo:
            sys.exit("Invalid argument '{}'.".format(arg))

        type_, name = mo.groups()
        mo = re.search(r'\[(\w+)\]\s+{}\s'.format(name),
                       comment,
                       re.DOTALL)

        if mo:
            direction = mo.group(1).split(',')
        else:
            direction = []

        arguments.append(Argument(name, type_, direction))

    return arguments


def generate_function_mock_write(return_type,
                                 name,
                                 arguments):
    '''Generate mock write function declaration and definition from given
    function return type, name and arguments.

    '''

    body = []

    if arguments:
        for argument in arguments:
            if argument.name == 'self_p':
                continue

            if (('*' in argument.type_)
                and (argument.name not in ['dev_p', 'pin_dev_p'])):
                if 'char' in argument.type_:
                    size = 'strlen({}) + 1'.format(argument.name)
                elif 'void' in argument.type_:
                    if any([arg.name == 'size' for arg in arguments]):
                        size = 'size'
                    else:
                        size = 'sizeof({})'.format(argument.name)
                else:
                    size = 'sizeof(*{})'.format(argument.name)

                if 'in' in argument.direction:
                    fmt = MOCK_WRITE_ARGUMENT_IN_PTR_FMT
                else:
                    fmt = MOCK_WRITE_ARGUMENT_OUT_PTR_FMT
            else:
                size = 'sizeof({})'.format(argument.name)
                fmt = MOCK_WRITE_ARGUMENT_IN_FMT

            body.append(fmt.format(function_name=name,
                                   name=argument.name,
                                   size=size))
    else:
        body.append(MOCK_WRITE_ARGUMENT_VOID_FMT.format(function_name=name))

    if return_type.strip() == 'void':
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
    args = [str(argument)
            for argument in arguments + return_argument
            if argument.name != 'self_p']
    declaration += (',\n' + padding).join(args)
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

    if return_type.strip() != 'void':
        body.append('    {}res;\n'.format(return_type))

    if arguments:
        for argument in arguments:
            if argument.name == 'self_p':
                continue

            if (('*' in argument.type_)
                and (argument.name not in ['dev_p', 'pin_dev_p'])):
                if 'in' in argument.direction:
                    size = 'sizeof(*{})'.format(argument.name)
                    fmt = MOCK_ARGUMENT_IN_PTR_FMT
                else:
                    size = 'sizeof(*{})'.format(argument.name)
                    fmt = MOCK_ARGUMENT_OUT_PTR_FMT
            else:
                size = 'sizeof({})'.format(argument.name)
                fmt = MOCK_ARGUMENT_IN_FMT

            body.append(fmt.format(function_name=name,
                                   name=argument.name,
                                   size=size))
    else:
        body.append(MOCK_ARGUMENT_VOID_FMT.format(function_name=name))

    if return_type.strip() != 'void':
        body.append(MOCK_ARGUMENT_OUT_FMT.format(function_name=name,
                                                 name='res',
                                                 size='sizeof(res)'))
        body += ['    return (res);', '']

    function_open = '{}__attribute__ ((weak)) STUB({})('.format(return_type, name)
    padding = ' ' * len(function_open)
    declaration = function_open
    declaration += (',\n' + padding).join([str(argument)
                                           for argument in arguments])
    declaration += ')'

    return STUB_FUNCTION_FMT.format(declaration=declaration,
                                    body='\n'.join(body))


def generate_function_stubs(declaration, comment, functions_to_ignore):
    '''Generate the stub definitions of given function.

    '''

    re_declaration = re.compile(r'([\w \r\n\*]+?)(\w+)\((.*?)\)$',
                                re.MULTILINE | re.DOTALL)

    return_type, name, args = re_declaration.match(declaration).groups()

    if 'inline' in return_type:
        return None, None

    if name in functions_to_ignore:
        return None, None

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


def generate_from_header(args,
                         re_function,
                         re_file_header,
                         header_path):
    '''Generate stub files for given header file.

    '''

    header_dir, header_file = os.path.split(header_path)
    header_name = os.path.splitext(header_file)[0]
    mock_dir = os.path.join(args.outdir, header_dir)

    try:
        os.makedirs(mock_dir)
    except FileExistsError:
        pass

    mock_h_path = os.path.join(mock_dir, header_name + '_mock.h')
    mock_c_path = os.path.join(mock_dir, header_name + '_mock.c')

    print("Generating '{}' and '{}' from '{}'.".format(
        mock_h_path,
        mock_c_path,
        header_path))

    with open(header_path, 'r') as fin:
        header_contents = fin.read()

    functions = re_function.findall(header_contents)
    file_header = re_file_header.search(header_contents)

    mock_h = []
    mock_c = []

    for comment, declaration in functions:
        func_h, func_c = generate_function_stubs(declaration.strip(),
                                                 comment,
                                                 args.ignore_function)
        if func_h and func_c:
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

    return mock_h_path, mock_c_path


def do_patch(args):
    '''Repalce symbols in an object file with their stubbed version.

    '''

    # Find symbols to stub in given source file (if any).
    symbols = find_symbols_for_file(args.sourcefile, args.patterns)

    if not symbols:
        return

    # Find all symbols in the object file.
    command = [args.crosscompile + 'readelf']
    command += ['-s']
    command += ['-W']
    command += [args.objectfile]

    readelf_output = subprocess.check_output(command)
    re_symbol = re.compile(r"\s+\d+: \w+\s+\d+\s+(NOTYPE|OBJECT|FUNC)"
                           r"\s+\w+\s+\w+\s+\w+\s+(.+)")
    symbols_in_objectfile = []

    for line in readelf_output.decode('ascii').splitlines():
        mo = re_symbol.match(line)

        if mo:
            symbols_in_objectfile.append(mo.group(2).strip())

    # Redefine given symbols in the object file.
    command = [args.crosscompile + 'objcopy']

    for symbol in symbols:
        if symbol not in symbols_in_objectfile:
            sys.exit("error: cannot stub missing symbol '{}'".format(symbol))

        print("Stubbing symbol '{}' in '{}'.".format(symbol, args.sourcefile))

        command += [
            '--redefine-sym', '{symbol}=__stub_{symbol}'.format(symbol=symbol)
        ]

    command += [args.objectfile, args.objectfile]

    subprocess.check_call(command)


def do_generate(args):
    '''Generate stubs for given header files.

    '''

    re_function = re.compile(r'/\*\*(.+?)\*/([\w \r\n\*]+?\w+\(.*?\));',
                             re.MULTILINE | re.DOTALL)
    re_file_header = re.compile(r'(/\*\*(.+?)\*/)',
                                re.MULTILINE | re.DOTALL)

    for header_path in args.headers:
        try:
            generate_from_header(args,
                                 re_function,
                                 re_file_header,
                                 header_path)
        except AttributeError:
            print("Failed to generate stub for '{}'".format(
                header_path))


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

    # The 'patch' subparser.
    patch_parser = subparsers.add_parser('patch')
    patch_parser.add_argument('crosscompile')
    patch_parser.add_argument('objectfile')
    patch_parser.add_argument('sourcefile')
    patch_parser.add_argument('patterns', nargs='+')
    patch_parser.set_defaults(func=do_patch)

    # The 'generate' subparser.
    generate_parser = subparsers.add_parser('generate')
    generate_parser.add_argument('-i', '--ignore-function',
                                 action='append',
                                 default=[],
                                 help='One or more functions to ignore.')
    generate_parser.add_argument('headers',
                                 nargs='+',
                                 help='One or more header files to generate stubs for.')
    generate_parser.add_argument('outdir',
                                 help='Output directory.')
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
