#include "header.h"

void OPTM(int size, int *lista, int largo) {
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
      printf("HIT: [%d]", item);
    }
    else {
      printf("MISS: [%d]", item);
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
	
	deleteItem(tabla,sleepiest->key);
	tabla.count--;
	insertItem(tabla, item, nextappear);
	tabla.count++;
      }
    }
    printf("\n");
    displayHash(tabla);
  }
  printf("Total Miss: %d\n", missCount);
  deleteHash(tabla);
}

void FIFO(int size, int *lista, int largo) {
    hashTable tabla = newHashTable(size);
    int missCount = 0;
    int queue[size];
    int index = 0;
    for(int i=0; i<largo; i++) {
        if(searchItem(tabla, lista[i]))
            printf("\n[%d]: HIT", lista[i]);
        else {
            printf("\n[%d]: MISS", lista[i]);
            missCount++;
            if(insertItem(tabla, lista[i], 0)) {
                tabla.count++;
                queue[index] = lista[i];
                index = (index+1)%size;
            } else {
                printf(" -> VICTIM: [%d]", queue[index]);
                deleteItem(tabla, queue[index]);
                tabla.count--;
                queue[index] = lista[i];+
                insertItem(tabla, lista[i], 0);
                tabla.count++;
                index = (index+1)%size;
            }
        }
    }
    printf("\n\nN%cmero de fallos: %d\n\n", 163, missCount);
    deleteHash(tabla);
}

void LRU(int size, int *lista, int largo) {
    hashTable tabla = newHashTable(size);
    int missCount = 0;
    int position = 0;
    item *currItem;
    for(int i=0; i<largo; i++) {
        if(searchItem(tabla, lista[i])) {
            printf("\n[%d]: HIT", lista[i]);
            getItem(tabla, lista[i])->value = position;
        } else {
            printf("\n[%d]: MISS", lista[i]);
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
                printf(" -> VICTIM: [%d]", victim->key);
                deleteItem(tabla, victim->key);
                tabla.count--;
                insertItem(tabla, lista[i], position);
                tabla.count++;
            }
        }
        position++;
    }
    printf("Total Miss: %d\n", missCount);
    deleteHash(tabla);
}


void LRU_RS(int size, int *lista, int largo) {
  
  hashTable tabla = newHashTable(size);
  int missCount = 0;
  int queue[size];
  int index = 0;
  item *currItem;
  item *victim;
  int position = 0;
  
  for(int i=0; i<largo; i++) {
    if(searchItem(tabla, lista[i])) {
      printf("HIT: [%d]", lista[i]);
      getItem(tabla, lista[i])->value = 1;
      for(int j=0;j<size;j++){
        if(lista[i] == queue[j]){
          position = j;
          break;
        }
      }
      if(position == index) {
        index = (index + 1) % size; 
      } else {
        victim = currItem;
        for(int k = 0; k<size; k++){
          if((position+k)%size != (index+size-1)%size){
            queue[(position+k)%size] = queue[(position+k+1)%size];
          }else{
            queue[(index+size-1)%size] = lista[i];
            position = (index+size-1)%size;
            break;
          }
        }       
      }
    } else {
      printf("MISS: [%d]", lista[i]);
      missCount++;
      
      if(insertItem(tabla, lista[i], 1)) {
	tabla.count++;
	queue[index] = lista[i];
	index = (index+1) % size;
	
      } else {
	      
	for(int j=0; j<size; j++) {
	  position = (j+index)%size;
	  int select = queue[position];
	  currItem = getItem(tabla, select);
	  
	  if(currItem->value == 0) {

	    if(position == index){
	      victim = currItem;
	      index = (index + 1) % size;
	      break;
	    } else {
	    
	    victim = currItem;
	    for(int k = 0; k<size; k++){
	      if((position+k)%size != (index+size-1)%size){
		queue[(position+k)%size] = queue[(position+k+1)%size];
	      }else{
		queue[(index+size-1)%size] = select;
		position = (index+size-1)%size;
		break;
	      }
	    }
	    break;
	    }
	  }
	  else {
	    currItem->value = 0;
	    if (j >= size - 1) {
	      victim = getItem(tabla, queue[index]);
	      position = index;
	      index = (index + 1) % size;
	    }
	  }
	}
       
	
	printf("\nVICTIM: [%d]", victim->key);
	deleteItem(tabla, victim->key);
	tabla.count--;
	insertItem(tabla, lista[i], 1);
	queue[position] = lista[i];
	tabla.count++;
      }
    }
    printf("\n");
    printf("indice: %d elemento: %d\n",index,queue[index]);
    for(int i= 0; i< size; i++){
      printf("%d ",queue[i]);
    }
    printf("\n");
    displayHash(tabla);
    
  }
  printf("Total Miss: %d\n", missCount);
  deleteHash(tabla);
}

