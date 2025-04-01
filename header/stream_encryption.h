#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_ULL 18446744073709551615ULL;

typedef struct LSFR
{
    int l1;
    int l2;
    int l3;
} LSFR;
void print_message_bits(char *message);
char *encrypt_decrypt_message(char *message, int session_key, int frame_counter);