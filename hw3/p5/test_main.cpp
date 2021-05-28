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

TEST(TreapTest, Split_Merge) {
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

    Node *treap1, *treap2, *treap3;
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

    Split(treap2, 3, &treap2, &treap3);

    for (int i = 0; i < 9; ++i)
        array[i] = 0;
    n = inorder(treap2, array);
    EXPECT_EQ(n, 3);
    EXPECT_THAT(array, testing::ElementsAre(4, 6, 1, 0, 0, 0, 0, 0, 0));

    for (int i = 0; i < 9; ++i)
        array[i] = 0;
    n = inorder(treap3, array);
    EXPECT_EQ(n, 2);
    EXPECT_THAT(array, testing::ElementsAre(18, 17, 0, 0, 0, 0, 0, 0, 0));

    treap1 = Merge(treap1, treap3);
    n = inorder(treap1, array);
    EXPECT_EQ(n, 6);
    EXPECT_THAT(array, testing::ElementsAre(20, 4, 17, 6, 18, 17, 0, 0, 0));

    root = Merge(treap2, treap1);
    n = inorder(root, array);
    EXPECT_EQ(n, 9);
    EXPECT_THAT(array, testing::ElementsAre(4, 6, 1, 20, 4, 17, 6, 18, 17));

    FreeTreap(root);
}

TEST(TreapTest, LargestPriority) {
    Node *root = NULL;
    root = Insert(root, 0, -9);
    root = Insert(root, 0, -7);
    root = Insert(root, 1, -2);
    root = Insert(root, 0, 7);
    root = Insert(root, 1, 6);
    root = Insert(root, 3, -3);
    root = Insert(root, 2, -4);
    root = Insert(root, 2, 1);
    root = Insert(root, 6, 0);

    int array[9] = {0};
    int n = inorder(root, array);
    EXPECT_EQ(n, 9);
    EXPECT_THAT(array, testing::ElementsAre(7, 6, 1, -4, -7, -3, 0, -2, -9));

    root = LargestPriority(root, 3, 8, &n);
    EXPECT_EQ(n, 1);
    root = LargestPriority(root, 2, 5, &n);
    EXPECT_EQ(n, 6);
    root = LargestPriority(root, 1, 9, &n);
    EXPECT_EQ(n, 7);
    root = LargestPriority(root, 8, 9, &n);
    EXPECT_EQ(n, -2);

    FreeTreap(root);
}

TEST(TreapTest, IncreasePriority) {
    Node *root = NULL;
    root = Insert(root, 0, 1);
    root = Insert(root, 0, -5);
    root = Insert(root, 2, -2);
    root = Insert(root, 1, 0);
    root = Insert(root, 4, -1);
    root = Insert(root, 0, 3);
    root = Insert(root, 2, -4);
    root = Insert(root, 3, -2);
    root = Insert(root, 1, -5);

    int array[9] = {0};
    int n = inorder(root, array);
    EXPECT_EQ(n, 9);
    EXPECT_THAT(array, testing::ElementsAre(3, -5, -5, -4, -2, 0, 1, -2, -1));

    root = IncreasePriority(root, 3, 6, 2);
    n = inorder(root, array);
    EXPECT_EQ(n, 9);
    EXPECT_THAT(array, testing::ElementsAre(3, -5, -3, -2, 0, 2, 1, -2, -1));

    root = IncreasePriority(root, 1, 9, -1);
    n = inorder(root, array);
    EXPECT_EQ(n, 9);
    EXPECT_THAT(array, testing::ElementsAre(2, -6, -4, -3, -1, 1, 0, -3, -2));
    FreeTreap(root);
}

TEST(TreapTest, Insert_Query_Error1) {
    Node *root = NULL;
    root = Insert(root, 0, -3);
    root = Insert(root, 1, -1);
    root = Insert(root, 2, -4);
    root = Insert(root, 3, -5);
    root = Insert(root, 4, 2);

    int array[6] = {0};
    int n = inorder(root, array);
    EXPECT_EQ(n, 5);
    EXPECT_THAT(array, testing::ElementsAre(-3, -1, -4, -5, 2, 0));

    Insert(root, 3, -2);
    n = inorder(root, array);
    EXPECT_EQ(n, 6);
    EXPECT_THAT(array, testing::ElementsAre(-3, -1, -4, -2, -5, 2));

    root = LargestPriority(root, 2, 6, &n);
    EXPECT_EQ(n, 2);
    root = LargestPriority(root, 1, 5, &n);
    EXPECT_EQ(n, -1);
}

TEST(TreapTest, Reverse) {
    Node *root = NULL;
    root = Insert(root, 0, 4);
    root = Insert(root, 0, 7);
    root = Insert(root, 2, 2);
    root = Insert(root, 0, -6);
    root = Insert(root, 2, 9);
    root = Insert(root, 0, -8);
    root = Insert(root, 5, -9);
    root = Insert(root, 6, -2);
    root = Insert(root, 6, -4);

    int array[9] = {0};
    int n = inorder(root, array);
    EXPECT_EQ(n, 9);
    EXPECT_THAT(array, testing::ElementsAre(-8, -6, 7, 9, 4, -9, -4, -2, 2));

    root = Reverse(root, 3, 6);
    n = inorder(root, array);
    EXPECT_EQ(n, 9);
    EXPECT_THAT(array, testing::ElementsAre(-8, -6, -9, 4, 9, 7, -4, -2, 2));

    root = Reverse(root, 1, 5);
    n = inorder(root, array);
    EXPECT_THAT(array, testing::ElementsAre(9, 4, -9, -6, -8, 7, -4, -2, 2));
    EXPECT_EQ(n, 9);

    root = Reverse(root, 4, 9);
    n = inorder(root, array);
    EXPECT_EQ(n, 9);
    EXPECT_THAT(array, testing::ElementsAre(9, 4, -9, 2, -2, -4, 7, -8, -6));

    root = Reverse(root, 1, 9);
    n = inorder(root, array);
    EXPECT_EQ(n, 9);
    EXPECT_THAT(array, testing::ElementsAre(-6, -8, 7, -4, -2, 2, -9, 4, 9));

    FreeTreap(root);
}

TEST(TreapTest, Mixed_Error1) {
    Node *root = NULL;
    root = Insert(root, 0, -6);
    root = Insert(root, 1, 2);
    root = Insert(root, 2, -6);
    root = Insert(root, 3, 2);
    root = Insert(root, 4, -3);
    root = Insert(root, 5, -9);
    root = Insert(root, 6, -4);
    root = Insert(root, 7, 2);
    root = Insert(root, 8, -5);
    root = Insert(root, 9, -4);

    int array[10] = {0};
    int ans1[10] = {-6, 2, -6, 2, -3, -9, -4, 2, -5, -4};
    int n = inorder(root, array);
    EXPECT_EQ(n, 10);
    EXPECT_THAT(array, testing::ElementsAreArray(ans1));

    root = LargestPriority(root, 5, 6, &n);
    EXPECT_EQ(n, -3);

    root = Reverse(root, 5, 6);
    n = inorder(root, array);
    int ans2[10] = {-6, 2, -6, 2, -9, -3, -4, 2, -5, -4};
    EXPECT_EQ(n, 10);
    EXPECT_THAT(array, testing::ElementsAreArray(ans2));

    root = IncreasePriority(root, 2, 5, 8);
    n = inorder(root, array);
    int ans3[10] = {-6, 10, 2, 10, -1, -3, -4, 2, -5, -4};
    EXPECT_EQ(n, 10);
    EXPECT_THAT(array, testing::ElementsAreArray(ans3));

    root = Reverse(root, 1, 5);
    n = inorder(root, array);
    int ans4[10] = {-1, 10, 2, 10, -6, -3, -4, 2, -5, -4};
    EXPECT_EQ(n, 10);
    EXPECT_THAT(array, testing::ElementsAreArray(ans4));

    int array2[9] = {0};
    int ans5[9] = {-1, 2, 10, -6, -3, -4, 2, -5, -4};
    root = RemoveLargestPriority(root);
    n = inorder(root, array2);
    EXPECT_EQ(n, 9);
    EXPECT_THAT(array2, testing::ElementsAreArray(ans5));

    root = LargestPriority(root, 3, 5, &n);
    EXPECT_EQ(n, 10);
    root = LargestPriority(root, 1, 1, &n);
    EXPECT_EQ(n, -1);
}

} // namespace

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
