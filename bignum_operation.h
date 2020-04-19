#include <linux/kernel.h>
#include <linux/string.h>

#define MAX_LENGTH 92
#define PART_NUM 8
#define BASE 100000000
#define MAX_DIGIT 65

typedef struct {
    unsigned long long part[PART_NUM];
} bignum;

void bignum_add(bignum *a, bignum *b, bignum *result);
void bignum_sub(bignum *a, bignum *b, bignum *result);
void bignum_mul(bignum *a, bignum *b, bignum *result);
void bignum_assign(bignum *dest, bignum *src);
int zero_num(long long n);
void bignum2str(bignum *bn, char *dest);
bignum fib_eval(long long k);
