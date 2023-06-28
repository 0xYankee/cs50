#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

bool only_alpha(string k);

bool no_dups(char uk[], char lk[], string k);

string decipher(char uk[], char lk[], string k, string p);

int main(int argc, string argv[])
{
    // Handles lack of key
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    string p;
    string k = argv[1];

    // Get the sets of key for all lowercase / uppercase
    char uk[26];
    char lk[26];
    for (int i = 0; i < 26; i++)
    {
        char u = toupper(k[i]);
        uk[i] = u;
        char l = tolower(k[i]);
        lk[i] = l;
    }

    // Handles invalid key length, duplicate keys and too many args
    if (argc == 2 && only_alpha(k) && no_dups(uk, lk, k) && strlen(k) == 26)
    {
        // Prompt user to input plaintext, p
        p = get_string("plaintext: ");

        // Decipher plaintext, p and receive a ciphertext, c in return
        string c = decipher(uk, lk, k, p);

        // Print ciphertext, c
        printf("ciphertext: %s\n", c);
    }
    else if (argc == 2 && only_alpha(k) && strlen(k) != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }
    else
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
}

// Checks for only letters in key
bool only_alpha(string k)
{
    for (int i = 0; i < 26; i++)
    {
        if (!isalpha(k[i]))
        {
            return false;
        }
    }
    return true;
}

// Checks for no duplicates of letters in key
bool no_dups(char uk[], char lk[], string k)
{
    for (int i = 0; i < 25; i++)
    {
        for (int j = i + 1; j < 26; j++)
        {
            if (k[i] == k[j] || uk[i] == uk[j] || lk[i] == lk[j])
            {
                return false;
            }
        }
    }
    return true;
}

// Substitute plaintext to ciphertext
string decipher(char uk[], char lk[], string k, string p)
{
    int l = strlen(p);
    string c = p;

    for (int i = 0; i < l; i++)
    {
        if (isupper(p[i]))
        {
            // If current letter in position i is (pos)ition in the alphabet, it corresponds to the position of the letter in the key
            int pos = p[i] - 65;
            char ch = uk[pos];
            c[i] = ch;
        }
        else if (islower(p[i]))
        {
            // If current letter in position i is (pos)ition in the alphabet, it corresponds to the position of the letter in the key
            int pos = p[i] - 97;
            char ch = lk[pos];
            c[i] = ch;
        }
    }
    
    return c;
}