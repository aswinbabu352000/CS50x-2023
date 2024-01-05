#include <cs50.h>
#include <stdio.h>

int get_negative_int();

int main(void)
{
    int i = get_negative_int();
    printf("%i\n",i);
}

int get_negative_int()
{
    int n;
    do
    {
        n = get_int("Negative Number: ");
    }
    while (n >= 0);

    return n;
}
