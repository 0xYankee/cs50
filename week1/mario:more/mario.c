#include <cs50.h>
#include <stdio.h>

int main(void)
{
    //Prompt user for a positive integer ONLY between 1 and 8
    int i;
    do
    {
        i = get_int("Choose a number between 1 and 8: ");
    }
    while (i > 8 || i < 1);

    //Print pyramid
    int l = 1;
    while (i > 0)
    {
        //Print space(s)
        int s = i - 1;
        do
        {
            if (s != 0)
            {
                printf(" ");
            }
            s--;
        }
        while (s > 0);

        //Print left hash(es)
        int lh = 0;
        do
        {
            printf("#");
            lh++;
        }
        while (lh < l);

        //Print middle hashes
        printf("  ");

        //Print right hash(es)
        int rh = 0;
        do
        {
            printf("#");
            rh++;
        }
        while (rh < l);

        //Print new line
        printf("\n");
        l++;
        i--;
    }
}