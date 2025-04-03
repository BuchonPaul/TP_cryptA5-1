#include <gmp.h>
#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
/**
 * @brief Calcule x^pow mod mod en utilisant l'algorithme d'exponentiation rapide "square and multiply".
 *
 * Elle calcule x^pow mod mod, où x est la base, pow est l'exposant, et mod est le modulo.
 *
 * @param result Résultat de l'exponentiation modulaire, où le résultat final sera stocké.
 * @param x La base de l'exponentiation.
 * @param pow L'exposant, un entier dont la puissance de `x` sera calculée.
 * @param mod Le modulo, utilisé pour réduire les résultats intermédiaires.
 */
void square_and_multiply(mpz_t result, mpz_t x, mpz_t pow, mpz_t mod)
{
    mpz_set_ui(result, 1);

    mpz_t base, e;
    mpz_init_set(base, x);
    mpz_init_set(e, pow);
    mpz_mod(base, base, mod);

    while (mpz_cmp_d(e, 0) > 0)
    {
        if (mpz_odd_p(e))
        {
            mpz_mul(result, result, base);
            mpz_mod(result, result, mod);
        }
        mpz_fdiv_q_2exp(e, e, 1);
        mpz_mul(base, base, base);
        mpz_mod(base, base, mod);
    }
    mpz_clears(base, e, NULL);
}
/**
 * @brief Effectue le test de primalité de Miller-Rabin sur un nombre.
 *
 * Cette fonction implémente le test probabiliste de primalité de Miller-Rabin.
 * Elle est utilisée pour déterminer si un nombre donné est probablement premier.
 * Le test est exécuté `k` fois pour une plus grande fiabilité. Si le test échoue à
 * une itération, le nombre est considéré comme composite.
 *
 * @param n Le nombre à tester pour la primalité.
 * @param k Le nombre d'itérations du test de Miller-Rabin, plus `k` est grand,
 *          plus la probabilité d'erreur est faible.
 *
 * @return 1 si `n` est probablement premier, 0 s'il est composite.
 */
int miller_rabin(mpz_t n, int k)
{
    if (mpz_cmp_ui(n, 1) <= 0)
        return 0;
    if (mpz_cmp_ui(n, 2) == 0 || mpz_cmp_ui(n, 3) == 0)
        return 1;
    if (mpz_even_p(n))
        return 0;

    mpz_t d, r;
    mpz_inits(d, r, NULL);
    mpz_sub_ui(d, n, 1);
    mpz_set_ui(r, 0);

    while (mpz_even_p(d))
    {
        mpz_fdiv_q_2exp(d, d, 1);
        mpz_add_ui(r, r, 1);
    }

    gmp_randstate_t state;
    gmp_randinit_mt(state);
    gmp_randseed_ui(state, time(NULL));

    for (int _ = 0; _ < k; _++)
    {
        mpz_t a, x, n_1;
        mpz_inits(a, x, n_1, NULL);
        mpz_sub_ui(n_1, n, 1);

        do
        {
            mpz_urandomm(a, state, n);
        } while (mpz_cmp_ui(a, 2) < 0 || mpz_cmp(a, n_1) >= 0);

        square_and_multiply(x, a, d, n);

        if (mpz_cmp_ui(x, 1) == 0 || mpz_cmp(x, n_1) == 0)
        {
            mpz_clears(a, x, n_1, NULL);
            continue;
        }

        int composite = 1;
        for (int j = 0; mpz_cmp_ui(r, j + 1) > 0; j++)
        {
            mpz_mul(x, x, x);
            mpz_mod(x, x, n);

            if (mpz_cmp(x, n_1) == 0)
                composite = 0;
            break;

            if (mpz_cmp_ui(x, 1) == 0)
                return 0;
        }
        if (composite)
            return 0;
    }
    return 1;
}
/**
 * @brief Génère un nombre premier de 512 bits.
 *
 * Cette fonction génère un nombre premier de 512 bits en utilisant un algorithme
 * probabiliste de type Miller-Rabin. Elle commence par générer un nombre aléatoire
 * et vérifie s'il est premier en appliquant le test de primalité de Miller-Rabin.
 * Si le nombre n'est pas premier, il est incrémenté jusqu'à ce qu'un nombre premier soit trouvé.
 *
 * @param prime Le nombre premier généré. Il sera stocké dans cette variable.
 */
void generate_prime(mpz_t prime)
{
    int k = 5;
    gmp_randstate_t state;
    gmp_randinit_mt(state);
    gmp_randseed_ui(state, time(NULL));

    mpz_t num;
    mpz_init(num);

    mpz_urandomb(num, state, 512);
    mpz_setbit(num, 511);
    mpz_setbit(num, 0);

    while (!miller_rabin(num, k))
    {
        mpz_add_ui(num, num, 2);
    }
    mpz_set(prime, num);
}