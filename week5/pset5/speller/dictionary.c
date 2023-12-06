// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <math.h>
#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word1[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 1000000;

// Hash table
node *table[N];
//counting the words
unsigned int count = 0;
// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    node *cursor;
    cursor = table[hash(word)];
    while (cursor != NULL)
    {
        if (strcasecmp(word, cursor->word1) == 0)
        {
            return true;
        }
        else
        {

            cursor = cursor->next;

        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    unsigned int k = (toupper(*word) - 'A');
    int len = strlen(word);
    for (int i = 0; i < len; i++)
    {
        k = k + (toupper(*(word + i)) - 'A') * 26 * (i);
    }
    return (k % N);

}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO

    char *word;
    word = malloc(sizeof(char) * 47);
    //opening the file
    FILE *dic = fopen(dictionary, "r");

    //checkcing if the file opened correctly
    if (dic == NULL)
    {
        return false;
    }

    //reading strings from file
    for (unsigned int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }
    while (fscanf(dic, "%s", word) != EOF)
    {
        node *L;
        count++;
        L = malloc(sizeof(node));
        if (L == NULL)
        {
            fclose(dic);
            return false;
        }
        strcpy(L->word1, word);
        L->next = table[hash(word)];
        table[hash(word)] = L;

    }
    fclose(dic);
    free(word);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    for (unsigned int j = 0; j < N; j++)
    {
        node *tmp;
        node *cursor;
        cursor = table[j];
        while (cursor != NULL)
        {
            tmp = cursor;
            cursor = cursor->next;
            free(tmp);



        }
    }

    return true;
}
