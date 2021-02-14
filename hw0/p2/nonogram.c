#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *nums;
    int size;
} Rule;

typedef enum {
    BLANK,
    BLACK,
    WHITE,
} Cell;

static int N, M;
static Rule *row_rules, *col_rules;
static Cell **board;

bool is_valid_row(int i) {
    int n, acc, j;
    j = 0;
    for (n = 0; n < row_rules[i].size; ++n) {
        acc = 0;
        while (j < M && board[i][j] == WHITE)
            ++j;
        while (j < M && board[i][j] == BLACK) {
            ++j;
            ++acc;
        }
        if (acc != row_rules[i].nums[n])
            return false;
    }
    return true;
}

bool is_valid_col(int j) {
    int n, acc, i;
    i = 0;
    for (n = 0; n < col_rules[j].size; ++n) {
        acc = 0;
        while (i < N && board[i][j] == WHITE)
            ++i;
        while (i < N && board[i][j] == BLACK) {
            ++i;
            ++acc;
        }
        if (acc != col_rules[j].nums[n])
            return false;
    }
    return true;
}

bool is_valid() {
    int i;

    for (i = 0; i < N; ++i)
        if (!is_valid_row(i))
            return false;
    for (i = 0; i < M; ++i)
        if (!is_valid_col(i))
            return false;

    return true;
}

bool solve(int x, int y) {
    if (x == N)
        return is_valid();

    int i, j;

    // Find next cell
    i = x;
    j = y + 1;
    if (j == M) {
        ++i;
        j = 0;
    }

    board[x][y] = BLACK;
    if (solve(i, j))
        return true;
    board[x][y] = WHITE;
    if (solve(i, j))
        return true;
    board[x][y] = BLANK;
    return false;
}

int main(int argc, char *argv[]) {
    int i, j;

    // Read input
    scanf("%d%d", &N, &M);
    board = malloc(N * sizeof(Cell *));
    for (i = 0; i < N; ++i)
        board[i] = malloc(M * sizeof(Cell));

    row_rules = malloc(N * sizeof(Rule));
    col_rules = malloc(M * sizeof(Rule));

    for (i = 0; i < N; ++i) {
        scanf("%d", &row_rules[i].size);
        row_rules[i].nums = malloc(row_rules[i].size * sizeof(int));
        for (j = 0; j < row_rules[i].size; ++j)
            scanf("%d", row_rules[i].nums + j);
    }
    for (i = 0; i < M; ++i) {
        scanf("%d", &col_rules[i].size);
        col_rules[i].nums = malloc(col_rules[i].size * sizeof(int));
        for (j = 0; j < col_rules[i].size; ++j)
            scanf("%d", col_rules[i].nums + j);
    }

    // solve
    for (i = 0; i < N; ++i) {
        for (j = 0; j < M; ++j) {
            board[i][j] = BLANK;
        }
    }
    solve(0, 0);

    // print
    for (i = 0; i < N; ++i) {
        for (j = 0; j < M; ++j) {
            if (board[i][j] == BLACK) {
                printf("o");
            } else if (board[i][j] == WHITE) {
                printf("_");
            } else {
                printf("Cell (%d, %d) is blank, something wrong\n", i, j);
                return 1;
            }
        }
        printf("\n");
    }

    // free memory
    for (i = 0; i < N; ++i)
        free(board[i]);
    free(board);
    for (i = 0; i < N; ++i)
        free(row_rules[i].nums);
    for (i = 0; i < M; ++i)
        free(col_rules[i].nums);
    free(row_rules);
    free(col_rules);

    return 0;
}
