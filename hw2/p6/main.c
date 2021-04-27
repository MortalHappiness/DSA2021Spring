/* References: None */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define ERR_EXIT(msg) \
    do {              \
        perror(msg);  \
        exit(1);      \
    } while (0)

#define MAX_VAL 100000

#define PUSH  0
#define MERGE 1

// ========================================

typedef struct {
    int op;
    int a;
    int b;
} Operation;

static int N, O, L;
Operation operations[MAX_VAL << 1];
int sequence[MAX_VAL];

bool sequence_possible() { return false; }

// ========================================

int main(int argc, char *argv[]) {
    int n, t, i;
    char buf[8];
    if ((n = scanf("%d", &t)) != 1)
        ERR_EXIT("scanf");
    while (t--) {
        if ((n = scanf("%d %d %d", &N, &O, &L)) != 3)
            ERR_EXIT("scanf");
        for (i = 0; i < O; ++i) {
            if ((n = scanf("%s %d %d", buf, &(operations[i].a),
                           &(operations[i].b))) != 3)
                ERR_EXIT("scanf");
            operations[i].op = (buf[0] == 'p') ? PUSH : MERGE;
        }
        for (i = 0; i < N; ++i) {
            if ((n = scanf("%d", sequence + i)) != 1)
                ERR_EXIT("scanf");
        }
        if (sequence_possible()) {
            printf("possible\n");
        } else {
            printf("impossible\n");
        }
    }
    return 0;
}
