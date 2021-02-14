#include <gtest/gtest.h>

extern "C" {
#include "bigint.h"
}

namespace {

TEST(BigIntTest, print) {
    BigInt *x;
    std::string output;

    char str[256];
    char c = '9';
    int i;
    for (i = 0; i < 200; ++i) {
        str[i] = c;
        c = (c == '0') ? '8' : (c - 1);
    }
    for (i = 200; i < 230; ++i)
        str[i] = '0';
    str[230] = '\0';

    x = BigInt_init(str);
    testing::internal::CaptureStdout();
    BigInt_print(x);
    output = testing::internal::GetCapturedStdout();
    EXPECT_STREQ(str, output.c_str());
    BigInt_destroy(x);

    x = BigInt_init("0");
    testing::internal::CaptureStdout();
    BigInt_print(x);
    output = testing::internal::GetCapturedStdout();
    EXPECT_STREQ("0", output.c_str());
    BigInt_destroy(x);
}

TEST(BigIntTest, iszero) {
    BigInt *a = BigInt_init("0");
    BigInt *b = BigInt_init("1");

    EXPECT_TRUE(BigInt_iszero(a));
    EXPECT_FALSE(BigInt_iszero(b));

    BigInt_destroy(a);
    BigInt_destroy(b);
}

TEST(BigIntTest, iseven) {
    BigInt *a = BigInt_init("98765432101012345678");
    BigInt *b = BigInt_init("987654321010123456789");
    BigInt *c = BigInt_init("0");

    EXPECT_TRUE(BigInt_iseven(a));
    EXPECT_FALSE(BigInt_iseven(b));
    EXPECT_TRUE(BigInt_iseven(c));

    BigInt_destroy(a);
    BigInt_destroy(b);
    BigInt_destroy(c);
}

TEST(BigIntTest, compare) {
    BigInt *a = BigInt_init("987654321010123456788");
    BigInt *b = BigInt_init("987654321010123456787");
    BigInt *c = BigInt_init("123");
    BigInt *d = BigInt_init("0");

    EXPECT_TRUE(BigInt_compare(a, a) == 0);
    EXPECT_TRUE(BigInt_compare(b, b) == 0);
    EXPECT_TRUE(BigInt_compare(c, c) == 0);
    EXPECT_TRUE(BigInt_compare(d, d) == 0);

    EXPECT_TRUE(BigInt_compare(a, b) > 0);
    EXPECT_TRUE(BigInt_compare(a, c) > 0);
    EXPECT_TRUE(BigInt_compare(a, d) > 0);
    EXPECT_TRUE(BigInt_compare(b, c) > 0);
    EXPECT_TRUE(BigInt_compare(b, d) > 0);
    EXPECT_TRUE(BigInt_compare(c, d) > 0);

    EXPECT_TRUE(BigInt_compare(b, a) < 0);
    EXPECT_TRUE(BigInt_compare(c, a) < 0);
    EXPECT_TRUE(BigInt_compare(d, a) < 0);
    EXPECT_TRUE(BigInt_compare(c, b) < 0);
    EXPECT_TRUE(BigInt_compare(d, b) < 0);
    EXPECT_TRUE(BigInt_compare(d, c) < 0);

    BigInt_destroy(a);
    BigInt_destroy(b);
    BigInt_destroy(c);
    BigInt_destroy(d);
}

TEST(BigIntTest, subtract) {
    BigInt *a = BigInt_init("987654321010123456787");
    BigInt *b = BigInt_init("91234567890987654321");
    BigInt *c = BigInt_init("91234567890987654322");
    BigInt *n = BigInt_init("123456789123456789123456789");
    BigInt *m = BigInt_init("987654321987654321987654321");

    std::string output;

    // a > b, so a -= b
    BigInt_subtract(a, b);
    testing::internal::CaptureStdout();
    BigInt_print(a);
    output = testing::internal::GetCapturedStdout();
    EXPECT_STREQ("896419753119135802466", output.c_str());

    // b < c, so unchanged
    BigInt_subtract(b, c);
    testing::internal::CaptureStdout();
    BigInt_print(b);
    output = testing::internal::GetCapturedStdout();
    EXPECT_STREQ("91234567890987654321", output.c_str());

    // c - b = 1
    BigInt_subtract(c, b);
    testing::internal::CaptureStdout();
    BigInt_print(c);
    output = testing::internal::GetCapturedStdout();
    EXPECT_STREQ("1", output.c_str());

    // b - b = 0
    BigInt_subtract(b, b);
    EXPECT_TRUE(BigInt_iszero(b));

    BigInt_subtract(m, n);
    testing::internal::CaptureStdout();
    BigInt_print(m);
    output = testing::internal::GetCapturedStdout();
    EXPECT_STREQ("864197532864197532864197532", output.c_str());

    BigInt_destroy(a);
    BigInt_destroy(b);
    BigInt_destroy(c);
    BigInt_destroy(n);
    BigInt_destroy(m);
}

TEST(BigIntTest, div2) {
    BigInt *x = BigInt_init("987654321010123456787");
    std::string output;

    BigInt_div2(x);
    testing::internal::CaptureStdout();
    BigInt_print(x);
    output = testing::internal::GetCapturedStdout();
    EXPECT_STREQ("493827160505061728393", output.c_str());

    for (int i = 0; i < 50; ++i)
        BigInt_div2(x);
    testing::internal::CaptureStdout();
    BigInt_print(x);
    output = testing::internal::GetCapturedStdout();
    EXPECT_STREQ("438606", output.c_str());

    BigInt_destroy(x);
}

TEST(BigIntTest, mul2power) {
    BigInt *x = BigInt_init("123");
    std::string output;

    BigInt_mul2power(x, 1);
    testing::internal::CaptureStdout();
    BigInt_print(x);
    output = testing::internal::GetCapturedStdout();
    EXPECT_STREQ("246", output.c_str());

    BigInt_mul2power(x, 100);
    testing::internal::CaptureStdout();
    BigInt_print(x);
    output = testing::internal::GetCapturedStdout();
    EXPECT_STREQ("311842047656144432768188988522496", output.c_str());

    BigInt_destroy(x);
}

} // namespace

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
