#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint8_t BYTE;

#define BLOCK_SIZE 512

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Correct Usage: ./recover filename\n");
        return 1;
    }

    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    int jpg_number = -1;
    char filename[8] = {0};
    bool open = false;

    BYTE buffer[512];
    FILE *output = NULL;

    // copy 512 bytes into buffer on each itteration
    while (fread(buffer, 1, BLOCK_SIZE, input) == BLOCK_SIZE)
    {
        // if signature is found in buffer
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // 2. if a jpeg is being already being written into
            if (open == true)
            {
                fclose(output);
                open = false;
            }

            // 1. if jpeg is not already created / 3. create next jpeg
            if (open == false)
            {
                jpg_number++;

                sprintf(filename, "%03i.jpg", jpg_number);

                output = fopen(filename, "w");
                if (output == NULL)
                {
                    printf("Cound not open output file\n");
                    return 1;
                }
                open = true;
            }
        }

        // write bytes to jpeg if file is open and signature not found
        if (open == true)
        {
            fwrite(buffer, 1, BLOCK_SIZE, output);
        }
    }
    fclose(input);
    fclose(output);
}