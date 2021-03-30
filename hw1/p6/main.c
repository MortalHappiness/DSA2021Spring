/* References: None */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define ERR_EXIT(msg) \
    do {              \
        perror(msg);  \
        exit(1);      \
    } while (0)

// ========================================

#define MAX_N          50000
#define MAX_VAL        100000
#define MIN_VAL        -100000
#define BUCKET_BITSIZE 9 // TODO: Tune this
#define BUCKET_SIZE    (1 << BUCKET_BITSIZE)

// ========================================
// Helper functions

void _swap(int *array, int i, int j) {
    int temp = array[i];
    array[i] = array[j];
    array[j] = temp;
}

int _compare(const void *a, const void *b) { return (*(int *)a - *(int *)b); }

void _array_print(int *array, int count) {
    printf("{");
    if (count >= 1) {
        printf("%d", array[0]);
    }
    for (int i = 1; i < count; ++i) {
        printf(", %d", array[i]);
    }
    printf("}");
}

void _array_insert(int *array, int size, int idx, int x) {
    for (int i = size - 1; i >= idx; --i)
        array[i + 1] = array[i];
    array[idx] = x;
}

void _array_delete(int *array, int size, int idx) {
    for (int i = idx; i < size; ++i) {
        array[i] = array[i + 1];
    }
}

void _array_reverse(int *array, int i, int j) {
    while (i < j) {
        _swap(array, i, j);
        ++i;
        --j;
    }
}

int _binary_search(int *array, int l, int r, int num) {
    int mid;
    while (l < r) {
        mid = (l + r) >> 1;
        if (array[mid] >= num) {
            r = mid;
        } else {
            l = mid + 1;
        }
    }
    return l;
}

void _nums_less_equal(int *array, int l, int r, int num, int *less,
                      int *less_equal) {
    *less = *less_equal = 0;
    for (int i = l; i <= r; ++i) {
        if (array[i] < num) {
            ++(*less);
            ++(*less_equal);
        }
        if (array[i] == num) {
            ++(*less_equal);
        }
    }
}

// ========================================

typedef struct Bucket {
    struct Bucket *next;
    struct Bucket *prev;
    int items[BUCKET_SIZE << 1];
    int sorted_items[BUCKET_SIZE << 1];
    int size;
    bool reversed;
} Bucket;

Bucket *BucketCreate() {
    Bucket *bucket = (Bucket *)malloc(sizeof(Bucket));
    bucket->next = NULL;
    bucket->prev = NULL;
    bucket->size = 0;
    bucket->reversed = false;
    return bucket;
}

Bucket *BucketCreateFromNums(int *nums, int count) {
    Bucket *head, *node, *temp;
    head = BucketCreate(); // sentinel node
    node = BucketCreate();
    head->prev = head->next = node;
    node->prev = node->next = head;
    head->size = BUCKET_SIZE << 1;
    for (int i = 0; i < count; ++i) {
        node->items[node->size] = node->sorted_items[node->size] = nums[i];
        ++(node->size);
        if (node->size == BUCKET_SIZE) {
            qsort(node->sorted_items, node->size, sizeof(int), _compare);
            temp = BucketCreate();
            temp->prev = node;
            temp->next = node->next;
            temp->prev->next = temp;
            temp->next->prev = temp;
            node = temp;
        }
    }
    qsort(node->sorted_items, node->size, sizeof(int), _compare);
    return head;
}

void BucketFree(Bucket *head) {
    Bucket *node = head, *next;
    do {
        next = node->next;
        free(node);
        node = next;
    } while (node != head);
}

void BucketPrint(Bucket *head) {
    Bucket *node = head->next;
    printf("\n");
    if (node == head) {
        printf("Empty\n");
        return;
    }
    do {
        printf("Bucket:\n");
        printf("  items: ");
        _array_print(node->items, node->size);
        printf("\n");
        printf("  sorted_items: ");
        _array_print(node->sorted_items, node->size);
        printf("\n");
        printf("  reversed: %d\n", node->reversed);
        node = node->next;
    } while (node != head);
    printf("\n");
}

Bucket *BucketFind(Bucket *head, int *idx) {
    Bucket *node = head->next;
    if (head->next == head) {
        node = BucketCreate();
        head->prev = head->next = node;
        node->prev = node->next = head;
        return node;
    }
    node = head->next;
    while (*idx >= node->size) {
        *idx -= node->size;
        node = node->next;
    }
    if (node == head) {
        *idx = head->prev->size;
        return head->prev;
    }
    return node;
}

/* Split bucket into two bucket, the size of the first bucket is size */
void BucketSplit(Bucket *bucket, int size) {
    if (bucket->reversed) {
        _array_reverse(bucket->items, 0, bucket->size - 1);
        bucket->reversed = false;
    }
    Bucket *new_bucket = BucketCreate();
    new_bucket->prev = bucket;
    new_bucket->next = bucket->next;
    new_bucket->prev->next = new_bucket;
    new_bucket->next->prev = new_bucket;
    for (int i = size; i < bucket->size; ++i)
        new_bucket->sorted_items[i - size] = new_bucket->items[i - size] =
            bucket->items[i];
    for (int i = 0; i < size; ++i)
        bucket->sorted_items[i] = bucket->items[i];
    new_bucket->size = bucket->size - size;
    bucket->size = size;
    qsort(bucket->sorted_items, bucket->size, sizeof(int), _compare);
    qsort(new_bucket->sorted_items, new_bucket->size, sizeof(int), _compare);
}

/* Merge current bucket and next bucket */
void BucketMerge(Bucket *bucket) {
    Bucket *next = bucket->next;
    if (bucket->reversed) {
        _array_reverse(bucket->items, 0, bucket->size - 1);
        bucket->reversed = false;
    }
    if (next->reversed) {
        _array_reverse(next->items, 0, next->size - 1);
        next->reversed = false;
    }
    for (int i = 0; i < next->size; ++i) {
        bucket->sorted_items[bucket->size] = bucket->items[bucket->size] =
            next->items[i];
        ++(bucket->size);
    }
    qsort(bucket->sorted_items, bucket->size, sizeof(int), _compare);
    next->prev->next = next->next;
    next->next->prev = next->prev;
    free(next);
}

void BucketReverseNodes(Bucket *left, Bucket *right) {
    Bucket *left_prev, *left_next, *right_prev, *right_next;
    while (1) {
        if (left == right) {
            left->reversed = !(left->reversed);
            break;
        }
        left->reversed = !(left->reversed);
        right->reversed = !(right->reversed);
        if (left->next == right) {
            left->next = right->next;
            right->next->prev = left;
            right->prev = left->prev;
            left->prev->next = right;
            left->prev = right;
            right->next = left;
            break;
        }
        left_prev = left->prev;
        left_next = left->next;
        right_prev = right->prev;
        right_next = right->next;

        // swap left and right
        left->prev = right_prev;
        left->next = right_next;
        right->prev = left_prev;
        right->next = left_next;
        left->prev->next = left;
        left->next->prev = left;
        right->prev->next = right;
        right->next->prev = right;

        left = left_next;
        right = right_prev;
    }
}

/* Find how many numbers are less than, less than or equal to "num" */
void BucketQuery(Bucket *head, int l, int r, int num, int *less,
                 int *less_equal) {
    Bucket *node, *left_node, *right_node;
    left_node = BucketFind(head, &l);
    right_node = BucketFind(head, &r);
    int i, less_temp, less_equal_temp;
    if (left_node->reversed) {
        _array_reverse(left_node->items, 0, left_node->size - 1);
        left_node->reversed = false;
    }
    if (right_node->reversed) {
        _array_reverse(right_node->items, 0, right_node->size - 1);
        right_node->reversed = false;
    }
    if (left_node == right_node) {
        _nums_less_equal(left_node->items, l, r, num, less, less_equal);
        return;
    }
    *less = *less_equal = 0;

    _nums_less_equal(left_node->items, l, left_node->size - 1, num, &less_temp,
                     &less_equal_temp);
    *less += less_temp;
    *less_equal += less_equal_temp;
    _nums_less_equal(right_node->items, 0, r, num, &less_temp,
                     &less_equal_temp);
    *less += less_temp;
    *less_equal += less_equal_temp;

    node = left_node->next;
    while (node != right_node) {
        i = _binary_search(node->sorted_items, 0, node->size, num);
        *less += i;
        i = _binary_search(node->sorted_items, 0, node->size, num + 1);
        *less_equal += i;
        node = node->next;
    }
}

// ========================================

void _merge(Bucket *node) {
    if (node->prev->size + node->size <= BUCKET_SIZE) {
        node = node->prev;
        BucketMerge(node);
    }
    if (node->next->size + node->size <= BUCKET_SIZE) {
        BucketMerge(node);
    }
}

void _insert(Bucket *head, int idx, int x) {
    Bucket *node;
    node = BucketFind(head, &idx);
    if (node->reversed) {
        _array_reverse(node->items, 0, node->size - 1);
        node->reversed = false;
    }
    _array_insert(node->items, node->size, idx, x);
    int i = _binary_search(node->sorted_items, 0, node->size, x);
    _array_insert(node->sorted_items, node->size, i, x);
    ++(node->size);
    if (node->size == BUCKET_SIZE << 1)
        BucketSplit(node, BUCKET_SIZE);
}

void _delete(Bucket *head, int idx) {
    Bucket *node = BucketFind(head, &idx);
    if (node->reversed) {
        _array_reverse(node->items, 0, node->size - 1);
        node->reversed = false;
    }
    int x = node->items[idx];
    _array_delete(node->items, node->size, idx);
    int i = _binary_search(node->sorted_items, 0, node->size, x);
    _array_delete(node->sorted_items, node->size, i);
    --(node->size);
    if (node->size == 0) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
        free(node);
    } else {
        _merge(node);
    }
}

void _reverse(Bucket *head, int l, int r) {
    Bucket *left_node = BucketFind(head, &l);
    Bucket *right_node = BucketFind(head, &r);
    if (left_node->reversed) {
        _array_reverse(left_node->items, 0, left_node->size - 1);
        left_node->reversed = false;
    }
    if (right_node->reversed) {
        _array_reverse(right_node->items, 0, right_node->size - 1);
        right_node->reversed = false;
    }
    if (left_node == right_node) {
        _array_reverse(left_node->items, l, r);
        return;
    }
    if (l != 0) {
        BucketSplit(left_node, l);
        left_node = left_node->next;
    }
    if (r != right_node->size - 1) {
        BucketSplit(right_node, r + 1);
    }
    BucketReverseNodes(left_node, right_node);
    _merge(left_node);
    _merge(right_node);
}

void _query(Bucket *head, int l, int r, int k) {
    int lower, upper, mid, less, less_equal;
    lower = MIN_VAL;
    upper = MAX_VAL + 1;
    while (lower < upper) {
        mid = (lower + upper) >> 1;
        BucketQuery(head, l, r, mid, &less, &less_equal);
        if (less < k && less_equal >= k) {
            printf("%d\n", mid);
            return;
        }
        if (less >= k) {
            upper = mid;
        } else {
            lower = mid + 1;
        }
    }
    ERR_EXIT("Cannot find");
}

// ========================================

int nums[MAX_N];
int N = 0;

// ========================================

int main(int argc, char *argv[]) {
    int i, q, num, l, r, x, idx;
    char buf[8];
    Bucket *head;

    if ((num = scanf("%d%d", &N, &q)) != 2)
        ERR_EXIT("scanf");
    for (i = 0; i < N; ++i) {
        if ((num = scanf("%d", nums + i)) != 1)
            ERR_EXIT("scanf");
    }

    head = BucketCreateFromNums(nums, N);

    for (i = 0; i < q; ++i) {
        if ((num = scanf("%8s", buf)) != 1)
            ERR_EXIT("scanf");
        switch (buf[0]) {
            case 'I': // Insert
                if ((num = scanf("%d%d", &idx, &x)) != 2)
                    ERR_EXIT("scanf");
                _insert(head, idx - 1, x);
                break;
            case 'D': // Delete
                if ((num = scanf("%d", &idx)) != 1)
                    ERR_EXIT("scanf");
                _delete(head, idx - 1);
                break;
            case 'R': // Reverse
                if ((num = scanf("%d%d", &l, &r)) != 2)
                    ERR_EXIT("scanf");
                _reverse(head, l - 1, r - 1);
                break;
            case 'Q': // Query
                if ((num = scanf("%d%d%d", &l, &r, &x)) != 3)
                    ERR_EXIT("scanf");
                _query(head, l - 1, r - 1, x);
                break;
            default:
                ERR_EXIT("Invalid input");
        }
    }
    BucketFree(head);
    return 0;
}
