#include "header.h"
#include "fileread.c"
#include "algoritmos_reemplazo.c"
#include "tabla_hash.c"

int main(int argc, char *argv[]){
  int size;
  int show = 0;
  if(argc >= 7){
    if(strcmp(argv[1],"-m") == 0){
      size = atoi(argv[2]);
      if(size>0){
	if(strcmp(argv[3],"-a") == 0){
	  if(strcmp(argv[5],"-f") == 0){
	    int largo;
	    int *lista = readReferences(argv[6],&largo);
	    if(lista == NULL){
	      printf("Archivo no valido");
	      return 1;
	    }
	    if(argc == 8){
	      if(strcmp(argv[7],"-s") == 0){
		show = 1;
	      } else {
		printf("Argumento: %s no reconocido\n", argv[7]);
		return 1;
	      }
	    }
	    if(strcmp(argv[4],"FIFO") == 0){
	      FIFO(size, lista, largo, show);
	      return 0;
	    } else if(strcmp(argv[4],"LRU") == 0){
	      LRU(size, lista, largo, show);
	      return 0;
	    } else if(strcmp(argv[4],"LRU_RELOJ") == 0){
	      LRU_RS(size, lista, largo, show);
	      return 0;
	    } else if(strcmp(argv[4],"OPTIMO") == 0){
	      OPTM(size, lista, largo, show);
	      return 0;
	    } else {
	      printf("Algoritmo no reconocido\n");
	    }
	  } else {
	    printf("Argumento: %s no reconocido\n", argv[5]);
	  }
	} else {
	  printf("Argumento: %s no reconocido\n", argv[3]);
	}
      } else {
	printf("Argumento: %s no reconocido\n", argv[2]);
      }
    }else {
      printf("Argumento: %s no reconocido\n", argv[1]);
    }
  }
  printf("uso: %s -m TAMAÑO_TABLA -a ALGORITMO -f archivo_de_referencias\n",argv[0]);
  printf("Algoritmos: FIFO, LRU, LRU_RELOJ, OPTIMO\n");
  return 1;
}
