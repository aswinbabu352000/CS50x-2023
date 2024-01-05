#include <cs50.h>
#include <stdio.h>

int main(void)
{
    long x = get_long("What's x? ");
    long y = get_long("What's y? ");

    long z =  x +  y;

    printf("Sum : %.li\n",z);
}