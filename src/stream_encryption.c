#include "./stream_encryption.h"
#include <stdint.h>
#include <stdio.h>

void print_message_bits(char *message)
{
    while (*message)
    {
        for (int i = 7; i >= 0; i--)
        {
            putchar((*message & (1 << i)) ? '1' : '0');
        }
        putchar(' ');
        message++;
    }
    putchar('\n');
}

short i_bit(int num, int position)
{
    return (num >> position) & 1;
}

short llu_bit(long long num, int position)
{
    return (num >> position) & 1;
}

short xor_l1(int lsfr)
{
    return i_bit(lsfr, 18) ^ i_bit(lsfr, 17) ^ i_bit(lsfr, 16) ^ i_bit(lsfr, 13);
}
short xor_l2(int lsfr)
{
    return i_bit(lsfr, 20) ^ i_bit(lsfr, 21);
}
short xor_l3(int lsfr)
{
    return i_bit(lsfr, 7) ^ i_bit(lsfr, 20) ^ i_bit(lsfr, 21) ^ i_bit(lsfr, 22);
}
/**
 * @brief Initialise les registres de décalage pour le chiffrement A5/1.
 *
 * Cette fonction initialise trois registres à décalage (L1, L2 et L3) pour
 * le protocole de chiffrement A5/1 en fonction de la clé de session et du
 * compteur de trame. Elle charge la clé de session (64 bits) et le compteur de trame
 * (22 bits), puis exécute 100 cycles d'initialisation avec un critère de majorité
 * pour synchroniser les registres.
 *
 * @param L1 Le registre de décalage 1.
 * @param L2 Le registre de décalage 2.
 * @param L3 Le registre de décalage 3.
 * @param key La clé de session (64 bits).
 * @param frame_counter Le compteur de trame (22 bits).
 */
LSFR initialize_registers(long long session_key, int frame_counter)
{
    LSFR l = {
        .l1 = 0,
        .l2 = 0,
        .l3 = 0,
    };
    for (int i = 63; i >= 0; i--)
    {
        short sk_b = llu_bit(session_key, i - 1);
        l.l1 = (l.l1 << 1) | (sk_b ^ xor_l1(l.l1));
        l.l2 = (l.l2 << 1) | (sk_b ^ xor_l2(l.l2));
        l.l3 = (l.l3 << 1) | (sk_b ^ xor_l3(l.l3));
    }
    for (int i = 21; i >= 0; i--)
    {
        short fc_b = llu_bit(frame_counter, i - 1);
        l.l1 = (l.l1 << 1) | (fc_b ^ xor_l1(l.l1));
        l.l2 = (l.l2 << 1) | (fc_b ^ xor_l2(l.l2));
        l.l3 = (l.l3 << 1) | (fc_b ^ xor_l3(l.l3));
    }
    for (int i = 100; i > 0; i--)
    {
        int maj = (i_bit(l.l1, 8) + i_bit(l.l2, 10) + i_bit(l.l3, 10)) >= 2 ? 1 : 0;
        if (i_bit(l.l1, 8) == maj)
            l.l1 = (l.l1 << 1) | xor_l1(l.l1);
        if (i_bit(l.l2, 8) == maj)
            l.l2 = (l.l2 << 1) | xor_l2(l.l2);
        if (i_bit(l.l3, 8) == maj)
            l.l3 = (l.l3 << 1) | xor_l3(l.l3);
    }
    return l;
}
/**
 * @brief Effectue un chiffrement/déchiffrement du message avec XOR.
 *
 * Cette fonction chiffre ou déchiffre un message en effectuant un XOR entre chaque
 * caractère du message et un buffer de chiffrement. Elle génère un buffer à partir
 * des bits produits par les registres de décalage, puis applique le XOR pour modifier
 * le message d'entrée. Le processus peut être inversé pour déchiffrer le message
 * avec la même clé et les mêmes paramètres.
 *
 * @param message Le message à chiffrer ou déchiffrer.
 * @param key La clé de session utilisée pour générer le buffer de chiffrement.
 * @param frame_counter Le compteur de trame, utilisé pour la synchronisation des registres.
 *
 * @return Le message chiffré ou déchiffré, sous forme de chaîne de caractères.
 */
char *encrypt_decrypt_message(char *message, int session_key, int frame_counter)
{
    int length = 0;
    while (message[length] != '\0')
        length++;

    char *xor_msg = malloc(length + 1);
    if (!xor_msg)
        return NULL;

    for (int i = 0; i < length; i++)
    {
        LSFR lsfr = initialize_registers(session_key, frame_counter++);
        char buffer = 0;
        for (int j = 0; j < 8; j++)
        {
            int maj = (i_bit(lsfr.l1, 8) + i_bit(lsfr.l2, 10) + i_bit(lsfr.l3, 10)) >= 2 ? 1 : 0;
            if (i_bit(lsfr.l1, 8) == maj)
                lsfr.l1 = (lsfr.l1 << 1) | xor_l1(lsfr.l1);
            if (i_bit(lsfr.l2, 10) == maj)
                lsfr.l2 = (lsfr.l2 << 1) | xor_l1(lsfr.l2);
            if (i_bit(lsfr.l3, 10) == maj)
                lsfr.l3 = (lsfr.l3 << 1) | xor_l1(lsfr.l3);

            if (xor_l1(lsfr.l1) ^ xor_l1(lsfr.l2) ^ xor_l1(lsfr.l3) == 0)
                buffer &= ~(1 << j); // Add 0
            else
                buffer |= (1 << j); // ADd 1
        }
        xor_msg[i] = message[i] ^ buffer;
    }
    xor_msg[length] = '\0';
    return xor_msg;
}