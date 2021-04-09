#
# Author: Chi-Sheng Liu
#
# ========================================

import unittest
import subprocess
import os

# ========================================


class TestP6(unittest.TestCase):
    def setUp(self):
        self.timeout = 8
        self.maxDiff = None
        self.script_name = "./main"

    def run_testcase(self, num):
        with open(os.path.join("input", f"{num}.txt")) as fin:
            input_data = fin.read()
        with open(os.path.join("output", f"{num}.txt")) as fin:
            ans = fin.read()
        output = subprocess.check_output([self.script_name], text=True,
                                         timeout=self.timeout, input=input_data)
        self.assertEqual(output, ans)

    def test_01_sample(self):
        self.run_testcase(1)

    def test_02_sample(self):
        self.run_testcase(2)

    def test_03_sample(self):
        self.run_testcase(3)

    def test_04_only_query(self):
        self.run_testcase(4)

    def test_05_no_query(self):
        self.run_testcase(5)

    def test_06_mix_small(self):
        self.run_testcase(6)

    def test_07_mix_mid(self):
        self.run_testcase(7)

    def test_08_mix_large(self):
        self.run_testcase(8)

    def test_09_only_query_large(self):
        self.run_testcase(9)

    def test_10_no_query_large(self):
        self.run_testcase(10)

    def test_11_reverse(self):
        self.run_testcase(11)

    def test_12_reverse_query_large(self):
        self.run_testcase(12)

    def test_13_no_delete_large(self):
        self.run_testcase(13)

    def test_14_insert_reverse_large(self):
        self.run_testcase(14)

    def test_15_delete_to_zero(self):
        self.run_testcase(15)

    def test_16_delete_to_zero(self):
        self.run_testcase(16)

    def test_17_reverse_insert_delete_many(self):
        self.run_testcase(17)

    def test_18_query_pressure(self):
        self.run_testcase(18)

if __name__ == "__main__":
    unittest.main()
