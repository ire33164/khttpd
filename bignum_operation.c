#include "bignum_operation.h"

void bignum_add(bignum *a, bignum *b, bignum *result)
{
    memset(result, 0, sizeof(bignum));
    long long carry = 0;
    int i;
    for (i = 0; i < PART_NUM; ++i) {
        unsigned long long tmp = a->part[i] + b->part[i] + carry;
        result->part[i] = tmp % BASE;
        carry = tmp / BASE;
    }
}

void bignum_sub(bignum *a, bignum *b, bignum *result)
{
    memset(result, 0, sizeof(bignum));
    long long borrow = 0;
    int i;
    for (i = 0; i < PART_NUM; ++i) {
        long long tmp = a->part[i] - b->part[i] - borrow;
        if (tmp < 0) {
            result->part[i] = tmp + BASE;
            borrow = 1;
        } else {
            result->part[i] = tmp;
            borrow = 0;
        }
    }
}
void bignum_mul(bignum *a, bignum *b, bignum *result)
{
    memset(result, 0, sizeof(bignum));
    int i, j;
    for (i = 0; i < PART_NUM; ++i) {
        result->part[i] = 0;
    }
    for (i = 0; i < PART_NUM; ++i) {
        long long carry = 0;
        for (j = 0; i + j < PART_NUM; ++j) {
            long long tmp =
                a->part[i] * b->part[j] + carry + result->part[i + j];
            result->part[i + j] = tmp % BASE;
            carry = tmp / BASE;
        }
    }
}

void bignum_assign(bignum *dest, bignum *src)
{
    memset(dest, 0, sizeof(bignum));
    int i;
    for (i = 0; i < PART_NUM; ++i) {
        dest->part[i] = src->part[i];
    }
}

int zero_num(long long n)
{
    if (n == 0)
        return 7;
    int count = 0;
    while (n != 0) {
        count++;
        n /= 10;
    }
    return 8 - count;
}

void bignum2str(bignum *bn, char *dest)
{
    int valid_part = 0;
    int i, k;
    for (i = PART_NUM - 1; i >= 0; --i) {
        if (bn->part[i] != 0) {
            valid_part = i;
            break;
        }
    }
    snprintf(dest, MAX_DIGIT, "%lld", bn->part[valid_part]);
    for (i = valid_part - 1; i >= 0; --i) {
        int zeros = zero_num(bn->part[i]);
        for (k = 0; k < zeros; ++k) {
            snprintf(dest, MAX_DIGIT, "%s0", dest);
        }
        snprintf(dest, MAX_DIGIT, "%s%lld", dest, bn->part[i]);
    }
}

bignum fib_eval(long long k)
{
    /* The highest bit of k */
    long long h = 32 - __builtin_clz(k);
    int i;
    bignum f[7];
    for (i = 0; i < 7; ++i)
        memset(&f[i], 0, sizeof(bignum));

    /* init F(1) = 1 */
    f[1].part[0] = 1;
    /* set f[4] = 2 */
    f[4].part[0] = 2;

    long long mask;
    for (mask = 1 << (h - 1); mask; mask >>= 1) {
        bignum_mul(&f[4], &f[1], &f[5]);  // 2 * b
        bignum_sub(&f[5], &f[0], &f[6]);  // 2 * b - a
        bignum_mul(&f[0], &f[6], &f[2]);  // c = a * (2 * b - a)

        bignum_mul(&f[0], &f[0], &f[5]);  // a * a
        bignum_mul(&f[1], &f[1], &f[6]);  // b * b
        bignum_add(&f[5], &f[6], &f[3]);  // d = a * a + b * b

        if (mask & k) {
            bignum_assign(&f[0], &f[3]);      // a = d
            bignum_add(&f[2], &f[3], &f[1]);  // b = c + d
        } else {
            bignum_assign(&f[0], &f[2]);  // a = c
            bignum_assign(&f[1], &f[3]);  // b = d
        }
    }
    pr_info("r = %lld\n", f[0].part[0]);

    return f[0];
}
