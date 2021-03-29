/* References: None */

#include <stdio.h>
#include <stdlib.h>

#define ERR_EXIT(msg) \
    do {              \
        perror(msg);  \
        exit(1);      \
    } while (0)

// ========================================

#define MAX_SIZE 100000
int array[MAX_SIZE];
int aux[MAX_SIZE];
int size = 0;

// ========================================

int _random_int(int l, int r) {
    double a = (double)rand() / RAND_MAX;
    return (r - l + 1) * a + l;
}

void _swap(int *arr, int i, int j) {
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

int _partition(int *arr, int l, int r) {
    int x, i, j;
    i = _random_int(l, r);
    _swap(arr, i, r);

    x = arr[r];
    i = l;
    for (j = l; j < r; ++j) {
        if (arr[j] <= x) {
            _swap(arr, i, j);
            ++i;
        }
    }
    _swap(arr, i, r);
    return i;
}

int _kth_smallest(int *arr, int l, int r, int k) {
    int idx;
    if (k > 0 && k <= r - l + 1) {
        idx = _partition(arr, l, r);
        if (idx - l == k - 1) {
            return arr[idx];
        }
        if (idx - l > k - 1) {
            return _kth_smallest(arr, l, idx - 1, k);
        }
        return _kth_smallest(arr, idx + 1, r, k - idx + l - 1);
    }
    ERR_EXIT("Invalid arguments");
}

void _insert(int idx, int x) {
    int i = size - 1;
    --idx;
    while (i >= idx) {
        array[i + 1] = array[i];
        --i;
    }
    array[idx] = x;
    ++size;
}

void _delete(int idx) {
    int i = idx - 1;
    while (i < size) {
        array[i] = array[i + 1];
        ++i;
    }
    --size;
}

void _reverse(int l, int r) {
    int i, j, temp;
    i = l - 1;
    j = r - 1;
    while (i < j) {
        temp = array[i];
        array[i] = array[j];
        array[j] = temp;
        ++i;
        --j;
    }
}

void _query(int l, int r, int k) {
    int i, n;
    n = r - l + 1;
    for (i = 0; i < n; ++i)
        aux[i] = array[l + i - 1];
    int x = _kth_smallest(aux, 0, n - 1, k);
    printf("%d\n", x);
}

// ========================================

int main(int argc, char *argv[]) {
    int i, n, q, num, l, r, x, idx;
    char buf[8];

    srand(12345);

    if ((num = scanf("%d%d", &n, &q)) != 2)
        ERR_EXIT("scanf");
    size = n;
    for (i = 0; i < n; ++i) {
        if ((num = scanf("%d", array + i)) != 1)
            ERR_EXIT("scanf");
    }
    for (i = 0; i < q; ++i) {
        if ((num = scanf("%8s", buf)) != 1)
            ERR_EXIT("scanf");
        switch (buf[0]) {
            case 'I': // Insert
                if ((num = scanf("%d%d", &idx, &x)) != 2)
                    ERR_EXIT("scanf");
                _insert(idx, x);
                break;
            case 'D': // Delete
                if ((num = scanf("%d", &idx)) != 1)
                    ERR_EXIT("scanf");
                _delete(idx);
                break;
            case 'R': // Reverse
                if ((num = scanf("%d%d", &l, &r)) != 2)
                    ERR_EXIT("scanf");
                _reverse(l, r);
                break;
            case 'Q': // Query
                if ((num = scanf("%d%d%d", &l, &r, &x)) != 3)
                    ERR_EXIT("scanf");
                _query(l, r, x);
                break;
            default:
                ERR_EXIT("Invalid input");
        }
    }
    return 0;
}
