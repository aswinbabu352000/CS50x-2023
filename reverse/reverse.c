#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "wav.h"

int check_format(WAVHEADER header);
int get_block_size(WAVHEADER header);

int main(int argc, char *argv[])
{
    // Ensure proper usage
    // TODO #1
    if (argc != 3)
    {
        printf("Correct Usage : ./reverse INPUT.wav OUTPUT.wav\n");
        return 1;
    }
    // Open input file for reading
    // TODO #2
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Couldn't open input file.\n");
        return 1;
    }

    // Read header
    // TODO #3
    WAVHEADER header;
    fread(&header, sizeof(WAVHEADER), 1, input);

    // Use check_format to ensure WAV format
    // TODO #4
    check_format(header);

    // Open output file for writing
    // TODO #5
    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Couldn't create output file.\n");
        return 1;
    }

    // Write header to file
    // TODO #6
    fwrite(&header, sizeof(WAVHEADER), 1, output);

    // Use get_block_size to calculate size of block
    // TODO #7
    int block_size = get_block_size(header);

    // Write reversed audio to file
    // TODO #8

    // finding number of blocks
    // ftell gives size of header in bytes
    // seeking to end gives size of the whole file in bytes
    long audio_start = ftell(input);
    fseek(input, 0, SEEK_END);
    long audio_end = ftell(input);
    long num_block = (audio_end - audio_start) / block_size;

    BYTE buffer[block_size];

    // itterating number-of-block times
    for (int i = 0; i < num_block; i++)
    {
        // seek back one block
        fseek(input, -(block_size), SEEK_CUR);
        // read one block
        fread(buffer, 1, block_size, input);
        // write one block
        fwrite(buffer, 1, block_size, output);
        // seeking back one block again as 'fread' shifts
        // the pointer one block away after reading
        fseek(input, -(block_size), SEEK_CUR);
    }

    fclose(input);
    fclose(output);
}

int check_format(WAVHEADER header)
{
    // TODO #4
    if (header.format[0] == 'W' && header.format[1] == 'A' && header.format[2] == 'V' & header.format[3] == 'E')
    {
        return true;
    }
    else
    {
        return false;
    }
    return 0;
}

int get_block_size(WAVHEADER header)
{
    // TODO #7
    int block_size = (int) (header.numChannels * (header.bitsPerSample / 8));
    return block_size;
}