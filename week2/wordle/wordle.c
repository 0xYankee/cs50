#include <cs50.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

// each of our text files contains 1000 words
#define LISTSIZE 1000

// values for colors and score (EXACT == right letter, right place; CLOSE == right letter, wrong place; WRONG == wrong letter)
#define EXACT 2
#define CLOSE 1
#define WRONG 0

// ANSI color codes for boxed in letters
#define GREEN   "\e[38;2;255;255;255;1m\e[48;2;106;170;100;1m"
#define YELLOW  "\e[38;2;255;255;255;1m\e[48;2;201;180;88;1m"
#define RED     "\e[38;2;255;255;255;1m\e[48;2;220;20;60;1m"
#define RESET   "\e[0;39m"

// user-defined function prototypes
string get_guess(int wordsize);
int check_word(string guess, int wordsize, int status[], string choice);
void print_word(string guess, int wordsize, int status[]);
bool check_size(string k);

int main(int argc, string argv[])
{
    // TODO #1 -- Ensure the program accepts a single command-line argument
    if (argc != 2)
    {
        printf("Usage: ./wordle wordsize\n");
        return 1;
    }

    int wordsize = 0;
    string k = argv[1];

    // TODO #2 -- Ensure the program only accepts 5, 6, 7, or 8 and store that value in wordsize instead
    if (argc == 2 && check_size(k))
    {
        wordsize = atoi(k);
    }
    else if (argc == 2 && !check_size(k))
    {
        printf("Error: wordsize must be either 5, 6, 7, or 8\n");
        return 1;
    }
    else
    {
        printf("Usage: ./wordle wordsize\n");
        return 1;
    }

    // open correct file, each file has exactly LISTSIZE words
    char wl_filename[6];
    sprintf(wl_filename, "%i.txt", wordsize);
    FILE *wordlist = fopen(wl_filename, "r");
    if (wordlist == NULL)
    {
        printf("Error opening file %s.\n", wl_filename);
        return 1;
    }

    // load word file into an array of size LISTSIZE
    char options[LISTSIZE][wordsize + 1];

    for (int i = 0; i < LISTSIZE; i++)
    {
        fscanf(wordlist, "%s", options[i]);
    }

    // pseudorandomly select a word for this game
    srand(time(NULL));
    string choice = options[rand() % LISTSIZE];

    // allow one more guess than the length of the word
    int guesses = wordsize + 1;
    bool won = false;

    // print greeting, using ANSI color codes to demonstrate
    printf(GREEN"This is WORDLE50"RESET"\n");
    printf("You have %i tries to guess the %i-letter word I'm thinking of\n", guesses, wordsize);

    // main game loop, one iteration for each guess
    for (int i = 0; i < guesses; i++)
    {
        // obtain user's guess
        string guess = get_guess(wordsize);

        // array to hold guess status, initially set to zero
        int status[wordsize];

        // TODO #4 -- Set the elements of the array to 0s at the start of game
        for (int n = 0; n < wordsize; n++)
        {
            status[n] = 0;
        }

        // Calculate score for the guess
        int score = check_word(guess, wordsize, status, choice);

        printf("Guess %i: ", i + 1);

        // Print the guess
        print_word(guess, wordsize, status);

        // if they guessed it exactly right, set terminate loop
        if (score == EXACT * wordsize)
        {
            won = true;
            break;
        }
    }

    // TODO #7 -- Print game results and choice word
    if (won)
    {
        printf("You won!\n");
    }
    if (!won)
    {
        printf("The correct word is %s\n", choice);
    }

    return 0;
}

bool check_size(string k)
{
    printf("%i\n", atoi(k));
    if (atoi(k) > 8 || atoi(k) < 5)
    {
        return false;
    }
    return true;
}

string get_guess(int wordsize)
{
    string guess = "";

    // TODO #3 -- Ensure users provide a guess that is the same length as wordsize
    do
    {
        guess = get_string("Input a %i-letter word: ", wordsize);
    }
    while (strlen(guess) != wordsize);

    return guess;
}

int check_word(string guess, int wordsize, int status[], string choice)
{
    int score = 0;

    // TODO #5 -- Compare `guess` and `choice` to score and allocate points in the status array
    for (int pos = 0; pos < wordsize; pos++)
    {
        for (int i = 0; i < wordsize; i++)
        {
            // Score EXACT points for the letters in the correct position
            if (guess[pos] == choice[pos])
            {
                status[pos] = EXACT;
                break;
            }
            // Score CLOSE points for letters in the word but in the wrong position
            else if (guess[pos] == choice[i])
            {
                status[pos] = CLOSE;
            }
        }

        // Add the current points to score to keep track
        score += status[pos];
    }
    return score;
}

void print_word(string guess, int wordsize, int status[])
{
    // TODO #6 -- Print the word, where each colour of character represents the points scored
    //         -- Then, reset font after each iteration
    for (int i = 0; i < wordsize; i++)
    {
        if (status[i] == 2)
        {
            printf(GREEN"%c", guess[i]);
        }
        else if (status[i] == 1)
        {
            printf(YELLOW"%c", guess[i]);
        }
        else if (status[i] == 0)
        {
            printf(RED"%c", guess[i]);
        }
    }

    printf(RESET"\n");
    return;
}