#include "tabla_hash.c"

void OPTM() {}

void FIFO(int size, int *lista, int largo) {
    hashTable tabla = newHashTable(size);
    int missCount = 0;
    int queue[size];
    int index = 0;
    for(int i=0; i<largo; i++) {
        if(searchItem(tabla, lista[i]))
            printf("HIT: [%d]", lista[i]);
        else {
            printf("MISS: [%d]", lista[i]);
            missCount++;
            if(insertItem(tabla, lista[i], 0)) {
                tabla.count++;
                queue[index] = lista[i];
                index++;
            } else {
                printf("\nVICTIM: [%d]", queue[0]);
                deleteItem(tabla, queue[0]);
                tabla.count--;
                for(int i=0; i<index-1; i++)
                    queue[i] = queue[i+1];
                queue[index-1] = lista[i];
                insertItem(tabla, lista[i], 0);
                tabla.count++;
            }
        }
        printf("\n");
        displayHash(tabla);
    }
    printf("Total Miss: %d\n", missCount);
    deleteHash(tabla);
}

void LRU(int size, int *lista, int largo) {
    hashTable tabla = newHashTable(size);
    int missCount = 0;
    int position = 0;
    item *currItem;
    for(int i=0; i<largo; i++) {
        if(searchItem(tabla, lista[i])) {
            printf("HIT: [%d]", lista[i]);
            getItem(tabla, lista[i])->value = position;
        } else {
            printf("MISS: [%d]", lista[i]);
            missCount++;
            if(insertItem(tabla, lista[i], position)) {
                tabla.count++;
            } else {
                item *victim = NULL;
                for(int j=0; j<tabla.buckets; j++) {
                    if(tabla.table[j] != NULL) {
                        if(victim==NULL)
                            victim = tabla.table[j];
                        currItem = tabla.table[j];
                        while(currItem != NULL) {
                            if(currItem->value < victim->value)
                                victim = currItem;
                            currItem = currItem->next;
                        }
                    }
                }
                printf("\nVICTIM: [%d]", victim->key);
                deleteItem(tabla, victim->key);
                tabla.count--;
                insertItem(tabla, lista[i], position);
                tabla.count++;
            }
        }
        position++;
        printf("\n");
        displayHash(tabla);
    }
    printf("Total Miss: %d\n", missCount);
    deleteHash(tabla);
}


// TA MAL; FUE UNA IDEA NOMA EL LRU_SR (Simple Reloj)
void LRU_RS(int size, int *lista, int largo) {    
    hashTable tabla = newHashTable(size);
    int missCount = 0;
    int queue[size];
    int index = 0;
    item *currItem;
    for(int i=0; i<largo; i++) {
        if(searchItem(tabla, lista[i])) {
            printf("HIT: [%d]", lista[i]);
            getItem(tabla, lista[i])->value = 1;
        } else {
            printf("MISS: [%d]", lista[i]);
            missCount++;
            if(insertItem(tabla, lista[i], 1)) {
                tabla.count++;
                queue[index] = lista[i];
                index++;
            } else {
                int victim = 0;
                for(int j=0; j<tabla.buckets; j++) {
                    currItem = getItem(tabla, queue[j]);
                    if(currItem->value == 0) {
                        victim = j;
                        break;
                    } else
                        currItem->value = 0;
                }
                printf("\nVICTIM: [%d]", queue[victim]);
                deleteItem(tabla, queue[victim]);
                tabla.count--;
                for(int i=victim; i<index-1; i++)
                    queue[i] = queue[i+1];
                queue[index-1] = lista[i];
                insertItem(tabla, lista[i], 1);
                tabla.count++;
            }
        }
        printf("\n");
        displayHash(tabla);
    }
    printf("Total Miss: %d\n", missCount);
    deleteHash(tabla);
}

int main(int argc, char *argv[]) {
    int lista[] = {0, 1, 2, 0, 1, 3, 0, 3, 1, 2, 1};
    LRU_RS(3, lista, 11);
}