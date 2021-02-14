import unittest
import subprocess
import math
import random

# ========================================


class TestGCD(unittest.TestCase):
    def setUp(self):
        random.seed(0)

    def test_1_1__sample_testcase_1(self):
        """ Test gcd for sample testcase 1. """
        a, b = 20210208, 80201202
        ans = 6
        output = subprocess.check_output(["./gcd"], input=f"{a} {b}".encode())
        self.assertEqual(ans, int(output))

    def test_1_2__sample_testcase_2(self):
        """ Test gcd for sample testcase 2. """
        a, b = 987654321987654321987654321, 123456789123456789123456789
        ans = 9000000009000000009
        output = subprocess.check_output(["./gcd"], input=f"{a} {b}".encode())
        self.assertEqual(ans, int(output))

    def test_2_1__randomized_testing_1(self):
        """ Test gcd by random numbers. """
        for _ in range(100):
            a = random.randint(1, 10**256-1)
            b = random.randint(1, 10**256-1)
            output = subprocess.check_output(
                ["./gcd"], input=f"{a} {b}".encode())
            self.assertEqual(math.gcd(a, b), int(output))

    def test_2_2__randomized_testing_2(self):
        """ Test gcd by random numbers with bigger common factor. """
        for _ in range(100):
            factor = random.randint(1, 10**100)
            a = random.randint(1, 10**128-1) * factor
            b = random.randint(1, 10**128-1) * factor
            output = subprocess.check_output(
                ["./gcd"], input=f"{a} {b}".encode())
            self.assertEqual(math.gcd(a, b), int(output))

# ========================================


if __name__ == "__main__":
    unittest.main(verbosity=2)
