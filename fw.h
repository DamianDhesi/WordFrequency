/*by Damian Dhesi*/
/*Contains funciton prototypes, typedefs, and struct definitions for hashtable.c
 *and main.c*/

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>

/*pointer to a struct holding a pointer to a word, its frequency, and a pointer
 *to the next word if there is one in the linked list*/
typedef struct link *WordPtr;

typedef struct link{
        char *word;
        int freq;
        WordPtr nxt;
} WordLink;

/*HashTable has a WordPtr wich is the table of values itself, a size tracking
 *the number of elements in the table and a capacity to track the number of 
 *indices in the table itself*/
typedef struct DataTable{
        WordPtr *table;
        int size;
        int capacity;
} HashTable;

/*Pointer for Hashtable structs*/
typedef struct DataTable *HashPtr;

/*Function prototypes for hashtable.c, functionality explained
 *at implementation*/
int Hash(char *str, HashPtr hash_table);

void add(char *str, HashPtr hash_table);

void UpCapacity(HashPtr PrevTable);

int LoadFactor(HashPtr hash_table);

char* PopMostFreq(HashPtr hash_table, int *MaxFreq);
