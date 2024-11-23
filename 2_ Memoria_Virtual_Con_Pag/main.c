#include "header.h"
#include "fileread.c"
#include "algoritmos_reemplazo.c"
#include "tabla_hash.c"

int main(int argc, char *argv[]){
  int size;
  if(argc == 7){
    if(strcmp(argv[1],"-m") == 0){
      size = atoi(argv[2]);
      if(size>0){
	if(strcmp(argv[3],"-a") == 0){
	  if(strcmp(argv[5],"-f") == 0){
	    int largo;
	    int *lista = readReferences(argv[6],&largo);
	    if(strcmp(argv[4],"FIFO") == 0){
	      FIFO(size, lista, largo);
	      return 0;
	    } else if(strcmp(argv[4],"LRU") == 0){
	      LRU(size, lista, largo);
	      return 0;
	    } else if(strcmp(argv[4],"LRU_RELOJ") == 0){
	      LRU_RS(size, lista, largo);
	      return 0;
	    } else if(strcmp(argv[4],"OPTIMO") == 0){
	      OPTM(size, lista, largo);
	      return 0;
	    }
	  }
	}
      }
    }
  }
  printf("uso: %s -m TAMAÑO_TABLA -a ALGORITMO -f archivo_de_referencias\n",argv[0]);
  printf("Algoritmos: FIFO, LRU, LRU_RELOJ, OPTIMO\n");
  return 1;
}
