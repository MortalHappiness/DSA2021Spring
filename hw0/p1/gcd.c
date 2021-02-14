#include <stdio.h>

#include "bigint.h"

int main(int argc, char *argv[]) {
    char buf[257];
    BigInt *n, *m, *temp;
    int k;

    scanf("%256s", buf);
    n = BigInt_init(buf);
    scanf("%256s", buf);
    m = BigInt_init(buf);
    k = 0;

    if (BigInt_compare(n, m) > 0) {
        temp = n;
        n = m;
        m = temp;
    }

    while (!BigInt_iszero(n) && !BigInt_iszero(m)) {
        if (BigInt_iseven(n) && BigInt_iseven(m)) {
            ++k;
            BigInt_div2(n);
            BigInt_div2(m);
        } else if (BigInt_iseven(n)) {
            BigInt_div2(n);
        } else if (BigInt_iseven(m)) {
            BigInt_div2(m);
        }
        if (BigInt_compare(n, m) > 0) {
            temp = n;
            n = m;
            m = temp;
        }
        BigInt_subtract(m, n);
    }

    BigInt_mul2power(n, k);
    BigInt_print(n);

    BigInt_destroy(n);
    BigInt_destroy(m);

    return 0;
}
