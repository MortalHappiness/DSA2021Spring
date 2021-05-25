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

void split_to_blocks(char *D) { printf("%s\n", D); }

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
