// Implements a dictionary's functionality
// Thanks to this StackOverflow answer - https://stackoverflow.com/a/31930047 for the major assistance in implementing load.

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N] = {NULL};

// Size
unsigned int dictSize = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    unsigned int wordHash = hash(word);
    for (node *tmp = table[wordHash]; tmp != NULL; tmp = tmp->next)
    {
        if (strcasecmp(word, tmp->word) == 0)
        {
            return true;
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    return tolower(word[0]) - 'a';
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    FILE *file = fopen(dictionary, "r");
    char *dictWord = malloc(LENGTH + 1);
    node *n;
    unsigned int wordHash;
    if (file == NULL)
    {
        return false;
    }
    while (fscanf(file, "%s", dictWord) != EOF)
    {
        n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }
        strcpy(n->word, dictWord);
        n->next = NULL;
        wordHash = hash(dictWord);
        if (table[wordHash] == NULL)
        {
            table[wordHash] = n;
            dictSize++;
        }
        else
        {
            node *p = table[wordHash];
            while (true)
            {
                if (p->next == NULL)
                {
                    dictSize++;
                    p->next = n;
                    break;
                }
                p = p->next;
            }
        }
    }
    free(dictWord);
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return dictSize;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        while (table[i] != NULL)
        {
            node *tmp = table[i]->next;
            free(table[i]);
            table[i] = tmp;
        }
    }
    return true;
}
