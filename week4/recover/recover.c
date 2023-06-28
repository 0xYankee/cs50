#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint8_t BYTE;
const size_t BYTE_SIZE = 8;
const size_t BLOCK_SIZE = 512;

int main(int argc, char *argv[])
{
    // Error out if wrong command is provided
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    FILE *file = fopen(argv[1], "r");

    // Error out if file not found or file cannot be opened
    if (file == NULL)
    {
        printf("Could not open %s\n", argv[1]);
        return 1;
    }

    // Look for the beginning of a JPEG file where
    // First 4 bytes of every file: 0xff 0xd8 0xff 0xe? (1110 ????)
    // Each JPEG file is in the size of multiples of 512 bytes (1 block)
    // Loop the reading of each block to detect the first 4 bytes
    // If the conditions of the first 4 bytes are met, open a new JPEG file to write to

    BYTE buffer[512];
    int jpeg_count = 0;
    char newfile[8];
    FILE *img = NULL;

    while (fread(buffer, 1, BLOCK_SIZE, file) == BLOCK_SIZE)
    {
        // Start of a new JPEG
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            if (jpeg_count > 0)
            {
                fclose(img);
            }

            sprintf(newfile, "%03i.jpg", jpeg_count);
            img = fopen(newfile, "w");
            jpeg_count++;
        }

        if (img != NULL)
        {
            fwrite(buffer, 1, BLOCK_SIZE, img);
        }
    }

    fclose(img);
    fclose(file);
    return 0;
}
