/* References:
 * 1. https://leetcode.com/problems/minimum-window-substring/solution/
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERR_EXIT(msg) \
    do {              \
        perror(msg);  \
        exit(1);      \
    } while (0)

#define MAX_LEN 100000

int max(int a, int b) { return (a > b) ? a : b; }

void remove_garble_text(char *D, char *G) {
    unsigned char c;
    int i, l, r, required, formed, len_D, len_G, ans_len, ans_l, ans_r;
    int garble_counts[256], window_counts[256];

    len_D = strlen(D);
    len_G = strlen(G);

    for (i = 0; i < 256; ++i)
        garble_counts[i] = window_counts[i] = 0;

    required = 0;
    for (i = 0; i < len_G; ++i) {
        c = G[i];
        if (!garble_counts[c])
            ++required;
        ++garble_counts[c];
    }

    l = r = formed = 0;
    ans_len = MAX_LEN + 1;
    ans_l = ans_r = -1;

    while (r < len_D) {
        c = D[r];
        ++window_counts[c];

        if (garble_counts[c] && window_counts[c] == garble_counts[c])
            ++formed;

        while (l <= r && formed == required) {
            c = D[l];
            if (r - l + 1 < ans_len) {
                ans_len = r - l + 1;
                ans_l = l;
                ans_r = r;
            }
            --window_counts[c];
            if (garble_counts[c] && window_counts[c] < garble_counts[c])
                --formed;
            ++l;
        }
        ++r;
    }
    if (ans_l != -1) {
        ++ans_r;
        while (ans_r < len_D)
            D[ans_l++] = D[ans_r++];
        D[ans_l] = '\0';
    }
}

void _helper(int *table, const char *s, int l, int r) {
    if (l > r)
        return;
    if (l == r) {
        putchar(s[l]);
        return;
    }
    if (s[l] == s[r]) {
        putchar(s[l]);
        putchar('|');
        if (l + 1 <= r - 1) {
            _helper(table, s, l + 1, r - 1);
            putchar('|');
        }
        putchar(s[r]);
        return;
    }

    int i, j, k, m, t, skip, jump_l, jump_num;
    m = (l + r) >> 1;
    table[l] = 0;
    k = -1;
    j = r - 1;
    skip = 0;
    jump_l = jump_num = -1;
    for (i = l + 1; i <= m; ++i) {
        if (j <= m)
            break;
        // Compute KMP prefix function
        while (k >= 0 && s[l + k + 1] != s[i])
            k = table[l + k] - 1;
        if (s[l + k + 1] == s[i])
            ++k;
        table[i] = k + 1;

        if (skip > 0) {
            --skip;
            continue;
        }

        t = 0;
        while (s[l + t] == s[j + t]) {
            if (j + t == r) {
                for (k = 0; k <= t; ++k)
                    putchar(s[l + k]);
                putchar('|');
                if (l + t + 1 <= j - 1) {
                    _helper(table, s, l + t + 1, j - 1);
                    putchar('|');
                }
                for (k = 0; k <= t; ++k)
                    putchar(s[l + k]);
                return;
            }
            if (jump_l != -1 && j + t + 1 == jump_l) {
                t += jump_num + 1;
                jump_l = -1;
            } else {
                ++t;
            }
        }
        if (t == 0) {
            --j;
            skip = 0;
            jump_l = -1;
        } else {
            skip = max(t - table[l + t], 0);
            jump_l = j;
            jump_num = table[l + t];
            j -= (skip + 1);
        }
    }
    for (k = l; k <= r; ++k)
        putchar(s[k]);
}

void split_to_blocks(char *D) {
    int table[MAX_LEN];
    _helper(table, D, 0, strlen(D) - 1);
    putchar('\n');
}

int main(int argc, char *argv[]) {
    int n, t;
    char D[MAX_LEN + 1], G[MAX_LEN + 1];
    if ((n = scanf("%d", &t)) != 1)
        ERR_EXIT("scanf");
    while (t--) {
        if ((n = scanf("%s %s", D, G)) != 2)
            ERR_EXIT("scanf");
        remove_garble_text(D, G);
        split_to_blocks(D);
    }
    return 0;
}
