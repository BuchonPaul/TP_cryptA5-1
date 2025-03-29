#include <math.h>
#include <stdio.h>

int square_and_multiply(int x, int pow, int mod)
{
    int result = 1;
    x = x % mod;

    while (pow > 0)
    {
        if (pow % 2 == 1)
        {
            result = (result * x) % mod;
        }
        pow = pow / 2;
        x = (x * x) % mod;
    }
    return result;
}

int main()
{
    int p = 23;
    int g = 5;

    int a = 6;
    int b = 15;

    int A = square_and_multiply(g, a, p);
    int B = square_and_multiply(g, b, p);

    printf("Alice:\t%d\n", a);
    printf("Bob:\t%d\n", b);

    printf("Public Alice:\t%d\n", A);
    printf("Public Bob:\t%d\n", B);

    int secret_Alice = square_and_multiply(B, a, p);
    int secret_Bob = square_and_multiply(A, b, p);

    printf("Alice secret:\t%d\n", secret_Alice);
    printf("Bob secret:\t%d\n", secret_Bob);

    if (secret_Alice == secret_Bob)
    {
        printf("La clé secrète partagée est : \t%d\n", secret_Alice);
    }

    return 0;
}
