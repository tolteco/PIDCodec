#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Erros.h"

/*
* Algoritmo de codificacao de imagens bitmap em arquivo de video formato tvf
*/

typedef struct pikcel{
    unsigned char B;
    unsigned char G;
    unsigned char R;
}pikcel;

FILE *IN; //Arquivo de entrada
FILE *OUT; //Arquivo de saida

int L; //4 bytes sendo lidos
int X; //Auxiliar de L
int Larg, Altu; //Largura e altura padrao
pikcel **N; //Matriz da imagem atual (sendo processada)
pikcel **A; //Matriz da imagem anterior (Ja processada) NAO FUNCIONA
pikcel **T; //Matriz da imagem anterior (Ja processada)
unsigned char *P; //Vetor para rapida leitura

//Aloca Matrizes
pikcel aloca(int l, int c){
  int i;
  A=(pikcel **) calloc(l, sizeof(pikcel));
  N=(pikcel **) calloc(l, sizeof(pikcel));
  T=(pikcel **) calloc(l, sizeof(pikcel));
  for(i=0;i<l;i++){
    A[i]=(pikcel *) calloc(c, sizeof(pikcel));
    N[i]=(pikcel *) calloc(c, sizeof(pikcel));
    T[i]=(pikcel *) calloc(c, sizeof(pikcel));
  }
}

/////////////////////////////////////////////////////////// MAIN

int main(int argc, char *argv[]){
  int i, j, k, h;
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
  fseek(IN, 10, SEEK_SET);
  fread(&L, sizeof(L), 1, IN);
  if (L != 54){ //Offset bits tem que ser igual a  para imagem true color (nao trabalhamos com paletas)
    return Erro(5);
  }

  //Tamanho (largura e altura) da imagem
  fseek(IN, 18, SEEK_SET);
  fread(&Larg, 4, 1, IN);
  fread(&Altu, 4, 1, IN);
  aloca(Larg, Altu);
  P = calloc(Larg * Altu * 3, 1);

  //Leitura do resto do cabecalho (nao importa)
  fseek(IN, 28, SEEK_CUR);
  h = 0;
  fread(&P, Larg * Altu * 3, 1, IN);
  for (i = 0; i < Larg; i++) {
    for (j = 0; j < Altu; j++) {
      N[i][j].B = P[h];
      N[i][j].G = P[h+1];
      N[i][j].R = P[h+2];
      T[i][j].B = P[h];
      T[i][j].G = P[h+1];
      T[i][j].R = P[h+2];
      h += 3;
    }
  }

  fclose(IN);
  for (k = 3; k < argc; k++){
    IN = fopen(argv[k],"rb"); //Leitura Binaria
    printf("Leitura de %s\n", argv[k]);
    if (IN == NULL){
      return Erro(2);
    }
    fseek(IN, 10, SEEK_SET);
    fread(&L, sizeof(L), 1, IN);
    if (L != 54){ //Offset bits tem que ser igual a  para imagem true color (nao trabalhamos com paletas)
      return Erro(5);
    }
    fseek(IN, 54, SEEK_SET);
    h = 0;
    fread(&P, Larg * Altu * 3, 1, IN);
    for (i = 0; i < Larg; i++) {
      for (j = 0; j < Altu; j++) {
        N[i][j].B = P[h];
        N[i][j].G = P[h+1];
        N[i][j].R = P[h+2];
        h += 3;
      }
    }

    //Algoritmo de compressao
  }

  return 0;
}
