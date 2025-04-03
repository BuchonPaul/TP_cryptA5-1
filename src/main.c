#include "./diffie_helman.h"
#include "./stream_encryption.h"
#include <gmp.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    printf("\n\n--------------Set-up des données publqiues--------------\n");
    mpz_t p_512, p, g;
    mpz_inits(p_512, p, g, NULL);
    generate_prime(p_512);
    // mpz_set_str(p_512,
    //             "73354658883438629681302852888425819414799977307764540519012033294405083679929453038354"
    //             "58008138016530970254318172965357391199557120543758438497838095839321",
    //             0);
    gmp_printf("Clef Publique sur 512 bits:\t%Zd\n", p_512);

    mpz_tdiv_q_2exp(p, p_512, (512 - 64));
    gmp_printf("Clef Publique sur 64 bits (64 bis de poids fort de la clef sur 512 bits):\t%Zd\n", p);

    mpz_set_ui(g, 3);
    gmp_printf("Générateur:\t%Zd\n", g);

    printf("\n\n--------------Choix et Partage des clefs--------------\n");
    mpz_t a, b;
    mpz_inits(a, b, NULL);
    mpz_set_ui(a, 6);  // Exposant Alice
    mpz_set_ui(b, 15); // Exposant Bob

    mpz_t A, B;
    mpz_inits(A, B, NULL);

    square_and_multiply(A, g, a, p);
    square_and_multiply(B, g, b, p);

    gmp_printf("Alice:\t%Zd\n", a);
    gmp_printf("Bob:\t%Zd\n", b);

    gmp_printf("Public Alice:\t%Zd\n", A);
    gmp_printf("Public Bob:\t%Zd\n", B);

    mpz_t secret_Alice, secret_Bob;
    mpz_init(secret_Alice);
    mpz_init(secret_Bob);
    square_and_multiply(secret_Alice, B, a, p);
    square_and_multiply(secret_Bob, A, b, p);

    gmp_printf("Alice secret:\t%Zd\n", secret_Alice);
    gmp_printf("Bob secret:\t%Zd\n", secret_Bob);

    int frame_counter = 0b1110101011001111001011;

    printf("\n################################## Communications #######################################\n");
    printf("\n------------------------------------ Chez Alice ---------------------------------------\n");
    char message[] = "Salut Bob c'est Alice!";
    printf("Message d'Alice:\n\t%s\n", message);
    printf("Original binary:\n\t");
    print_message_bits(message);
    char *encrypted_message = encrypt_decrypt_message(message, mpz_get_ui(secret_Alice), frame_counter);

    printf("\n------------------------------------ Sur le réseaux (Eve) -----------------------------\n");
    printf("Eve intercepte (binaire):\n\t");
    print_message_bits(encrypted_message);
    printf("Eve intercepte:\n\t%s\n", encrypted_message);

    printf("\n------------------------------------ Chez Bob ------------------------------------------------\n");
    char *decrypted_message = encrypt_decrypt_message(encrypted_message, mpz_get_ui(secret_Bob), frame_counter);
    printf("Bob décrypte le message:\n\t%s\n\n\n", decrypted_message);
    return 0;
}
