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

    def test_1_sample(self):
        self.run_testcase(1)

    def test_2_sample(self):
        self.run_testcase(2)

    def test_3_sample(self):
        self.run_testcase(3)

    def test_4_only_query(self):
        self.run_testcase(4)

    def test_5_no_query(self):
        self.run_testcase(5)

    def test_6_mix(self):
        self.run_testcase(6)


if __name__ == "__main__":
    unittest.main()
