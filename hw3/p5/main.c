/* References:
 * 1.
 * https://stackoverflow.com/questions/37236176/data-structure-for-inverting-a-subarray-in-logn
 * 2. https://en.wikipedia.org/wiki/Treap
 */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define ERR_EXIT(msg) \
    do {              \
        perror(msg);  \
        exit(1);      \
    } while (0)

// ========================================

typedef enum {
    LEFT,
    RIGHT,
    SELF,
} Position;

// ========================================
// Treap implementation

#define MAX_PRIORITY 2147483647
#define MIN_PRIORITY (-1)

typedef struct Node {
    struct Node *parent;
    struct Node *left;
    struct Node *right;
    int treap_priority;
    int subtree_size; // The number of nodes in the subtree. Using this we can
                      // compute the in-order index of any node. The in-order
                      // traversal index is used as the key of the treap

    // The following attributes are not related to basic treap implementation
    int diff_priority;     // Start from root, add up this along the path will
                           // result in the actual priority
    bool subtree_reversed; // Whether the subtree below is reversed
    int largest_priority;  // The largest priority in the subtree
    Position largest_position; // The position of the node with largest priority
                               // in the subtree
} Node;

int _rand_priority() { return rand() & 0xfffffff; }

// ========================================

Node *NewNode() {
    Node *node = (Node *)malloc(sizeof(Node));
    node->parent = NULL;
    node->left = NULL;
    node->right = NULL;
    node->treap_priority = _rand_priority();
    node->subtree_reversed = false;
    return node;
}

void _maintain_largest_priority(Node *node) {
    node->largest_position = SELF;
    node->largest_priority = node->diff_priority;
    if (node->left && node->left->largest_priority + node->diff_priority >
                          node->largest_priority) {
        node->largest_priority =
            node->left->largest_priority + node->diff_priority;
        node->largest_position = LEFT;
    }
    if (node->right && node->right->largest_priority + node->diff_priority >
                           node->largest_priority) {
        node->largest_priority =
            node->right->largest_priority + node->diff_priority;
        node->largest_position = RIGHT;
    }
}

Node *left_rotate(Node *root, Node *node) {
    Node *child;
    child = node->right;
    node->right = child->left;
    if (child->left != NULL)
        child->left->parent = node;
    child->parent = node->parent;
    if (node->parent == NULL) {
        root = child;
    } else if (node == node->parent->left) {
        node->parent->left = child;
    } else {
        node->parent->right = child;
    }
    child->left = node;
    node->parent = child;

    // maintain subtree_size
    node->subtree_size = 1;
    if (node->left != NULL)
        node->subtree_size += node->left->subtree_size;
    if (node->right != NULL)
        node->subtree_size += node->right->subtree_size;
    child->subtree_size = child->left->subtree_size + 1;
    if (child->right != NULL)
        child->subtree_size += child->right->subtree_size;

    // maintain diff_priority
    int node_diff_priority = node->diff_priority;
    int child_diff_priority = child->diff_priority;
    child->diff_priority = node_diff_priority + child_diff_priority;
    node->diff_priority = -child_diff_priority;
    if (node->right != NULL)
        node->right->diff_priority += child_diff_priority;

    // maintain largest_priority
    _maintain_largest_priority(node);
    _maintain_largest_priority(child);

    return root;
}

Node *right_rotate(Node *root, Node *node) {
    Node *child;
    child = node->left;
    node->left = child->right;
    if (child->right != NULL)
        child->right->parent = node;
    child->parent = node->parent;
    if (node->parent == NULL) {
        root = child;
    } else if (node == node->parent->left) {
        node->parent->left = child;
    } else {
        node->parent->right = child;
    }
    child->right = node;
    node->parent = child;

    // maintain subtree_size
    node->subtree_size = 1;
    if (node->left != NULL)
        node->subtree_size += node->left->subtree_size;
    if (node->right != NULL)
        node->subtree_size += node->right->subtree_size;
    child->subtree_size = child->right->subtree_size + 1;
    if (child->left != NULL)
        child->subtree_size += child->left->subtree_size;

    // maintain diff_priority
    int node_diff_priority = node->diff_priority;
    int child_diff_priority = child->diff_priority;
    child->diff_priority = node_diff_priority + child_diff_priority;
    node->diff_priority = -child_diff_priority;
    if (node->left != NULL)
        node->left->diff_priority += child_diff_priority;

    // maintain largest_priority
    _maintain_largest_priority(node);
    _maintain_largest_priority(child);

    return root;
}

Node *sift_up(Node *root, Node *node) {
    while (node->parent != NULL &&
           node->treap_priority > node->parent->treap_priority) {
        if (node->parent->left == node) {
            root = right_rotate(root, node->parent);
        } else {
            root = left_rotate(root, node->parent);
        }
    }
    return root;
}

Node *sift_down(Node *root, Node *node) { return root; }

// ========================================

Node *Insert(Node *root, int index, int priority) {
    int current_index, diff, acc_priority;
    Node *node, *parent, *child, *newnode;
    Position position;

    newnode = NewNode();
    newnode->subtree_size = 1;
    newnode->largest_position = SELF;

    if (root == NULL) {
        newnode->largest_priority = newnode->diff_priority = priority;
        return newnode;
    }

    current_index = diff = acc_priority = 0;
    node = root;
    parent = NULL;
    while (node != NULL) {
        // compute current_index
        if (node->right == NULL) {
            diff = node->subtree_size;
        } else {
            diff = node->subtree_size - node->right->subtree_size;
        }
        current_index += diff;
        // compute acc_priority
        acc_priority += node->diff_priority;

        parent = node;
        if (index < current_index) {
            current_index -= diff;
            node = node->left;
            position = LEFT;
        } else {
            node = node->right;
            position = RIGHT;
        }
    }
    newnode->parent = parent;
    newnode->largest_priority = newnode->diff_priority =
        priority - acc_priority;

    if (position == LEFT) {
        parent->left = newnode;
    } else {
        parent->right = newnode;
    }

    // traverse up to root and increase subtree size
    node = parent;
    while (node != NULL) {
        ++node->subtree_size;
        node = node->parent;
    }

    // traverse up to root and update largest_priority
    node = parent;
    child = newnode;
    while (node != NULL) {
        if (node->left == child &&
            (node->diff_priority + child->largest_priority >
             node->largest_priority)) {
            node->largest_priority =
                node->diff_priority + child->largest_priority;
            node->largest_position = LEFT;
        }
        if (node->right == child &&
            (node->diff_priority + child->largest_priority >
             node->largest_priority)) {
            node->largest_priority =
                node->diff_priority + child->largest_priority;
            node->largest_position = RIGHT;
        }
        child = node;
        node = node->parent;
    }

    root = sift_up(root, newnode);

    return root;
}

Node *Delete(Node *root, int index) { return root; }

Node *IncreasePriority(Node *root, int left, int right, int priority) {
    return root;
}

int LargestPriority(Node *root, int left, int right) { return 0; }

Node *Reverse(Node *root, int left, int right) { return root; }

Node *RemoveLargestPriority(Node *root) { return root; }

void FreeTreap(Node *root) {
    if (root == NULL)
        return;
    FreeTreap(root->left);
    FreeTreap(root->right);
    free(root);
}

// ========================================

int _inorder(Node *root, int *array, int idx, int acc_priority) {
    if (root == NULL)
        return idx;
    if ((root->left && root->left->parent != root) ||
        (root->right && root->right->parent != root))
        ERR_EXIT("Parent link corrupted");
    int priority = acc_priority + root->diff_priority;
    idx = _inorder(root->left, array, idx, priority);
    array[idx++] = priority;
    idx = _inorder(root->right, array, idx, priority);
    return idx;
}

// Inorder traveral for debug, assume the array is large enough
int inorder(Node *root, int *array) { return _inorder(root, array, 0, 0); }

// ========================================

#ifndef __main__
#define __main__
int main(int argc, char *argv[]) {
    int n, N, Q, i, x, y, z;
    if ((n = scanf("%d %d", &N, &Q) != 2))
        ERR_EXIT("scanf");
    for (i = 0; i < N; ++i) {
        if ((n = scanf("%d", &x) != 1))
            ERR_EXIT("scanf");
    }
    for (i = 0; i < Q; ++i) {
        if ((n = scanf("%d", &x) != 1))
            ERR_EXIT("scanf");
        switch (x) {
            case 1:
                if ((n = scanf("%d %d", &x, &y) != 2))
                    ERR_EXIT("scanf");
                break;
            case 2:
                if ((n = scanf("%d", &x) != 1))
                    ERR_EXIT("scanf");
                break;
            case 3:
                if ((n = scanf("%d %d %d", &x, &y, &z) != 3))
                    ERR_EXIT("scanf");
                break;
            case 4:
                if ((n = scanf("%d %d", &x, &y) != 2))
                    ERR_EXIT("scanf");
                break;
            case 5:
                if ((n = scanf("%d %d", &x, &y) != 2))
                    ERR_EXIT("scanf");
                break;
            case 6:
                break;
            default:
                ERR_EXIT("Invalid operation number");
                break;
        }
    }
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
    return 0;
}
#endif
