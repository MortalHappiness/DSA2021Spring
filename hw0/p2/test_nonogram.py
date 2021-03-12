import unittest
import subprocess
import random

# ========================================


class TestNonogram(unittest.TestCase):
    def setUp(self):
        random.seed(0)

    def gen_testcase(self):
        n_row = random.randint(1, 25)
        n_col = random.randint(1, int(25 // n_row))
        board = [[random.randint(0, 1) for j in range(n_col)]
                 for i in range(n_row)]
        input_data = f"{n_row} {n_col}\n"
        rule = list()
        # row rules
        for i in range(n_row):
            rule.clear()
            prev = 0
            for j in range(n_col):
                if board[i][j]:
                    if prev:
                        rule[-1] += 1
                    else:
                        rule.append(1)
                    prev = 1
                else:
                    prev = 0
            input_data += "{} {}\n".format(
                len(rule), " ".join([str(x) for x in rule]))
        # column rules
        for j in range(n_col):
            rule.clear()
            prev = 0
            for i in range(n_row):
                if board[i][j]:
                    if prev:
                        rule[-1] += 1
                    else:
                        rule.append(1)
                    prev = 1
                else:
                    prev = 0
            input_data += "{} {}\n".format(
                len(rule), " ".join([str(x) for x in rule]))
        return input_data

    def check_ans(self, input_data, output):
        data = [[int(ch) for ch in x.split()] for x in input_data.split("\n")]
        n_row, n_col = data[0]
        row_rules = [x[1:] for x in data[1:n_row+1]]
        col_rules = [x[1:] for x in data[n_row+1:-1]]
        board = output.split("\n")[:-1]
        for i in range(n_row):
            j = 0
            for n in row_rules[i]:
                acc = 0
                while j < n_col and board[i][j] == "_":
                    j += 1
                while j < n_col and board[i][j] == "o":
                    j += 1
                    acc += 1
                if acc != n:
                    return False
            if any([board[i][x] == "o" for x in range(j, n_col)]):
                return False
        for j in range(n_col):
            i = 0
            for n in col_rules[j]:
                acc = 0
                while i < n_row and board[i][j] == "_":
                    i += 1
                while i < n_row and board[i][j] == "o":
                    i += 1
                    acc += 1
                if acc != n:
                    return False
            if any([board[x][j] == "o" for x in range(i, n_row)]):
                return False
        return True

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

    def test_2_1__random_test(self):
        """ Test nonogram with random testcases. """
        for _ in range(100):
            input_data = self.gen_testcase()
            output = subprocess.check_output(
                ["./nonogram"], input=input_data.encode())
            self.assertTrue(self.check_ans(input_data, output.decode()))

# ========================================


if __name__ == "__main__":
    unittest.main(verbosity=2)
