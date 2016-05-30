import unittest

from readchar import readkey


def readchar_fn_factory(stream):

    v = [x for x in stream]

    def inner(blocking=False):
        return v.pop(0)
    return inner


class ReadKeyTest(unittest.TestCase):
    def test_basic_character(self):
        getchar_fn = readchar_fn_factory('a')

        result = readkey(getchar_fn)

        self.assertEqual('a', result)

    def test_string_instead_of_char(self):
        char = 'a'
        getchar_fn = readchar_fn_factory(char + 'bcde')

        result = readkey(getchar_fn)

        self.assertEqual(char, result)

    def test_special_combo_character(self):
        char = '\x1b\x01'
        getchar_fn = readchar_fn_factory(char + 'foo')

        result = readkey(getchar_fn)

        self.assertEqual(char, result)

    def test_special_key(self):
        char = '\x1b\x5b\x41'
        getchar_fn = readchar_fn_factory(char + 'foo')

        result = readkey(getchar_fn)

        self.assertEqual(char, result)

    def test_special_key_combo(self):
        char = '\x1b\x5b\x33\x5e'
        getchar_fn = readchar_fn_factory(char + 'foo')

        result = readkey(getchar_fn)

        self.assertEqual(char, result)
