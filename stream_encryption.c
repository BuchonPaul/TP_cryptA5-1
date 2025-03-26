#include <stdint.h>
#include <stdio.h>

#define MAX_ULL 18446744073709551615ULL;

typedef struct LSFR
{
    int l1;
    int l2;
    int l3;
} LSFR;

void print_binary(unsigned long long int n, int size)
{
    for (int i = size - 1; i >= 0; i--)
    {
        printf("%llu%c", (n >> i) & 1, i % 4 == 0 ? ' ' : '\0');
    }
    printf("");
}

void print_lsfr(int n, char lsfr_id, int size)
{
    // int delta = 32 - size;
    printf("LSFR_%d (%d bits):\t", lsfr_id, size);
    for (int i = 0; i < size; i++)
    {
        printf("%d%c", (n >> i) & 1, (i + 1) % 4 == 0 ? ' ' : '\0');
    }
    printf("\n");
}

short i_bit(int num, int position)
{
    return (num >> position) & 1;
}

short llu_bit(unsigned long long int num, int position)
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

LSFR initialize_registers(unsigned long long session_key, int frame_counter)
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

int main()
{
    unsigned long long int session_key = 0b0100111000101111010011010111110000011110101110001000101100111010;
    int frame_counter = 0b1110101011001111001011;
    LSFR lsfr = initialize_registers(session_key, frame_counter);

    char message[] = "salut cest moi";
    int length = 0;
    while (message[length] != '\0')
        length++;
    printf("\nOriginal message:\n\t%s\n", message);
    printf("Binary Original message :\n\t");
    for (int i = 0; i < length; i++)
    {
        print_binary(message[i], 8);
    }
    char encrypted_message[length];
    for (int i = 0; i < length; i++)
    {
        int maj = (i_bit(lsfr.l1, 8) + i_bit(lsfr.l2, 10) + i_bit(lsfr.l3, 10)) >= 2 ? 1 : 0;

        if (i_bit(lsfr.l1, 8) == maj)
            lsfr.l1 = (lsfr.l1 << 1) | xor_l1(lsfr.l1);
        if (i_bit(lsfr.l2, 10) == maj)
            lsfr.l2 = (lsfr.l2 << 1) | xor_l1(lsfr.l2);
        if (i_bit(lsfr.l3, 10) == maj)
            lsfr.l3 = (lsfr.l3 << 1) | xor_l1(lsfr.l3);

        encrypted_message[i] = message[i] ^ (i_bit(lsfr.l1, 18) ^ i_bit(lsfr.l2, 21) ^ i_bit(lsfr.l3, 22));
    }

    printf("\nCiffer Binary Original message:\n\t");
    for (int i = 0; i < length; i++)
    {
        print_binary(encrypted_message[i], 8);
    }
    return 0;
}