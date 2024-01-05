#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>

int collatz(int n);

int main(int argc, string argv[])
{
    int number = atoi(argv[1]);
    printf("Steps: %i\n",collatz(number));
}

int collatz(n)
{
    if(n == 1)
        return 0;

    else if(n % 2 == 0)
       return 1 + collatz(n / 2);

    else
        return 1 + collatz(3 * n + 1);
}