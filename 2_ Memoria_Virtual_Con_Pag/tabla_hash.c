#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

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

hashTable newHashTable(int size) {
    hashTable newHash;
    newHash.buckets = size;
    newHash.count = 0;
    newHash.table = (item**)malloc(sizeof(item*)*newHash.buckets);
    for(int i=0; i<size; i++)
        newHash.table[i] = NULL;
    return newHash;
}

int hashFunction(hashTable hash, int value) {
    return (value % hash.buckets);
}

bool insertItem(hashTable hash, int key, int value) {
    if(hash.count == hash.buckets)
        return false;
    int index = hashFunction(hash, key);
    item *newItem = (item*)malloc(sizeof(item));
    newItem->key = key;
    newItem->value = value;
    newItem->next = NULL;
    if (hash.table[index] == NULL)
        hash.table[index] = newItem;
    else {
        item *currItem = hash.table[index];
        while(currItem->next != NULL) {
            currItem = currItem->next;
        }
        currItem->next = newItem;
    }
    return true;
}

void deleteItem(hashTable hash, int key) {
    int index = hashFunction(hash, key);
    if (hash.table[index] == NULL)
        return;
    item *currItem = hash.table[index];
    if(currItem->key == key) {
        hash.table[index] = currItem->next;
        free(currItem);
        return;
    }
    item *prevItem = hash.table[index];
    currItem = hash.table[index]->next;
    while(currItem != NULL) {
        if(currItem->key == key) {
            prevItem->next = currItem->next;
            free(currItem);
            return;
        } else {
            prevItem = currItem;
            currItem = currItem->next;
        }
    }
}

bool searchItem(hashTable hash, int key) {
    int index = hashFunction(hash, key);
    if (hash.table[index] == NULL)
        return false;
    item *currItem = hash.table[index];
    while(currItem != NULL) {
        if(currItem->key == key) {
            return true;
        } else {
            currItem = currItem->next;
        }
    }
    return false;
}

item* getItem(hashTable hash, int key) {
    int index = hashFunction(hash, key);
    if (hash.table[index] == NULL)
        return NULL;
    item *currItem = hash.table[index];
    while(currItem != NULL) {
        if(currItem->key == key) {
            return currItem;
        } else {
            currItem = currItem->next;
        }
    }
    return NULL;
}

int displayHash(hashTable hash) {
    item *currItem;
    for(int i=0; i<hash.buckets; i++) {
        printf("%d: ", i);
        if(hash.table[i] == NULL) {
            printf("-");
        } else {
            currItem = hash.table[i];
            while(currItem != NULL) {
                printf("%d", currItem->key);
                if(currItem->next != NULL)
                    printf(" --> ", currItem->key);
                currItem = currItem->next;
            }
        }
        printf("\n");
    }
    printf("\n");
}

int deleteHash(hashTable hash) {
    item *currItem;
    item *list[hash.buckets];
    for(int i=0; i<hash.buckets; i++) {
        if(hash.table[i] != NULL) {
            currItem = hash.table[i];
            int j=0;
            while(currItem != NULL) {
                list[j] = currItem;
                currItem = currItem->next;
                j++;
            }
            for(int k=j; k<=0; k--)
                free(list[k]);
        }
    }
    free(hash.table);
}