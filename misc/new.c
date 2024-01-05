#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void)
{
    int x = 0;

    int arr[2] = {1, 2};

    srand(time(0));

    int num = (rand() % (1 - 0 + 1)) + 0;
    x =  arr[num];

    printf("%i\n", x);

}