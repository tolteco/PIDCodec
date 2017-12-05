#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define VER 0

/*
* Algoritmo de codificacao de imagens bitmap em arquivo de video formato tvf
*/

/*typedef struct pikcel{
    unsigned char B;
    unsigned char G;
    unsigned char R;
}pikcel;*/

FILE *IN; //Arquivo de entrada
FILE *OUT; //Arquivo de saida

int L; //4 bytes sendo lidos
int X; //Auxiliar de L
int Larg, Altu; //Largura e altura padrao
//Matrizes: Matriz[Largura][Altura]
unsigned char **N; //Matriz da imagem atual (sendo processada)
unsigned char **T; //Matriz da imagem anterior (Ja processada)
//unsigned char *P; //Vetor para rapida leitura

//Aloca Matrizes
void aloca(){
  int i;
  N=(unsigned char **) calloc(Altu, sizeof(unsigned char));
  T=(unsigned char **) calloc(Altu, sizeof(unsigned char));
  for(i=0;i<l;i++){
    N[i]=(unsigned char *) calloc(Larg * 3, sizeof(unsigned char));
    T[i]=(unsigned char *) calloc(Larg, sizeof(unsigned char));
  }
  if (T == NULL || N == NULL){
    exit(-1);
  }
}

//Verifica se ha alteracao de cor nos pixels
void diferenca(){
  int i, j, Aux;
  for (i = 0; i < Altu; i++){
    for (j = 0; j < Larg; j++){
      Aux = T[i][j] - N[i][j];
      if(N[i][j] != T[i][j]){
        T[i][j] = N[i][j];
        N[i][j] = Aux;
      }
      N[i][j] = Aux;
    }
  }
}

//Efetua quantizacao por Floyd–Steinberg (Paleta uniforme)
void quantiza(){
  int i, j;
  unsigned char NR, NG, NB, QP; //Novo pixel, Posicao de quantizacao da paleta
  for (i = Altu; i > 0; i++) {
    for (j = 0; j < Larg; j+=3) {
      QP = (round(N[i][j]/51) * 42) + (round(N[i][j+1]/42.5) * 6) + round(N[i][j+2]/51);
      NR = ceil(QP/42) * 51;
      NG = round(ceil((QP % 42) / 6) * 42.5);
      NB = (QP % 42) % 6;
      //Floyd Aqui
    }
  }
}

/////////////////////////////////////////////////////////// MAIN

int main(int argc, char *argv[]){
  int i, j, k, h;
  if (argc < 3){ //Esperado pelo o menos o programa, arquivo de saida e uma imagem
    return 1;
  }
  IN = fopen(argv[2],"rb"); //Leitura Binaria
  if (IN == NULL){
    return 2;
  }
  OUT = fopen(argv[1],"wb"); //Arquivo de saida
  if (OUT == NULL){
    return 3;
  }

  //Leitura do cabecalho
  fread(&L, sizeof(L), 1, IN);
  if ((L & 65535) != 19778){ //BM nao encontrado
    return 4;
  }
  fseek(IN, 10, SEEK_SET);
  fread(&L, sizeof(L), 1, IN);
  if (L != 54){ //Offset bits tem que ser igual a para imagem true color (nao trabalhamos com paletas)
    return 5;
  }

  //Tamanho (largura e altura) da imagem
  fseek(IN, 18, SEEK_SET);
  fread(&Larg, 4, 1, IN);
  fread(&Altu, 4, 1, IN);
  if ((Larg & 4095) != Larg || (Altu & 4095) != Altu){
    return 6;
  }
  aloca();
  L = (Larg << 20) + (Altu << 8) + (VER << 4); //Escrita do cabecalho do video
  fwrite(&L, sizeof(int), 1, OUT);

  //Leitura do resto do cabecalho (nao importa)
  fseek(IN, 28, SEEK_CUR);
  h = 0;
  for (i = 0; i < Altu; i++){ //Leitura da primeira imagem
    fread(&N[i], 1, Larg * 3, IN);
    fwrite(&N[i], 1, Larg * 3, OUT);
  }
  //fflush(OUT);

  //Quantizacao da primeira imagem

  fclose(IN);

  for (k = 3; k < argc; k++){
    IN = fopen(argv[k],"rb"); //Leitura Binaria
    //printf("Leitura de %s\n", argv[k]);
    if (IN == NULL){
      return 2;
    }
    fseek(IN, 10, SEEK_SET);
    fread(&L, sizeof(L), 1, IN);
    if (L != 54){ //Offset bits tem que ser igual a  para imagem true color (nao trabalhamos com paletas)
      return 5;
    }
    fseek(IN, 54, SEEK_SET); //Pula Cabecalho
    h = 0;
    for (i = 0; i < Altu; i++){ //Leitura
      fread(&N[i], 1, Larg * 3, IN);
      fwrite(&N[i], 1, Larg * 3, OUT);
    }
    //Quantizacao da imagem
    //quantiza();

  } //Fim para cada arquivo
  fclose(OUT);
  return 0;
}
