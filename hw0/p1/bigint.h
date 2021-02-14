#ifndef _BIGINT_H_
#define _BIGINT_H_

#include <stdbool.h>

#define N_DIGIT      32
#define BASE         100000000 // 10**8
#define BASE_LEN     8
#define BASE_LEN_STR "8"

/* BigInt struct, represent [0, 10**256) */
typedef struct {
    int digits[N_DIGIT];
    int size; // how many digits are inuse
} BigInt;

/* Initialize a BigInt from string */
BigInt *BigInt_init(char const *str);

/* Destroy a BigInt */
void BigInt_destroy(BigInt *x);

/* Print a BigInt */
void BigInt_print(const BigInt *x);

/* Returns true if x = 0, false otherwise */
bool BigInt_iszero(const BigInt *x);

/* Returns true if x is even, false otherwise */
bool BigInt_iseven(const BigInt *x);

/* Returns negative if a < b, positive if a > b, 0 if a = b */
int BigInt_compare(const BigInt *a, const BigInt *b);

/* a -= b if a > b, otherwise nothing happens */
void BigInt_subtract(BigInt *a, const BigInt *b);

/* x //= 2 */
void BigInt_div2(BigInt *x);

/* x *= 2**k */
void BigInt_mul2power(BigInt *x, int k);

#endif /* _BIGINT_H_ */
