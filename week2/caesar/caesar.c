#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

bool only_digits(string s);

string decipher(int k, string p);

int main(int argc, string argv[])
{
    string s = argv[1];
    string p;
    if (argc == 2 && only_digits(s))
    {
        // Convert key from string to int
        int k = atoi(argv[1]);

        // Prompt user to input plaintext, p
        p = get_string("plaintext: ");

        // Decipher plaintext, p and receive a ciphertext, c in return
        string c = decipher(k, p);

        // Print ciphertext, c
        printf("ciphertext: %s\n", c);
    }
    else
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
}

bool only_digits(string s)
{
    int l = strlen(s);
    for (int i = 0; i < l; i++)
    {
        if (!isdigit(s[i]))
        {
            return false;
        }
    }
    return true;
}

string decipher(int k, string p)
{
    // Receive an array of chars, p
    // Rotate the int of chars by key
    // Output and return array of chars, c

    int l = strlen(p);
    string c = p;

    for (int i = 0; i < l; i++)
    {
        int n = p[i];
        int t = c[i];
        if (islower(n))
        {
            t = ((n - 97 + k) % 26) + 97;
            c[i] = t;
        }
        else if (isupper(n))
        {
            t = ((n - 65 + k) % 26) + 65;
            c[i] = t;
        }
    }

    return c;
}