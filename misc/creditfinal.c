#include <cs50.h>
#include <stdio.h>

long get_number(void);
int check_sum(long n);
void card_name(int sum, long n);

int main(void)
{
    long n = get_number(); // Ask user for a card number

    int sum = check_sum(n); // Checksum

    card_name(sum, n); // Print Card Name / Invalid
}

long get_number(void)
{
    long n = 0;
    n = get_long("Number: ");

    return n;
}

int check_sum(long n)
{
    long o;
    long p;
    long mod1 = 100;
    long div1 = 10;
    long mod2 = 10;
    long div2 = 1;
    int s = 0; // sum

    for (int i = 0; i < 8; i++) // CHECKSUM
    {
        o = n % mod1; // To get second last digit
        o = o / div1;

        mod1 = mod1 * 100; // To get alternative digits
        div1 = div1 * 100;

        o = o * 2;

        if (o < 10) // If single digit
        {
            s = s + o;
        }
        else if (o > 9) // If double digits, converting to single digit
        {
            s = s + (o % 10) + (o / 10);
        }

        p = n % mod2; // To get last digit
        p = p / div2;

        mod2 = mod2 * 100; // To get alternative digits
        div2 = div2 * 100;

        s = s + p;
    }

    s = s % 10;

    return s;
}

void card_name(int sum, long n)
{

    if (sum == 0) // Checksum passed
    {
        long num = n / 1000000000000;

        if (num < 10 && num == 4) // 14 digit
        {
            printf("VISA\n");
        }
        else if ((num > 99 && num < 1000 && num / 10 == 34) || (num > 99 && num < 1000 && num / 10 == 37)) // 15 digit
        {
            printf("AMEX\n");
        }
        else if (num > 999 && num / 1000 == 4) // 16 digit
        {
            printf("VISA\n");
        }
        else if (num > 999 && num / 100 < 56 && num / 100 > 50) // 16 digit
        {
            printf("MASTERCARD\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else // Checksum failed
    {
        printf("INVALID\n");
    }
}