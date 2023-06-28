#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int main(void)
{
    // Prompt user for a string of text
    string text = get_string("Text: ");

    // Get numbers for letters, words and sentences
    int letters = 0;
    int words = 1;
    int sentences = 0;

    for (int n = 0, l = strlen(text); n < l; n++)
    {
        // Get letters
        if (isupper(text[n]) || islower(text[n]))
        {
            letters++;
        }

        // Get words
        if (text[n] == 32)
        {
            words++;
        }

        // Get sentences
        if (text[n] == 33 || text[n] == 46 || text[n] == 63)
        {
            sentences++;
        }
    }

    //Calculate algorithm
    float l = (float) letters / words * 100;
    float s = (float) sentences / words * 100;
    float grade = 0.0588 * l - 0.296 * s - 15.8;
    int g = round(grade);

    // Print result
    if (g < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (g > 15)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", g);
    }

}