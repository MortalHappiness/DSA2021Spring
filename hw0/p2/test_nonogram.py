import unittest
import subprocess

# ========================================


class TestNonogram(unittest.TestCase):
    def test_1_1__sample_testcase_1(self):
        """ Test nonogram for sample testcase 1. """
        with open("testcases/1.txt", "rb") as fin:
            input_data = fin.read()
        ans = (
            b'o_o_\n'
            b'_o_o\n'
            b'o_o_\n'
            b'oooo\n'
        )
        output = subprocess.check_output(["./nonogram"], input=input_data)
        self.assertEqual(ans, output)

    def test_1_2__sample_testcase_2(self):
        """ Test nonogram for sample testcase 2. """
        with open("testcases/2.txt", "rb") as fin:
            input_data = fin.read()
        ans = (
            b'_oooo\n'
            b'oo___\n'
            b'_ooo_\n'
            b'___oo\n'
            b'oooo_\n'
        )
        output = subprocess.check_output(["./nonogram"], input=input_data)
        self.assertEqual(ans, output)

# ========================================


if __name__ == "__main__":
    unittest.main(verbosity=2)
