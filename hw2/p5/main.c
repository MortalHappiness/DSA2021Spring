/* References: None */
#include <stdio.h>
#include <stdlib.h>

#include "generator.h"

#define ERR_EXIT(msg) \
    do {              \
        perror(msg);  \
        exit(1);      \
    } while (0)

#define MAX_SIZE 3000000
#define MAX_VAL  1048575

typedef struct {
    int p;
    int q;
    int r;
} Triangle;

static int n;
static Triangle triangles[MAX_SIZE];
static int sort_q[MAX_SIZE], sort_r[MAX_SIZE];

void _swap_triangle(const int i, const int j) {
    Triangle temp = triangles[i];
    triangles[i] = triangles[j];
    triangles[j] = temp;
}

int _compare(const void *a, const void *b) { return (*(int *)a - *(int *)b); }

long cross_intersect(const int i1, const int j1, const int i2, const int j2) {
    if (i1 > j2 || i2 > j2)
        return 0L;
    int i, j;
    long ans;

    i = i1;
    j = i2;
    ans = 0L;

    while (i <= j1 && j <= j2) {
        if (sort_r[i] >= sort_q[j]) {
            ans += (long)(j1 + 1 - i);
            ++j;
        } else {
            ++i;
        }
    }
    return ans;
}

void _merge(const int i, const int j, const int left, const int right) {
    int *lists[3], *heads[3], k, idx;
    lists[0] = (int *)malloc((left - i + 1) * sizeof(int));
    lists[1] = (int *)malloc((right - left + 2) * sizeof(int));
    lists[2] = (int *)malloc((j - right + 1) * sizeof(int));

    lists[0][left - i] = MAX_VAL + 1;
    lists[1][right - left + 1] = MAX_VAL + 1;
    lists[2][j - right] = MAX_VAL + 1;

    // merge q
    for (k = i; k <= left - 1; ++k)
        lists[0][k - i] = sort_q[k];
    for (k = left; k <= right; ++k)
        lists[1][k - left] = sort_q[k];
    for (k = right + 1; k <= j; ++k)
        lists[2][k - right - 1] = sort_q[k];
    for (k = 0; k < 3; ++k)
        heads[k] = lists[k];
    for (idx = i; idx <= j; ++idx) {
        if (*heads[0] <= *heads[1] && *heads[0] <= *heads[2]) {
            sort_q[idx] = *(heads[0]++);
        } else if (*heads[1] <= *heads[2]) {
            sort_q[idx] = *(heads[1]++);
        } else {
            sort_q[idx] = *(heads[2]++);
        }
    }

    // merge r
    for (k = i; k <= left - 1; ++k)
        lists[0][k - i] = sort_r[k];
    for (k = left; k <= right; ++k)
        lists[1][k - left] = sort_r[k];
    for (k = right + 1; k <= j; ++k)
        lists[2][k - right - 1] = sort_r[k];
    for (k = 0; k < 3; ++k)
        heads[k] = lists[k];
    for (idx = i; idx <= j; ++idx) {
        if (*heads[0] <= *heads[1] && *heads[0] <= *heads[2]) {
            sort_r[idx] = *(heads[0]++);
        } else if (*heads[1] <= *heads[2]) {
            sort_r[idx] = *(heads[1]++);
        } else {
            sort_r[idx] = *(heads[2]++);
        }
    }

    free(lists[0]);
    free(lists[1]);
    free(lists[2]);
}

long num_intersect(const int i, const int j) {
    if (i > j)
        return 0L;
    if (i == j) {
        sort_q[i] = triangles[i].q;
        sort_r[i] = triangles[i].r;
        return 0L;
    }
    int left, right, pivot, k;
    long ans = 0L;

    // partition by pivot
    pivot = triangles[i].p;
    left = i;
    for (k = i; k <= j; ++k)
        if (triangles[k].p < pivot)
            _swap_triangle(k, left++);
    right = left;
    for (k = left; k <= j; ++k)
        if (triangles[k].p == pivot)
            _swap_triangle(k, right++);
    --right;

    // recursive call
    ans += num_intersect(i, left - 1) + num_intersect(right + 1, j);
    if (left < right)
        ans += ((long)(right - left + 1) * (long)(right - left)) >> 1L;
    for (k = left; k <= right; ++k) {
        sort_q[k] = triangles[k].q;
        sort_r[k] = triangles[k].r;
    }
    qsort(sort_q + left, right - left + 1, sizeof(int), _compare);
    qsort(sort_r + left, right - left + 1, sizeof(int), _compare);

    // merge
    ans += cross_intersect(i, left - 1, left, right);
    ans += cross_intersect(i, left - 1, right + 1, j);
    ans += cross_intersect(left, right, right + 1, j);
    _merge(i, j, left, right);

    return ans;
}

int main(int argc, char *argv[]) {
    generator.init();
    int t = generator.getT();
    int *p, *q, *r;
    while (t--) {
        generator.getData(&n, &p, &q, &r);
        for (int i = 0; i < n; ++i) {
            triangles[i].p = p[i];
            if (q[i] <= r[i]) {
                triangles[i].q = q[i];
                triangles[i].r = r[i];
            } else {
                triangles[i].q = r[i];
                triangles[i].r = q[i];
            }
        }

        printf("%ld\n", num_intersect(0, n - 1));
    }
    return 0;
}
