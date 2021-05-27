#include <gtest/gtest.h>

#define __main__

extern "C" {
#include "main.c"
}

namespace {

TEST(AddTest, add) { EXPECT_EQ(2, add(1, 1)); }

} // namespace

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
