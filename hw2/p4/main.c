/* References: None */
#include <stdio.h>
#include <stdlib.h>

#define ERR_EXIT(msg) \
    do {              \
        perror(msg);  \
        exit(1);      \
    } while (0)

#define MAX_N   1000000
#define MIN_KEY 1
#define MAX_KEY 1000000000

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

typedef struct {
    int key;
    int left;
    int right;
} Node;

Node tree[MAX_N + 1];

int num_findable(int idx, int cur_min, int cur_max) {
    if (idx == -1)
        return 0;
    if (cur_min > cur_max)
        return 0;
    Node node = tree[idx];
    int ans = (node.key >= cur_min && node.key <= cur_max) ? 1 : 0;
    ans += num_findable(node.left, cur_min, MIN(node.key, cur_max));
    ans += num_findable(node.right, MAX(node.key, cur_min), cur_max);
    return ans;
}

int main(int argc, char *argv[]) {
    int i, n, N;
    if ((n = scanf("%d", &N)) != 1)
        ERR_EXIT("scanf");

    // One-indexed
    for (i = 1; i <= N; ++i) {
        if ((n = scanf("%d %d %d", &(tree[i].key), &(tree[i].left),
                       &(tree[i].right))) != 3)
            ERR_EXIT("scanf");
    }

    n = num_findable(1, MIN_KEY, MAX_KEY);
    printf("%d\n", n);

    return 0;
}
