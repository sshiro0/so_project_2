#include "header.h"

void OPTM(int size, int *lista, int largo, int mostrar) {
  hashTable tabla = newHashTable(size);
  int missCount = 0;
  item *sleepiest;
  item *currItem;
  
  for(int i=0; i<largo; i++){
    int item = lista[i];
    int nextappear = largo+1;
    for(int j=i+1; j<largo; j++){
      if(lista[j] == item){
	nextappear = j;
	break;
      }
    }
      
    if(searchItem(tabla, item)){
      getItem(tabla,item)->value = nextappear;
      if(mostrar == 1){
        printf("\n[%d]: HIT", item);
      }
    }
    else {
      if(mostrar == 1){
	printf("\n[%d]: MISS", item);
      }
      missCount++;
      if(insertItem(tabla, item, nextappear)) {
	tabla.count++;
      }
      else{
	sleepiest = NULL;

	
	for(int j=0; j<tabla.buckets; j++) {
	  if(tabla.table[j] != NULL) {
	    if(sleepiest==NULL)
	      sleepiest = tabla.table[j];
	    currItem = tabla.table[j];
	    while(currItem != NULL) {
	      
	      if(currItem->value > sleepiest->value)
		sleepiest = currItem;
	      
	      currItem = currItem->next;
	    }
	  }
	}
	if(mostrar == 1){
	  printf(" -> VICTIM: [%d]", sleepiest->key);
	}
	deleteItem(tabla,sleepiest->key);
	tabla.count--;
	insertItem(tabla, item, nextappear);
	tabla.count++;
      }
    }
    if(mostrar == 1){
      printf("\n");
      displayHash(tabla);
    }
  }
  printf("\nNúmero de fallos: %d\n", missCount);
  deleteHash(tabla);
}

void FIFO(int size, int *lista, int largo, int mostrar) {
    hashTable tabla = newHashTable(size);
    int missCount = 0;
    int queue[size];
    int index = 0;
    for(int i=0; i<largo; i++) {
      if(searchItem(tabla, lista[i])) {
	  if(mostrar == 1){
            printf("\n[%d]: HIT", lista[i]);
	  }
      } else {
	  
	  if(mostrar == 1){
            printf("\n[%d]: MISS", lista[i]);
	  }
	  
            missCount++;
            if(insertItem(tabla, lista[i], 0)) {
                tabla.count++;
                queue[index] = lista[i];
                index = (index+1)%size;
            } else {
	      
	      if(mostrar == 1){
                printf(" -> VICTIM: [%d]", queue[index]);
	      }
                deleteItem(tabla, queue[index]);
                tabla.count--;
                queue[index] = lista[i];+
                insertItem(tabla, lista[i], 0);
                tabla.count++;
                index = (index+1)%size;
            }
        }

	if(mostrar == 1){
	  printf("\n");
	  displayHash(tabla);
	}
    }
    printf("\nNúmero de fallos: %d\n", missCount);
    deleteHash(tabla);
}

void LRU(int size, int *lista, int largo, int mostrar) {
    hashTable tabla = newHashTable(size);
    int missCount = 0;
    int position = 0;
    item *currItem;
    for(int i=0; i<largo; i++) {
        if(searchItem(tabla, lista[i])) {
	  if(mostrar == 1){
            printf("\n[%d]: HIT", lista[i]);
	  }
            getItem(tabla, lista[i])->value = position;
        } else {
	  if(mostrar == 1){
            printf("\n[%d]: MISS", lista[i]);
	  }
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
		if(mostrar == 1){
		  printf(" -> VICTIM: [%d]", victim->key);
		}
                deleteItem(tabla, victim->key);
                tabla.count--;
                insertItem(tabla, lista[i], position);
                tabla.count++;
            }
        }
	if(mostrar == 1){
	  printf("\n");
	  displayHash(tabla);
	}
        position++;
    }
    printf("\nNúmero de fallos: %d\n", missCount);
    deleteHash(tabla);
}


void LRU_RS(int size, int *lista, int largo, int mostrar) {

  hashTable tabla = newHashTable(size);
  int missCount = 0;
  int queue[size];
  int index = 0;
  item *currItem;
  item *victim;
  int position = 0;
  
  for(int i=0; i<largo; i++) {
    if(searchItem(tabla, lista[i])) {
      if(mostrar == 1)
	printf("\n[%d]: HIT", lista[i]);
      getItem(tabla, lista[i])->value = 1;
      
      
    } else {
      if(mostrar == 1)
	printf("\n[%d]: MISS", lista[i]);
      missCount++;
      
      if(insertItem(tabla, lista[i], 1)) {
	queue[index] = lista[i];
	index = (index + 1)%size;
	tabla.count++;
	
      } else {
	      
	for(int j=0; j<size; j++) {
	  position = (j+index)%size;
	  int select = queue[position];
	  currItem = getItem(tabla, select);
	  
	  if(currItem->value == 0) {
	    index = (position + 1)%size;
	    victim = currItem;
	    break;
	  }
	  else {
	    currItem->value = 0;
	    if (j >= size - 1) {
	      position = index;
	      index = (index+1)%size;
	      victim = getItem(tabla, queue[position]);
	    }
	  }
	}
       
	if(mostrar == 1)
	  printf(" -> VICTIM: [%d]", victim->key);
	deleteItem(tabla, victim->key);
	tabla.count--;
	insertItem(tabla, lista[i], 1);
	queue[position] = lista[i];
	tabla.count++;
      }
    }
    if(mostrar == 1){
      printf("\n");
      displayHash(tabla);
    }
  }
  printf("\nNúmero de fallos: %d\n", missCount);
  deleteHash(tabla);
}

