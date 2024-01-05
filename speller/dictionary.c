// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];

// Total words in dictonary
int loaded_words = 0;

//  bool to check if dictonary is loaded
bool loaded = false;

// number of words unloaded
int unloaded_words = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    bool first = true;

    // create a ptr that checks each node
    node *ptr = NULL;

    // initialise it to point to head
    if (first)
    {
        ptr = table[hash(word)];
        first = false;
    }

    // loop for each node in the list
    while (ptr != NULL)
    {
        // if found
        if (strcasecmp(ptr->word, word) == 0)
            return true;
        // point to next node
        ptr = ptr->next;
    }
    // return false if not found
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    return toupper(word[0]) - 'A';
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    // open dictonary (file containing words)
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        printf("Couldn't open file.\n");
        return false;
    }

    // reads one char at a time till EOF
    char word_buffer[LENGTH];
    int index = 0;

    char c;
    while (fread(&c, sizeof(char), 1, file))
    {
        // copying char by char into word_buffer
        if (c != '\n')
        {
            word_buffer[index] = c;
            index++;
            continue;
        }
        // when word ends
        else
        {
            word_buffer[index] = '\0';
            index = 0;
        }
        // word has been fully copied into word_buffer

        // find hash value of obtained word
        int hash_num = hash(word_buffer);

        // put it inside the hashmapped linked list
        // create a temp node
        node *tmp = malloc(sizeof(node));
        if (tmp == NULL)
        {
            printf("Couldn't copy word into memory.\n");
            return false;
        }

        // copy value and pointer to tmp
        strcpy(tmp->word, word_buffer);
        tmp->next = table[hash_num];

        // add word into the linked list accodring to its hash value
        table[hash_num] = tmp;

        loaded_words++;
    }

    fclose(file);

    loaded = true;

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    if (loaded)
        return loaded_words;
    return 0;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO

    // create a ptr
    node *ptr = NULL;

    // loop for each element of the hash table
    for (int i = 0; i < N; i++)
    {
        // table[i] if the head pointer of a bucket
        while (table[i] != NULL)
        {
            ptr = table[i];
            table[i] = table[i]->next;
            free(ptr);

            unloaded_words++;
        }
    }

    if (loaded_words == unloaded_words)
        return true;
    return false;
}