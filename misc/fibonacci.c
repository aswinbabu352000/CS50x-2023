#include <cs50.h>
#include <stdio.h>

int fibonacci(int n);

int counter = 0;

int main(void)
{
    // fibonacci of a number n is the sum of (n-1) and (n-2)            0, 1, 1, 2, 3, 5.....
    // n = (n-1) + (n=2)

    // find the nth number

    int number = get_int("Position: ");
    printf("%i\n", fibonacci(number));
}

int fibonacci(int n)
{
    if (n == 1)
        return 1;
    else if (n == 0)
        return 0;

    n = fibonacci(n - 1) + fibonacci(n - 2);
    counter += 1;

    while (counter == )
    return n;

}