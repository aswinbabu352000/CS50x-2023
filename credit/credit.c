#include <cs50.h>
#include <stdio.h>

long get_number(void);
int check_sum(long number);
void card_name(int sum, long number);

int main(void)
{
    // Prompt user for a card number
    long number = get_number();
    // Checksum
    int sum = check_sum(number);
    // Print Card Name or Invalid
    card_name(sum, number);
}

long get_number(void)
{
    long n = 0;
    n = get_long("Number: ");

    return n;
}

int check_sum(long number)
{
    // Iterating through the digits of the card number
    // loops run 8 times as the highest no. of digits is 16, so half of it will be the number of digit in both cases.

    long last;
    long second_last;
    long divide;
    long modulo;
    int sum = 0;

    // Geting alternative digits starting from second last digit
    modulo = 100;
    divide = 10;

    for (int i = 0; i < 8; i++)
    {
        second_last = (number % modulo) / divide;

        // multipying by two
        second_last *= 2;

        // double digit
        if (second_last > 9)
            sum += (second_last % 10) + (second_last / 10);

        // single digit
        else
            sum += second_last;

        modulo *= 100;
        divide *= 100;
    }

    // Geting alternative digits starting from last digit
    modulo = 10;
    divide = 1;

    for (int j = 0; j < 8; j++)
    {
        last = (number % modulo) / divide;

        sum += last;

        modulo *= 100;
        divide *= 100;
    }

    return sum % 10;
}

void card_name(int sum, long number)
{
    // Checksum passed
    if (sum == 0)
    {
        // To determine number of digits
        long n = number / 1000000000000;

        if ((n < 10 && n == 4) || (n > 999 && n / 1000 == 4))
            printf("VISA\n");
        else if ((n > 99 && n < 1000 && n / 10 == 34) || (n > 99 && n < 1000 && n / 10 == 37))
            printf("AMEX\n");
        else if (n > 999 && n / 100 < 56 && n / 100 > 50)
            printf("MASTERCARD\n");
        else
            printf("INVALID\n");
    }
    // Checksum failed
    else
        printf("INVALID\n");
}