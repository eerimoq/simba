import unittest

from readchar import key


class KeyTest(unittest.TestCase):
    def test_character_length_1(self):
        self.assertEqual(1, len(key.CTRL_A))

    def test_character_length_2(self):
        self.assertEqual(2, len(key.ALT_A))

    def test_character_length_3(self):
        self.assertEqual(3, len(key.UP))

    def test_character_length_4(self):
        self.assertEqual(4, len(key.CTRL_ALT_SUPR))
