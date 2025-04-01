#include <gmp.h>
#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

uint64_t square_and_multiply(uint64_t x, uint64_t pow, uint64_t mod)
{
    uint64_t result = 1;
    x = x % mod;
    while (pow > 0)
    {
        if (pow % 2 == 1)
        {
            result = (result * x) % mod;
        }
        pow = pow / 2;
        x = ((unsigned __int128)x * x) % mod;
    }
    return result;
}

int miller_rabin(uint64_t n, int k)
{
    if (n <= 1)
        return 0;
    if (n == 2 || n == 3)
        return 1;
    if (n % 2 == 0)
        return 0;

    uint64_t d = n - 1;
    int r = 0;
    while (d % 2 == 0)
    {
        d /= 2;
        r++;
    }

    for (int i = 0; i < k; i++)
    {
        uint64_t a = 2 + ((uint64_t)rand() * rand() % (n - 3));
        uint64_t x = square_and_multiply(a, d, n);

        if (x == 1 || x == n - 1)
            continue;

        for (int j = 0; j < r - 1; j++)
        {
            x = ((unsigned __int128)x * x) % n;
            if (x == 1)
                return 0;
            if (x == n - 1)
                break;
        }
    }
    return 1;
}
uint64_t generate_prime()
{
    int k = 1000000;
    uint64_t num = ((uint64_t)rand() << 48) | ((uint64_t)rand() << 32) | ((uint64_t)rand() << 16) | rand();

    if (num % 2 == 0)
        num++;
    while (!miller_rabin(num, k))
    {
        num += 2;
    }
    return num;
}

int main()
{
    srand(time(NULL));
    int p = 23;
    int g = 5;

    int a = 6;
    int b = 15;

    uint64_t A = square_and_multiply(g, a, p);
    uint64_t B = square_and_multiply(g, b, p);

    printf("Alice:\t%d\n", a);
    printf("Bob:\t%d\n", b);

    printf("Public Alice:\t%lld\n", A);
    printf("Public Bob:\t%lld\n", B);

    uint64_t secret_Alice = square_and_multiply(B, a, p);
    uint64_t secret_Bob = square_and_multiply(A, b, p);

    printf("Alice secret:\t%lld\n", secret_Alice);
    printf("Bob secret:\t%lld\n", secret_Bob);

    if (secret_Alice == secret_Bob)
    {
        printf("La clé secrète partagée est : \t%lld\n", secret_Alice);
    }
    uint64_t n = 9222939239367149057;
    if (miller_rabin(n, 100000))
    {
        printf("%" PRIu64 " est probablement premier.\n", n);
    }
    else
    {
        printf("%llu est composé.\n", n);
    }
    return 0;
}
