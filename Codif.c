#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Erros.h"

/*
* Algoritmo de codificacao de imagens bitmap em arquivo de video formato tvf
*/

typedef struct pikcel{
    unsigned char R;
    unsigned char G;
    unsigned char B;
}pikcel;

FILE *IN; //Arquivo de entrada
FILE *OUT; //Arquivo de saida

int L; //4 bytes sendo lidos
pikcel **N; //Matriz da imagem atual (sendo processada)
pikcel **A; //Matriz da imagem anterior (Ja processada)

//Aloca Matrizes
pikcel aloca(int l, int c){
  int i;
  A=(pikcel **) calloc(l, sizeof(pikcel));
  N=(pikcel **) calloc(l, sizeof(pikcel));
  for(i=0;i<l;i++){
    A[i]=(pikcel *) calloc(c, sizeof(pikcel));
    N[i]=(pikcel *) calloc(c, sizeof(pikcel));
  }
}

/////////////////////////////////////////////////////////// MAIN

int main(int argc, char *argv[]){
  if (argc < 3){ //Esperado pelo o menos o programa e uma imagem
    return Erro(1);
  }
  IN = fopen(argv[2],"rb"); //Leitura Binaria
  if (IN == NULL){
    return Erro(2);
  }
  OUT = fopen(argv[1],"wb"); //Arquivo de saida
  if (OUT == NULL){
    return Erro(3);
  }

  //Leitura do cabecalho
  fread(&L, sizeof(L), 1, IN);
  if ((L & 65535) != 19778){ //BM nao encontrado
    return Erro(4);
  }
  fread(&L, sizeof(L), 12, IN); //Resto do cabecalho

}
