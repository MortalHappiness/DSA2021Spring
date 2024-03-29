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

void _fix_reverse(Node *node) {
    if (!node || !node->subtree_reversed)
        return;
    node->subtree_reversed = false;
    Node *left, *right;
    left = node->left;
    right = node->right;
    if (left)
        left->subtree_reversed = !(left->subtree_reversed);
    if (right)
        right->subtree_reversed = !(right->subtree_reversed);
    node->left = right;
    node->right = left;
    // maintain largest priority
    node->largest_position = SELF;
    node->largest_priority = node->diff_priority;
    if (node->right && node->right->largest_priority + node->diff_priority >
                           node->largest_priority) {
        node->largest_priority =
            node->right->largest_priority + node->diff_priority;
        node->largest_position = RIGHT;
    }
    if (node->left && node->left->largest_priority + node->diff_priority >=
                          node->largest_priority) {
        node->largest_priority =
            node->left->largest_priority + node->diff_priority;
        node->largest_position = LEFT;
    }
}

void _maintain_largest_priority(Node *node) {
    _fix_reverse(node);
    node->largest_position = SELF;
    node->largest_priority = node->diff_priority;
    if (node->right && node->right->largest_priority + node->diff_priority >
                           node->largest_priority) {
        node->largest_priority =
            node->right->largest_priority + node->diff_priority;
        node->largest_position = RIGHT;
    }
    if (node->left && node->left->largest_priority + node->diff_priority >=
                          node->largest_priority) {
        node->largest_priority =
            node->left->largest_priority + node->diff_priority;
        node->largest_position = LEFT;
    }
}

// left rotate, return tree root
Node *left_rotate(Node *root, Node *node) {
    _fix_reverse(node->parent);
    _fix_reverse(node);
    _fix_reverse(node->right);
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
    if (node->right != NULL)
        _maintain_largest_priority(node->right);
    _maintain_largest_priority(node);
    _maintain_largest_priority(child);

    return root;
}

// right rotate, return tree root
Node *right_rotate(Node *root, Node *node) {
    _fix_reverse(node->parent);
    _fix_reverse(node);
    _fix_reverse(node->left);
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
    if (node->left != NULL)
        _maintain_largest_priority(node->left);
    _maintain_largest_priority(node);
    _maintain_largest_priority(child);

    return root;
}

// sift up, return tree root
Node *sift_up(Node *root, Node *node) {
    while (node->parent != NULL &&
           node->treap_priority > node->parent->treap_priority) {
        _fix_reverse(node->parent);
        if (node->parent->left == node) {
            root = right_rotate(root, node->parent);
        } else {
            root = left_rotate(root, node->parent);
        }
    }
    return root;
}

// sift down, return tree root
Node *sift_down(Node *root, Node *node) {
    Position largest;
    int largest_priority;
    while (1) {
        _fix_reverse(node);
        largest = SELF;
        largest_priority = node->treap_priority;
        if (node->left && node->left->treap_priority > largest_priority) {
            largest = LEFT;
            largest_priority = node->left->treap_priority;
        }
        if (node->right && node->right->treap_priority > largest_priority) {
            largest = RIGHT;
            largest_priority = node->right->treap_priority;
        }
        if (largest == SELF)
            break;
        if (largest == LEFT) {
            root = right_rotate(root, node);
        } else {
            root = left_rotate(root, node);
        }
    }
    return root;
}

// ========================================

// Get, return the node searched
Node *Get(Node *root, const int index) {
    int current_index, diff;
    current_index = diff = 0;
    Node *node = root;
    while (node != NULL) {
        _fix_reverse(node);
        // compute current_index
        if (node->right == NULL) {
            diff = node->subtree_size;
        } else {
            diff = node->subtree_size - node->right->subtree_size;
        }
        current_index += diff;

        if (index == current_index) {
            return node;
        }
        if (index < current_index) {
            current_index -= diff;
            node = node->left;
        } else {
            node = node->right;
        }
    }
    return NULL;
}

// Insert, return tree root
Node *Insert(Node *root, int index, int priority) {
    int current_index, diff, acc_priority;
    Node *node, *parent, *newnode;
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
        _fix_reverse(node);
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

    // traverse up to root and increase subtree size and update largest_priority
    node = parent;
    while (node != NULL) {
        ++node->subtree_size;
        _fix_reverse(node);
        _maintain_largest_priority(node);
        node = node->parent;
    }

    root = sift_up(root, newnode);

    return root;
}

void delete_leaf_node(Node *node) {
    Node *parent;
    parent = node->parent;
    if (parent->left == node) {
        parent->left = NULL;
    } else {
        parent->right = NULL;
    }
    _fix_reverse(parent);
    _maintain_largest_priority(parent);
    // traverse up to root and decrease subtree size and maintain
    // largest_priority
    while (parent != NULL) {
        --parent->subtree_size;
        _fix_reverse(parent);
        _maintain_largest_priority(parent);
        parent = parent->parent;
    }
    free(node);
}

// Delete, return tree root
Node *Delete(Node *root, int index) {
    Node *node;
    node = Get(root, index);
    _fix_reverse(node);
    node->treap_priority = MIN_PRIORITY;
    root = sift_down(root, node);
    if (node == root) {
        free(node);
        return NULL;
    } else {
        delete_leaf_node(node);
        return root;
    }
}

// Split into two treaps and store them in the treap1_ptr and treap2_ptr
// treap1 = [1..index], treap2 = [index+1..n]
void Split(Node *root, int index, Node **treap1_ptr, Node **treap2_ptr) {
    _fix_reverse(root);

    Node *node;
    root = Insert(root, index, 0);
    node = Get(root, index + 1);
    _fix_reverse(node);
    node->treap_priority = MAX_PRIORITY;
    root = sift_up(root, node);
    _fix_reverse(root->left);
    _fix_reverse(root->right);
    *treap1_ptr = root->left;
    *treap2_ptr = root->right;
    if (root->left)
        root->left->parent = NULL;
    if (root->right)
        root->right->parent = NULL;
    free(root);
}

// Merge two treaps, return the new tree root
Node *Merge(Node *treap1, Node *treap2) {
    _fix_reverse(treap1);
    _fix_reverse(treap2);

    Node *root, *node;
    root = node = NewNode();
    root->left = treap1;
    root->right = treap2;
    root->treap_priority = MIN_PRIORITY;
    root->diff_priority = 0;
    root->subtree_size = 1;
    if (treap1) {
        treap1->parent = root;
        root->subtree_size += treap1->subtree_size;
    }
    if (treap2) {
        treap2->parent = root;
        root->subtree_size += treap2->subtree_size;
    }
    _maintain_largest_priority(root);
    root = sift_down(root, root);
    delete_leaf_node(node);
    _fix_reverse(root);
    return root;
}

// IncreasePriority, return tree root
Node *IncreasePriority(Node *root, int left, int right, int priority) {
    Node *treap1, *treap2, *treap3;
    Split(root, right, &treap2, &treap3);
    Split(treap2, left - 1, &treap1, &treap2);

    treap2->diff_priority += priority;

    root = Merge(treap1, treap2);
    root = Merge(root, treap3);
    return root;
}

// Store the largest priority in ans, return the tree root
Node *LargestPriority(Node *root, int left, int right, int *ans) {
    Node *treap1, *treap2, *treap3;
    treap1 = treap2 = treap3 = NULL;
    Split(root, right, &treap2, &treap3);
    Split(treap2, left - 1, &treap1, &treap2);
    *ans = treap2->largest_priority;

    root = Merge(treap1, treap2);
    root = Merge(root, treap3);
    return root;
}

// Reverse, return tree root
Node *Reverse(Node *root, int left, int right) {
    Node *treap1, *treap2, *treap3;
    treap1 = treap2 = treap3 = NULL;
    Split(root, right, &treap2, &treap3);
    Split(treap2, left - 1, &treap1, &treap2);

    treap2->subtree_reversed = true;

    root = Merge(treap1, treap2);
    root = Merge(root, treap3);
    return root;
}

// Remove largest priority, return tree root
Node *RemoveLargestPriority(Node *root) {
    Node *node;
    node = root;
    while (1) {
        _fix_reverse(node);
        if (node->largest_position == SELF)
            break;
        if (node->largest_position == LEFT) {
            node = node->left;
        } else {
            node = node->right;
        }
    }
    node->treap_priority = MIN_PRIORITY;
    root = sift_down(root, node);
    if (node == root) {
        free(node);
        return NULL;
    } else {
        delete_leaf_node(node);
        return root;
    }
}

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
    _fix_reverse(root);
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
    Node *root = NULL;
    int n, N, Q, i, x, y, z, ans;
    if ((n = scanf("%d %d", &N, &Q) != 2))
        ERR_EXIT("scanf");
    for (i = 0; i < N; ++i) {
        if ((n = scanf("%d", &x) != 1))
            ERR_EXIT("scanf");
        root = Insert(root, i, x);
    }
    for (i = 0; i < Q; ++i) {
        if ((n = scanf("%d", &x) != 1))
            ERR_EXIT("scanf");
        switch (x) {
            case 1:
                if ((n = scanf("%d %d", &x, &y) != 2))
                    ERR_EXIT("scanf");
                root = Insert(root, y, x);
                break;
            case 2:
                if ((n = scanf("%d", &x) != 1))
                    ERR_EXIT("scanf");
                root = Delete(root, x);
                break;
            case 3:
                if ((n = scanf("%d %d %d", &x, &y, &z) != 3))
                    ERR_EXIT("scanf");
                root = IncreasePriority(root, x, y, z);
                break;
            case 4:
                if ((n = scanf("%d %d", &x, &y) != 2))
                    ERR_EXIT("scanf");
                root = LargestPriority(root, x, y, &ans);
                printf("%d\n", ans);
                break;
            case 5:
                if ((n = scanf("%d %d", &x, &y) != 2))
                    ERR_EXIT("scanf");
                root = Reverse(root, x, y);
                break;
            case 6:
                root = RemoveLargestPriority(root);
                break;
            default:
                ERR_EXIT("Invalid operation number");
                break;
        }
    }
    FreeTreap(root);
    return 0;
}
#endif
