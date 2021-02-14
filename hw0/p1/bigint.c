#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bigint.h"

BigInt *BigInt_init(char const *str) {
    int i, len;
    char buf[BASE_LEN + 1];

    BigInt *x = malloc(sizeof(BigInt));
    x->size = 0;

    while (*str == '0')
        ++str;

    len = strlen(str);
    for (i = len - BASE_LEN; i >= 0; i -= BASE_LEN) {
        strncpy(buf, str + i, BASE_LEN);
        buf[BASE_LEN] = '\0';
        x->digits[(x->size)++] = atoi(buf);
    }

    len = i + BASE_LEN;
    if (len > 0) {
        strncpy(buf, str, len);
        buf[len] = '\0';
        x->digits[(x->size)++] = atoi(buf);
    }

    return x;
}

void BigInt_destroy(BigInt *x) { free(x); }

void BigInt_print(const BigInt *x) {
    if (x->size == 0) {
        printf("0");
        return;
    }

    int i;
    printf("%d", x->digits[x->size - 1]);
    for (i = x->size - 2; i >= 0; --i) {
        printf("%0" BASE_LEN_STR "d", x->digits[i]);
    }
    fflush(stdout);
}

bool BigInt_iszero(const BigInt *x) { return x->size == 0; }

bool BigInt_iseven(const BigInt *x) {
    return x->size == 0 || (x->digits[0] & 1) == 0;
}

int BigInt_compare(const BigInt *a, const BigInt *b) {
    if (a->size != b->size || a->size == 0)
        return a->size - b->size;

    int i;
    for (i = a->size - 1; i >= 0; --i) {
        if (a->digits[i] != b->digits[i])
            return a->digits[i] - b->digits[i];
    }
    return 0;
}

void BigInt_subtract(BigInt *a, const BigInt *b) {
    int i;
    if ((i = BigInt_compare(a, b)) < 0)
        return;
    if (i == 0) {
        a->size = 0;
        return;
    }
    for (i = 0; i < b->size; ++i) {
        a->digits[i] -= b->digits[i];
        if (a->digits[i] < 0) {
            a->digits[i] += BASE;
            --(a->digits[i + 1]);
        }
    }
    while (a->size > 0 && a->digits[a->size - 1] == 0)
        --(a->size);
}

void BigInt_div2(BigInt *x) {
    if (x->size == 0)
        return;

    int i, dividend, remainder = 0;

    if (x->digits[x->size - 1] == 1) {
        --(x->size);
        remainder = 1;
    }
    for (i = x->size - 1; i >= 0; --i) {
        dividend = x->digits[i] + BASE * remainder;
        x->digits[i] = dividend >> 1;
        remainder = dividend & 1;
    }
}

static void BigInt_mul2(BigInt *x) {
    int i;
    for (i = 0; i < x->size; ++i) {
        x->digits[i] <<= 1;
    }
    for (i = 0; i < x->size - 1; ++i) {
        if (x->digits[i] >= BASE) {
            x->digits[i] -= BASE;
            ++(x->digits[i + 1]);
        }
    }
    if (x->digits[x->size - 1] >= BASE) {
        x->digits[x->size - 1] -= BASE;
        x->digits[x->size] = 1;
        ++(x->size);
    }
}

void BigInt_mul2power(BigInt *x, int k) {
    for (int i = 0; i < k; ++i)
        BigInt_mul2(x);
}
