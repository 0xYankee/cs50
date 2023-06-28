#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

long long card_number();
bool is_valid(long long n);

int main(void)
{
    long long n = card_number();
    bool valid = is_valid(n);

    char str[20];
    sprintf(str, "%lld", n);

    const char *AMEX = "37";
    const char *MASTERCARD1 = "222";
    const char *MASTERCARD2 = "51";
    const char *MASTERCARD3 = "52";
    const char *MASTERCARD4 = "55";
    const char *VISA1 = "40";
    const char *VISA2 = "41";
    const char *VISA3 = "42";
    const char *VISA4 = "499999";

    if (valid && strncmp(str, AMEX, strlen(AMEX)) == 0)
    {

        printf("AMEX\n");
    }
    else if (valid && (strncmp(str, MASTERCARD1, strlen(MASTERCARD1)) == 0 || strncmp(str, MASTERCARD2, strlen(MASTERCARD2)) == 0
                       || strncmp(str, MASTERCARD3, strlen(MASTERCARD3)) == 0 || strncmp(str, MASTERCARD4, strlen(MASTERCARD4)) == 0))
    {
        printf("MASTERCARD\n");
    }
    else if (valid && (strncmp(str, VISA1, strlen(VISA1)) == 0 || strncmp(str, VISA2, strlen(VISA2)) == 0
                       || strncmp(str, VISA3, strlen(VISA3)) == 0 || strncmp(str, VISA4, strlen(VISA4)) == 0))
    {
        printf("VISA\n");
    }
    else
    {
        printf("INVALID\n");
    }
}

//Prompt user to insert card number
long long card_number()
{
    long long n;
    char str[20];
    int length;
    n = get_long_long("Card: ");
    sprintf(str, "%lld", n);
    length = strlen(str);
    if (length < 13 || length > 16)
    {
        printf("INVALID\n");
        exit(0); // Exit the program if the card number is invalid
    }
    else
    {
        return n;
    }
}

//Check card number validity
bool is_valid(long long n)
{
    long long sum = 0;
    bool alternate = false;
    while (n > 0)
    {
        long long digit = (n % 10);
        if (alternate)
        {
            digit *= 2;
            if (digit > 9)
            {
                digit = (digit % 10) + 1;
            }
        }
        sum += digit;
        n /= 10;
        alternate = !alternate;
    }
    return (sum % 10) == 0;
}