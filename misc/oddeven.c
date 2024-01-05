#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int i = get_int("Number: ");

    if  ( (int)(((float)i / 2) * 10) % 10 == 0)
         printf("%i is even.\n", i);
    else
          printf("%i is odd.\n", i);

}