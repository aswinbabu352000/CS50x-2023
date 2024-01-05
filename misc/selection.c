#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int array[5] = {5, 4, 1, 3, 2};
    int smallest;
    int position;
    int dummy;

    // Selection Sort

    // Iterate for each elemnt
    for (int i = 0; i < 5; i++)
    {
        // Set that element as smallest
        position = -1;
        smallest = array[i];

        // Iterate the rest of the elements (of the unsorted array)
        for (int j = i + 1; j < 5; j++)
        {
            // An element (of the unsorted array) is smaller current smallest value
            if (array[j] < smallest)
            {
                // Record its value and position
                smallest = array[j];
                position = j;
            }
        }

        // Swap the smallest value of the unsorted array with the last value of the sorted aray
        dummy = smallest;
        array[position] = array[i];
        array[i] = dummy;
    }

    // Print the array
    for (int k = 0; k < 5; k++)
        printf("%i, ", array[k]);

    printf("\n");
}