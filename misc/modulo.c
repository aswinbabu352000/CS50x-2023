#include <cs50.h>
#include <stdio.h>

int main(void)
{
    long o;
    long p;
    long mod1 = 100;
    long div1 = 10;
    long mod2 = 10;
    long div2 = 1;
    int sum;

    long n = get_long("Number: "); // Ask user for a card number

    for (int i = 0; i < 8; i++) // CHECKSUM
    {
        o = n % mod1; // To get second last digit
        o = o / div1;

        mod1 = mod1 * 100; // To get alternative digits
        div1 = div1 * 100;

        o = o * 2;

        if (o < 10) // If single digit
        {
            sum = sum + o;
        }
        else if (o > 9) // If double digits, converting to single digit
        {
            sum = sum + (o % 10) + (o / 10);
        }

        p = n % mod2; // To get last digit
        p = p / div2;

        mod2 = mod2 * 100; // To get alternative digits
        div2 = div2 * 100;

        sum = sum + p;
    }
    sum = sum % 10;

    if (sum == 0) // Checksum passed
    {
        n = n / 1000000000000;

        if (n < 10 && n == 4) // 14 digit
        {
            printf("VISA\n");
        }
        else if ((n > 99 && n < 1000 && n / 10 == 34) || (n > 99 && n < 1000 && n / 10 == 37)) // 15 digit
        {
            printf("AMEX\n");
        }
        else if (n > 999 && n / 1000 == 4) // 16 digit
        {
            printf("VISA\n");
        }
        else if (n > 999 && n / 100 < 56 && n / 100 > 50) // 16 digit
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
