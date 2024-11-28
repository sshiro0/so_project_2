#include "header.h"

int* readReferences(char *filename, int *size){
  FILE *f;
  f = fopen(filename,"r");
  if(!f){
    perror("Error al abrir archivo");
    return NULL;
  }

  int num;
  int num_count = 0;
  while(fscanf(f, "%d", &num) == 1){
    num_count++;
  }

  if(num_count == 0){
    printf("No hay referencias en archivo\n");
    fclose(f);
    return NULL;
  }

  int *list = (int*)malloc(num_count*sizeof(int));
  if(!list){
    perror("Error al asignar memoria");
    fclose(f);
    return NULL;
  }

  rewind(f);
  int i = 0;
  while(fscanf(f, "%d", &num) == 1){
    list[i++] = num;
  }

  fclose(f);
  *size = num_count;
  return list;
}
