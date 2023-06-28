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
        printf("Usage: ./reverse input.wav output.wav\n");
        return 1;
    }

    // Open input file for reading
    // TODO #2
    if (fopen(argv[1], "r") == NULL)
    {
        printf("Unable to open file.\n");
        return 1;
    }

    FILE *input = fopen(argv[1], "r");

    // Read header (44 bytes)
    // TODO #3
    WAVHEADER header;
    fread(&header, 1, 44, input);
    int end_header = ftell(input);

    // Use check_format to ensure WAV format
    // TODO #4
    if (check_format(header) == 1)
    {
        printf("Input is not a WAV file.\n");
        return 2;
    }

    // Open output file for writing
    // TODO #5
    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Output file did not open successfully.\n");
        return 1;
    }

    // Write header to file
    // TODO #6
    fwrite(&header, 1, 44, output);

    // Use get_block_size to calculate size of block
    // TODO #7
    int block_size = get_block_size(header);

    // Read each block from end to start
    // Reverese bytes
    // Write reversed audio to file
    // Loop steps until end of header file
    // TODO #8
    uint8_t buffer[block_size];
    uint8_t reversed[block_size];
    fseek(input, block_size * -1, SEEK_END);

    while (ftell(input) >= end_header)
    {
        fread(buffer, 1, block_size, input);
        fwrite(buffer, 1, block_size, output);
        fseek(input, block_size * -2, SEEK_CUR);
    }

    fclose(input);
    fclose(output);
    return 0;
}

int check_format(WAVHEADER header)
{
    // Check format in header if char == W,A,V,E
    // TODO #4
    if ((header.format[0]) != 87 && (header.format[1]) != 65 && (header.format[2]) != 86 && (header.format[3]) != 69)
    {
        return 1;
    }
    return 0;
}

int get_block_size(WAVHEADER header)
{
    // Size of each block is calculated by
    // no. of channels * bytes per sample
    // Of which can be found in the header file
    // TODO #7
    int block_size = header.numChannels * header.bitsPerSample / 8;
    return block_size;
}