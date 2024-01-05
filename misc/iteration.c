#include <cs50.h>
#include <stdio.h>

void draw_pyramid(int size);

int main(void)
{
    int size = get_int("Size: ");

    draw_pyramid(size);
}

void draw_pyramid(int size)
{
    for (int i = 0; i < size ; i++)
    {
        for (int j = 0; j < i + 1; j++ )
        {
            printf("#");
        }
        printf("\n");
    }
}