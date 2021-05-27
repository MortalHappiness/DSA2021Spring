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

typedef struct Node {
    struct Node *parent;
    struct Node *left;
    struct Node *right;
    int treap_priority;

    // The following attributes are not related to basic treap implementation
    int diff_priority;
    int subtree_size;
    bool subtree_reversed;
    int largest_priority;
    Position largest_position;
} Node;

// ========================================

int add(int i, int j) { return i + j; }

// ========================================

#ifndef __main__
#define __main__
int main(int argc, char *argv[]) {
    printf("%d\n", add(1, 1));
    return 0;
}
#endif
