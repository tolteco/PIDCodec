#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#define VER 1

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
unsigned char N[1080][5760]; //Matriz da imagem atual (sendo processada)
unsigned char T[1080][1920]; //Matriz da imagem anterior (Ja processada)
//unsigned char *P; //Vetor para rapida leitura

//Aloca Matrizes
/*void aloca(){
  int i;
  N=(unsigned char **) calloc(Altu, sizeof(unsigned char));
  T=(unsigned char **) calloc(Altu, sizeof(unsigned char));
  for(i=0;i<Altu;i++){
    N[i]=(unsigned char *) calloc(Larg * 3, sizeof(unsigned char));
    T[i]=(unsigned char *) calloc(Larg, sizeof(unsigned char));
  }
  if (T == NULL || N == NULL){
    exit(-1);
  }
}*/

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
  int i, j, h;
  unsigned char NR, NG, NB, QP; //Novos R, G e B, Posicao de quantizacao da paleta
  char ER, EG, EB; //Erros de quantizacao
  for (i = Altu-1; i > 0; i--) {
    for (j = 0; j < (Larg * 3) - 3; j+=3) {
      //N[i][j] = B, N[i][j+1] = G, N[i][j+2] = R
      QP = (round(N[i][j]/51.0f) * 42) + (round(N[i][j+1]/42.5f) * 6) + round(N[i][j+2]/51.0f);
      NB = floor(QP/42.0f) * 51;
      NG = round(floor((QP % 42) / 6.0f) * 42.5f);
      NR = ((QP % 42) % 6) * 51;
      //Floyd Steinberg
      ER = N[i][j+2] - NR; //Erro
      EG = N[i][j+1] - NG;
      EB = N[i][j  ] - NB;
      N[i  ][j+3] += round(EB * 0.4375); //L
      N[i  ][j+4] += round(EG * 0.4375);
      N[i  ][j+5] += round(ER * 0.4375);
      N[i-1][j-3] += round(EB * 0.1875); //SO
      N[i-1][j-2] += round(EG * 0.1875);
      N[i-1][j-1] += round(ER * 0.1875);
      N[i-1][j  ] += round(EB * 0.3125); //S
      N[i-1][j+1] += round(EG * 0.3125);
      N[i-1][j+2] += round(ER * 0.3125);
      N[i-1][j+3] += round(EB * 0.0625); //SE
      N[i-1][j+4] += round(EG * 0.0625);
      N[i-1][j+5] += round(ER * 0.0625);
      N[i][j%3] = QP;
    }
    //Ultima coluna
    QP = (round(N[i][j]/51.0f) * 42) + (round(N[i][j+1]/42.5f) * 6) + round(N[i][j+2]/51.0f);
    NB = floor(QP/42.0f) * 51;
    NG = round(floor((QP % 42) / 6.0f) * 42.5f);
    NR = ((QP % 42) % 6) * 51;
    ER = N[i][j+2] - NR; //Erro
    EG = N[i][j+1] - NG;
    EB = N[i][j  ] - NB;
    N[i-1][j-3] += round(EB * 0.1875); //SO
    N[i-1][j-2] += round(EG * 0.1875);
    N[i-1][j-1] += round(ER * 0.1875);
    N[i-1][j  ] += round(EB * 0.3125); //S
    N[i-1][j+1] += round(EG * 0.3125);
    N[i-1][j+2] += round(ER * 0.3125);
    N[i-1][j+3] += round(EB * 0.0625); //SE
    N[i-1][j+4] += round(EG * 0.0625);
    N[i-1][j+5] += round(ER * 0.0625);
    N[i][j%3] = QP;
  }
  for (j = 0; j < Larg - 3; j+=3) {
    QP = (round(N[0][j]/51.0f) * 42) + (round(N[0][j+1]/42.5f) * 6) + round(N[0][j+2]/51.0f);
    NB = floor(QP/42.0f) * 51;
    NG = round(floor((QP % 42) / 6.0f) * 42.5f);
    NR = ((QP % 42) % 6) * 51;
    ER = N[0][j+2] - NR; //Erro
    EG = N[0][j+1] - NG;
    EB = N[0][j  ] - NB;
    N[0][j+3] += round(EB * 0.4375); //L
    N[0][j+4] += round(EG * 0.4375);
    N[0][j+5] += round(ER * 0.4375);
    N[0][j%3] = QP;
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
  //aloca();
  L = (Larg << 20) + (Altu << 8) + (VER << 4); //Escrita do cabecalho do video
  fwrite(&L, sizeof(int), 1, OUT);

  //Leitura do resto do cabecalho (nao importa)
  fseek(IN, 28, SEEK_CUR);
  h = 0;
  for (i = 0; i < Altu; i++){ //Leitura da primeira imagem
    fread(&N[i], 1, Larg * 3, IN);
    //fwrite(&N[i], 1, Larg * 3, OUT);
  }
  //fflush(OUT);

  //Quantizacao da primeira imagem
  quantiza();
  /*for (i = 0; i < Altu; i++) {
    for (j = 0; j < Larg; j+=3) {
      T[i][j] = N[i][j];
    }
  }*/
  for (i = 0; i < Altu; i++) {
    fwrite(&N[i], 1, Larg, OUT);
  }
  fflush(OUT);

  fclose(IN);

  for (k = 3; k < argc; k++){
    IN = fopen(argv[k],"rb"); //Leitura Binaria
    printf("Leitura de %s\n", argv[k]);
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
      //fwrite(&N[i], 1, Larg * 3, OUT);
    }
    //Quantizacao da imagem
    quantiza();
    //escreveT();
  } //Fim para cada arquivo
  fclose(OUT);
  return 0;
}
