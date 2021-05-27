#include <gmock/gmock.h>
#include <gtest/gtest.h>

#define __main__

extern "C" {
#include "main.c"
}

namespace {

TEST(TreapTest, insert) {
    Node *root = NULL;
    root = Insert(root, 0, 1);
    root = Insert(root, 1, 2);
    root = Insert(root, 1, 3);
    root = Insert(root, 3, 4);
    root = Insert(root, 2, 5);
    root = Insert(root, 5, 6);
    root = Insert(root, 4, 7);
    root = Insert(root, 3, 8);
    root = Insert(root, 0, 9);

    int array[9] = {0};
    int n = inorder(root, array);
    EXPECT_EQ(n, 9);
    EXPECT_THAT(array, testing::ElementsAre(9, 1, 3, 5, 8, 2, 7, 4, 6));

    FreeTreap(root);
}

} // namespace

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
