#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int  smallest = 0;
    int number[5] = {4, 1, 2, 5, 3};

    smallest = number[0];

    for (int i = 1; i < 5; i++)
    {
            if (number[i] < smallest)
            {
                smallest = number[i];
                printf("%i\n", smallest);
            }
    }

    printf("Smallest value is: %i\n", smallest);

}
