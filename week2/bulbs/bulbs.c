#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

const int BITS_IN_BYTE = 8;

void print_bulb(int bit);

int main(void)
{
    // Get string from user
    string s = get_string("Message: ");

    //  Convert string to 8-bit binary numbers
    int bit = 0;
    int length = strlen(s);
    for (int x = 0; x < length; x++)
    {
        int n = s[x];
        for (int y = 7; y > -1; y--)
        {
            // Mod n by 2^7 to 2^0
            int r = (n / (int) pow(2, y)) % 2;
            if (r > 0)
            {
                bit = 1;
            }
            else
            {
                bit = 0;
            }
            // Print bulbs
            print_bulb(bit);
        }
        printf("\n");
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
