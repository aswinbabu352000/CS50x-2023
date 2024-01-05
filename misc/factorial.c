#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>

int factorial(int n);

int main(int arc, string argv[])
{
    int number = 0;
    number = atoi(argv[1]);
    if (number > 0)
    {

         printf("%i\n", factorial(number));
         return 0;
    }
    else
    {
        printf("Invalid Input\n");
        return 1;
    }
}

int factorial(int n)
{
    if (n == 0)
    {
        return 1;
    }
    n = n * factorial(n - 1);
    return n;

}