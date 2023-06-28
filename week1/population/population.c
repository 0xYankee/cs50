#include <cs50.h>
#include <stdio.h>

int main(void)
{
    //Prompt for start size
    int start;
    do
    {
        start = get_int("Start size: ");
    }
    while (start <= 8);

    //Prompt for end size
    int end;
    do
    {
        end = get_int("End size: ");
    }
    while (start > end);

    //Calculate number of years until we reach threshold
    int year = 0;
    while (start < end)
    {
        start = start + (start / 3) - (start / 4);
        year++;
    }

    //Print number of year(s)
    printf("Years: %i\n", year);
}