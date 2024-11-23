#ifndef DUPGUARD
#define DUPGUARD

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef struct item {
    int key;
    int value;
    struct item *next;
} item;

typedef struct hashTable {
    int buckets;
    int count;
    item **table;
} hashTable;

hashTable newHashTable(int size);

int hashFunction(hashTable hash, int value);

bool insertItem(hashTable hash, int key, int value);

void deleteItem(hashTable hash, int key);

bool searchItem(hashTable hash, int key);

item* getItem(hashTable hash, int key);

int displayHash(hashTable hash);

int deleteHash(hashTable hash);

void OPTM(int size, int *lista, int largo);

void FIFO(int size, int *lista, int largo);

void LRU(int size, int *lista, int largo);

void LRU_RS(int size, int *lista, int largo);

#endif
