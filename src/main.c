#include "./stream_encryption.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
    unsigned long long int session_key = 0b0100111000101111010011010111110000011110101110001000101100111010;
    int frame_counter = 0b1110101011001111001011;

    printf("\n------------------------------------Original Message---------------------------------------\n");
    char message[] = "C'est un message crypté!";
    printf("Original message:\n\t%s\n", message);
    printf("Original binary:\n\t");
    print_message_bits(message);

    printf("\n------------------------------------Encrypt------------------------------------------------\n");
    char *encrypted_message = encrypt_decrypt_message(message, session_key, frame_counter);
    printf("Encrypted message:\n\t%s\n", encrypted_message);
    printf("Encrypted binary:\n\t");
    print_message_bits(encrypted_message);

    printf("\n------------------------------------Decrypt------------------------------------------------\n");
    char *decrypted_message = encrypt_decrypt_message(encrypted_message, session_key, frame_counter);
    printf("Decrypted message:\n\t%s\n", decrypted_message);
    return 0;
}