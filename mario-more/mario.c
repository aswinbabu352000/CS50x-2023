#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Prompt user for height
    int height;
    do
        height = get_int("Height: ");
    while (height < 1 || height > 8);

    int n = height - 1;
    int o = 1;

    // Print pyramid of height
    for (int i = 0; i < height; i++)
    {
        // Flipped right sided invisible triangle of 'height - 1' height
        for (int j = 0; j < n; j++)
            printf(" ");

        // Normal left sided triangle
        for (int k = 0; k < o; k++)
            printf("#");

        // Space between the two triangles
        printf("  ");

        // Normal right sided triangle
        for (int l = 0; l < o; l++)
            printf("#");

        // go to next line
        printf("\n");

        n--;
        o++;
    }
}