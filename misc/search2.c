#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int array[3] = {0, 1, 2};
    bool found = false;

    for (int i = 0; i < 3; i++)
    {
        if (0 == array[i])
            found = true;
    }

    if (found == true)
        printf("Found!\n");
    else
        printf("Not Found!\n");
}