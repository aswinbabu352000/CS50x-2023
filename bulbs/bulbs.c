#include <cs50.h>
#include <stdio.h>
#include <string.h>

const int BITS_IN_BYTE = 8;

void print_bulb(int bit);

int main(void)
{
    // TODO
    string s = get_string("Message: ");

    int binary_value[8] = {128, 64, 32, 16, 8, 4, 2, 1};
    int i = 0;
    int light;

    while (s[i] != '\0')
    {
        int decimal_value = s[i];

        for (int j = 0; j < 8; j++)
        {
            if (decimal_value - binary_value[j] == 0)
            {
                light = 1;
                decimal_value = decimal_value - binary_value[j];
            }
            else if (decimal_value - binary_value[j] > 0)
            {
                light = 1;

                decimal_value = decimal_value - binary_value[j];
            }
            else 
            {
                light = 0;
            }

            print_bulb(light);
        }

        printf("\n");
        i++;
    }
}

void print_bulb(int bit)

{
    if (bit == 0)
    {
        // Dark emoji
        printf("\U000026AB");
    }
    else if (bit == 1)
    {
        // Light emoji
        printf("\U0001F7E1");
    }
}
