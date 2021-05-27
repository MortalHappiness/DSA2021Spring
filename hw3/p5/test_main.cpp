#include <gmock/gmock.h>
#include <gtest/gtest.h>

#define __main__

extern "C" {
#include "main.c"
}

namespace {

TEST(TreapTest, Insert) {
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

TEST(TreapTest, Delete) {
    Node *root = NULL;
    root = Insert(root, 0, 10);
    root = Insert(root, 0, 8);
    root = Insert(root, 1, 5);
    root = Insert(root, 3, 18);
    root = Insert(root, 1, 14);
    root = Insert(root, 2, 14);
    root = Insert(root, 0, 1);
    root = Insert(root, 5, 15);
    root = Insert(root, 1, 10);

    int array[9] = {0};
    int n = inorder(root, array);
    EXPECT_EQ(n, 9);
    EXPECT_THAT(array, testing::ElementsAre(1, 10, 8, 14, 14, 5, 15, 10, 18));

    for (int i = 0; i < 9; ++i)
        array[i] = 0;

    root = Delete(root, 3);
    n = inorder(root, array);
    EXPECT_EQ(n, 8);
    EXPECT_THAT(array, testing::ElementsAre(1, 10, 14, 14, 5, 15, 10, 18, 0));

    for (int i = 0; i < 9; ++i)
        array[i] = 0;

    root = Delete(root, 1);
    n = inorder(root, array);
    EXPECT_EQ(n, 7);
    EXPECT_THAT(array, testing::ElementsAre(10, 14, 14, 5, 15, 10, 18, 0, 0));

    for (int i = 0; i < 9; ++i)
        array[i] = 0;

    root = Delete(root, 7);
    n = inorder(root, array);
    EXPECT_EQ(n, 6);
    EXPECT_THAT(array, testing::ElementsAre(10, 14, 14, 5, 15, 10, 0, 0, 0));

    for (int i = 0; i < 9; ++i)
        array[i] = 0;

    for (int i = 0; i < 6; ++i)
        root = Delete(root, 1);
    n = inorder(root, array);
    EXPECT_EQ(n, 0);
    EXPECT_THAT(array, testing::ElementsAre(0, 0, 0, 0, 0, 0, 0, 0, 0));

    FreeTreap(root);
}

TEST(TreapTest, Split) {
    Node *root = NULL;
    root = Insert(root, 0, 6);
    root = Insert(root, 1, 18);
    root = Insert(root, 2, 17);
    root = Insert(root, 1, 1);
    root = Insert(root, 0, 17);
    root = Insert(root, 1, 4);
    root = Insert(root, 1, 6);
    root = Insert(root, 0, 20);
    root = Insert(root, 1, 4);

    int array[9] = {0};
    int n = inorder(root, array);
    EXPECT_EQ(n, 9);
    EXPECT_THAT(array, testing::ElementsAre(20, 4, 17, 6, 4, 6, 1, 18, 17));

    Node *treap1, *treap2;
    Split(root, 4, &treap1, &treap2);

    for (int i = 0; i < 9; ++i)
        array[i] = 0;
    n = inorder(treap1, array);
    EXPECT_EQ(n, 4);
    EXPECT_THAT(array, testing::ElementsAre(20, 4, 17, 6, 0, 0, 0, 0, 0));

    for (int i = 0; i < 9; ++i)
        array[i] = 0;
    n = inorder(treap2, array);
    EXPECT_EQ(n, 5);
    EXPECT_THAT(array, testing::ElementsAre(4, 6, 1, 18, 17, 0, 0, 0, 0));

    FreeTreap(treap1);
    FreeTreap(treap2);
}

} // namespace

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
