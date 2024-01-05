#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int numbers[5] = {9, 6, 2, 4, 1};
    int swap = -1;
    int store;

    // first loop - to eliminate the last element after each pass as it becomes sorted.
    for (int j = 4; j > 0; j--)
    {
        // while loop to stop this algorithm when its sorted.
        while (swap != 0)
        {
            swap = 0;
            // This lopp is where bubbling happens
            for (int i = 0; i < j; i++)
            {

                store = 0;

                if (numbers[i] > numbers[i + 1])
                {
                    store = numbers[i + 1];
                    numbers[i + 1] = numbers[i];
                    numbers[i] = store;

                    swap++; // if swap = 0 it means that the array is sorted.
                }
            }

            for (int l = 0; l < 5; l++)
            {
                printf("%i, ", numbers[l]);
            }
            printf("\n");
        }
    }

    printf("Final Sorted array: ");

    for (int o = 0; o < 5; o++)
    {
        printf("%i, ", numbers[o]);
    }

    printf("\n");
}