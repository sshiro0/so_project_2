#include "tabla_hash.c"

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
  item *victim;
  int position = 0;
  
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

int main(int argc, char *argv[]) {
    int lista[] = {0, 1, 2, 0, 1, 3, 0, 3, 1, 2, 1};
    LRU_RS(3, lista, 11);
}
