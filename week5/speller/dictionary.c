// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>


#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int N = LENGTH + 1;
const unsigned int A = (26 * 26 * 26) + (26 * 26) + (26);

// Hash 2d table (length of words by first 3 letters of alphabet)
node *table[N][A];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    // Hash word to a number, i based on length of word
    // Hash word to a number, j based on first 3 letters
    // Identify position on hash table
    // Search through linked list to identify word
    // If exist return true, else false
    unsigned int i = hash(word);
    unsigned int j = hash2(word);
    node *ptr = table[i][j];
    while (ptr != NULL)
    {
        if (strcasecmp(word, (ptr->word)) == 0)
        {
            return true;
        }
        ptr = ptr->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Hash word to a number based on length of word
    return strlen(word);
}

unsigned int hash2(const char *word)
{
    // TODO: Hash word to a number based on first 3 letters
    unsigned int i = 0;
    int l = strlen(word);
    if (l > 2)
    {
        char c0 = tolower(word[0]);
        char c1 = tolower(word[1]);
        char c2 = tolower(word[2]);
        i = 26 * 26 * (c0 - 96) + 26 * (c1 - 96) + 1 * (c2 - 96);
    }
    else if (l == 2)
    {
        char c0 = tolower(word[0]);
        char c1 = tolower(word[1]);
        i = 26 * (c0 - 96) + 1 * (c1 - 96);
    }
    else if (l == 1)
    {
        char c0 = tolower(word[0]);
        i = 1 * (c0 - 96);
    }
    return i;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open dictionary file
    // Loop through every word and whenever it hits a \0,
    // Write the word into a buffer
    // Call hash function to get 1st number based on length of word
    // Call hash2 function to get 2nd number based on first 3 letters
    // Sort accordingly to the hash table
    // TODO
    char buffer[LENGTH + 2];
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }
    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        node *n = malloc(sizeof(node));
        remove_newline(buffer);
        strcpy(n->word, buffer);
        unsigned int i = hash(buffer);
        unsigned int j = hash2(buffer);
        n->next = table[i][j];
        table[i][j] = n;
    }
    if (feof(file) == 1)
    {
        fclose(file);
        return true;
    }
    return false;
}

// Remove \n from words in dictionary
void remove_newline(char *word)
{
    int l = strlen(word);
    if (l > 0 && word[l - 1] == '\n')
    {
        word[l - 1] = '\0';
    }
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    unsigned int count = 0;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < A; j++)
        {
            node *ptr = table[i][j];
            while (ptr != NULL)
            {
                count++;
                ptr = ptr->next;
            }
        }
    }
    return count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < A; j++)
        {
            node *ptr = table[i][j];
            while (ptr != NULL)
            {
                node *tmp = ptr;
                ptr = ptr->next;
                free(tmp);
            }
        }
    }
    return true;
}
