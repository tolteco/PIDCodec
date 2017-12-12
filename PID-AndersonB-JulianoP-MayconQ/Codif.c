#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <pthread.h>
#include <math.h>
#define VER 3

/*
* Algoritmo de codificacao de imagens bitmap em arquivo de video formato tvf
*/

FILE *IN1; //Arquivo de entrada
FILE *IN2; //Arquivo de entrada para thread 2
FILE *OUT0; //Arquivo de saida

int L; //4 bytes sendo lidos
int X; //Auxiliar de L
int Larg, Altu, LarN; //Largura e altura padrao
int Sin; //Sinal entre as threads
int QArg; //Quantidade de argumentos (Para ser global)
int Fini = 1; //Finalizacao da segunda thread
//Matrizes: Matriz[Altura][Largura]
unsigned char N[1080][5760]; //Matriz da imagem com todos os pixels true color
unsigned char T[1080][1920]; //Matriz da imagem processada em Floyd
unsigned char N2[1080][5760]; //Matriz da imagem com todos os pixels true color
unsigned char T2[1080][1920]; //Matriz da imagem processada em Floyd
unsigned char P[255][3]; //Paleta de cores para acesso direto (sem calculos)
char **largs; //argv global

//Efetua quantizacao por Floyd–Steinberg (Paleta uniforme) (Primeira Thread)
void quantiza(){
  int i, j, h;
  unsigned char NR, NG, NB, QP; //Novos R, G e B, Posicao de quantizacao da paleta
  char ER, EG, EB; //Erros de quantizacao
  short B; //Calculo de sinal
  //De baixo para cima, da esquerda para a direita
  for (i = Altu-1; i > 0; i--) {
    //Primeira coluna
    QP = (round(N[i][0]/51.0f) * 42) + (round(N[i][1]/42.5f) * 6) + round(N[i][2]/51.0f);
    ER = N[i][2] - P[QP][0]; //Erro
    EG = N[i][1] - P[QP][1];
    EB = N[i][0] - P[QP][2];
    B = N[i  ][3] + round(EB * 0.4375);
    N[i  ][3] = ((B & 255) == B) ? B : (B & 32768) ? 0 : 255 ; //L
    B = N[i  ][4] + round(EG * 0.4375);
    N[i  ][4] = ((B & 255) == B) ? B : (B & 32768) ? 0 : 255 ;
    B = N[i  ][5] + round(ER * 0.4375);
    N[i  ][5] = ((B & 255) == B) ? B : (B & 32768) ? 0 : 255 ;
    B = N[i-1][0] + round(EB * 0.3125);
    N[i-1][0] = ((B & 255) == B) ? B : (B & 32768) ? 0 : 255 ; //S
    B = N[i-1][1] + round(EG * 0.3125);
    N[i-1][1] = ((B & 255) == B) ? B : (B & 32768) ? 0 : 255 ;
    B = N[i-1][2] + round(ER * 0.3125);
    N[i-1][2] = ((B & 255) == B) ? B : (B & 32768) ? 0 : 255 ;
    B = N[i-1][3] + round(EB * 0.0625);
    N[i-1][3] = ((B & 255) == B) ? B : (B & 32768) ? 0 : 255 ; //SE
    B = N[i-1][4] + round(EG * 0.0625);
    N[i-1][4] = ((B & 255) == B) ? B : (B & 32768) ? 0 : 255 ;
    B = N[i-1][5] + round(ER * 0.0625);
    N[i-1][5] = ((B & 255) == B) ? B : (B & 32768) ? 0 : 255 ;
    T[i][0] = QP;
    for (j = 3; j < LarN; j+=3) {
      //N[i][j] = B, N[i][j+1] = G, N[i][j+2] = R
      QP = (round(N[i][j]/51.0f) * 42) + (round(N[i][j+1]/42.5f) * 6) + round(N[i][j+2]/51.0f);
      ER = N[i][j+2] - P[QP][0]; //Erro
      EG = N[i][j+1] - P[QP][1];
      EB = N[i][j  ] - P[QP][2];
      B = N[i  ][j+3] + round(EB * 0.4375);
      N[i  ][j+3] = ((B & 255) == B) ? B : (B & 32768) ? 0 : 255; //L
      B = N[i  ][j+4] + round(EG * 0.4375);
      N[i  ][j+4] = ((B & 255) == B) ? B : (B & 32768) ? 0 : 255;
      B = N[i  ][j+5] + round(ER * 0.4375);
      N[i  ][j+5] = ((B & 255) == B) ? B : (B & 32768) ? 0 : 255;
      B = N[i-1][j-3] + round(EB * 0.1875);
      N[i-1][j-3] = ((B & 255) == B) ? B : (B & 32768) ? 0 : 255; //SO
      B = N[i-1][j-2] + round(EG * 0.1875);
      N[i-1][j-2] = ((B & 255) == B) ? B : (B & 32768) ? 0 : 255;
      B = N[i-1][j-1] + round(ER * 0.1875);
      N[i-1][j-1] = ((B & 255) == B) ? B : (B & 32768) ? 0 : 255;
      B = N[i-1][j  ] + round(EB * 0.3125);
      N[i-1][j  ] = ((B & 255) == B) ? B : (B & 32768) ? 0 : 255; //S
      B = N[i-1][j+1] + round(EG * 0.3125);
      N[i-1][j+1] = ((B & 255) == B) ? B : (B & 32768) ? 0 : 255;
      B = N[i-1][j+2] + round(ER * 0.3125);
      N[i-1][j+2] = ((B & 255) == B) ? B : (B & 32768) ? 0 : 255;
      B = N[i-1][j+3] + round(EB * 0.0625);
      N[i-1][j+3] = ((B & 255) == B) ? B : (B & 32768) ? 0 : 255; //SE
      B = N[i-1][j+4] + round(EG * 0.0625);
      N[i-1][j+4] = ((B & 255) == B) ? B : (B & 32768) ? 0 : 255;
      B = N[i-1][j+5] + round(ER * 0.0625);
      N[i-1][j+5] = ((B & 255) == B) ? B : (B & 32768) ? 0 : 255;
      T[i][j/3] = QP;
    }
    //Ultima coluna
    j = LarN;
    QP = (round(N[i][j]/51.0f) * 42) + (round(N[i][j+1]/42.5f) * 6) + round(N[i][j+2]/51.0f);
    ER = N[i][LarN+2] - P[QP][0]; //Erro
    EG = N[i][LarN+1] - P[QP][1];
    EB = N[i][LarN  ] - P[QP][2];
    B = N[i-1][j-3] + round(EB * 0.1875);
    N[i-1][j-3] = ((B & 255) == B) ? B : (B & 32768) ? 0 : 255; //SO
    B = N[i-1][j-2] + round(EG * 0.1875);
    N[i-1][j-2] = ((B & 255) == B) ? B : (B & 32768) ? 0 : 255;
    B = N[i-1][j-1] + round(ER * 0.1875);
    N[i-1][j-1] = ((B & 255) == B) ? B : (B & 32768) ? 0 : 255;
    B = N[i-1][j  ] + round(EB * 0.3125);
    N[i-1][j  ] = ((B & 255) == B) ? B : (B & 32768) ? 0 : 255; //S
    B = N[i-1][j+1] + round(EG * 0.3125);
    N[i-1][j+1] = ((B & 255) == B) ? B : (B & 32768) ? 0 : 255;
    B = N[i-1][j+2] + round(ER * 0.3125);
    N[i-1][j+2] = ((B & 255) == B) ? B : (B & 32768) ? 0 : 255;
    T[i][Larg-1] = QP;
  }
  i = 0; //Ultima linha
  for (j = 0; j < LarN; j+=3) {
    QP = (round(N[0][j]/51.0f) * 42) + (round(N[0][j+1]/42.5f) * 6) + round(N[0][j+2]/51.0f);
    ER = N[0][j+2] - P[QP][0]; //Erro
    EG = N[0][j+1] - P[QP][1];
    EB = N[0][j  ] - P[QP][2];
    B = N[i  ][j+3] + round(EB * 0.4375);
    N[i  ][j+3] = ((B & 255) == B) ? B : (B & 32768) ? 0 : 255; //L
    B = N[i  ][j+4] + round(EG * 0.4375);
    N[i  ][j+4] = ((B & 255) == B) ? B : (B & 32768) ? 0 : 255;
    B = N[i  ][j+5] + round(ER * 0.4375);
    N[i  ][j+5] = ((B & 255) == B) ? B : (B & 32768) ? 0 : 255;
    T[0][j/3] = QP;
  }
  //Ultimo pixel
  T[0][Larg-1] = (round(N[0][LarN]/51.0f) * 42) + (round(N[0][LarN+1]/42.5f) * 6) + round(N[0][LarN+2]/51.0f);
}

//Efetua quantizacao por Floyd–Steinberg (Paleta uniforme) (Segunda thread)
void quantizaT2(){
  int i, j, h;
  unsigned char NR, NG, NB, QP; //Novos R, G e B, Posicao de quantizacao da paleta
  char ER, EG, EB; //Erros de quantizacao
  short B; //Calculo de sinal
  //De baixo para cima, da esquerda para a direita
  for (i = Altu-1; i > 0; i--) {
    //Primeira coluna
    QP = (round(N2[i][0]/51.0f) * 42) + (round(N2[i][1]/42.5f) * 6) + round(N2[i][2]/51.0f);
    ER = N2[i][2] - P[QP][0]; //Erro
    EG = N2[i][1] - P[QP][1];
    EB = N2[i][0] - P[QP][2];
    B = N2[i  ][3] + round(EB * 0.4375);
    N2[i  ][3] = ((B & 255) == B) ? B : (B & 32768) ? 0 : 255 ; //L
    B = N2[i  ][4] + round(EG * 0.4375);
    N2[i  ][4] = ((B & 255) == B) ? B : (B & 32768) ? 0 : 255 ;
    B = N2[i  ][5] + round(ER * 0.4375);
    N2[i  ][5] = ((B & 255) == B) ? B : (B & 32768) ? 0 : 255 ;
    B = N2[i-1][0] + round(EB * 0.3125);
    N2[i-1][0] = ((B & 255) == B) ? B : (B & 32768) ? 0 : 255 ; //S
    B = N2[i-1][1] + round(EG * 0.3125);
    N2[i-1][1] = ((B & 255) == B) ? B : (B & 32768) ? 0 : 255 ;
    B = N2[i-1][2] + round(ER * 0.3125);
    N2[i-1][2] = ((B & 255) == B) ? B : (B & 32768) ? 0 : 255 ;
    B = N2[i-1][3] + round(EB * 0.0625);
    N2[i-1][3] = ((B & 255) == B) ? B : (B & 32768) ? 0 : 255 ; //SE
    B = N2[i-1][4] + round(EG * 0.0625);
    N2[i-1][4] = ((B & 255) == B) ? B : (B & 32768) ? 0 : 255 ;
    B = N2[i-1][5] + round(ER * 0.0625);
    N2[i-1][5] = ((B & 255) == B) ? B : (B & 32768) ? 0 : 255 ;
    T2[i][0] = QP;
    for (j = 3; j < LarN; j+=3) {
      //N2[i][j] = B, N2[i][j+1] = G, N2[i][j+2] = R
      QP = (round(N2[i][j]/51.0f) * 42) + (round(N2[i][j+1]/42.5f) * 6) + round(N2[i][j+2]/51.0f);
      ER = N2[i][j+2] - P[QP][0]; //Erro
      EG = N2[i][j+1] - P[QP][1];
      EB = N2[i][j  ] - P[QP][2];
      B = N2[i  ][j+3] + round(EB * 0.4375);
      N2[i  ][j+3] = ((B & 255) == B) ? B : (B & 32768) ? 0 : 255; //L
      B = N2[i  ][j+4] + round(EG * 0.4375);
      N2[i  ][j+4] = ((B & 255) == B) ? B : (B & 32768) ? 0 : 255;
      B = N2[i  ][j+5] + round(ER * 0.4375);
      N2[i  ][j+5] = ((B & 255) == B) ? B : (B & 32768) ? 0 : 255;
      B = N2[i-1][j-3] + round(EB * 0.1875);
      N2[i-1][j-3] = ((B & 255) == B) ? B : (B & 32768) ? 0 : 255; //SO
      B = N2[i-1][j-2] + round(EG * 0.1875);
      N2[i-1][j-2] = ((B & 255) == B) ? B : (B & 32768) ? 0 : 255;
      B = N2[i-1][j-1] + round(ER * 0.1875);
      N2[i-1][j-1] = ((B & 255) == B) ? B : (B & 32768) ? 0 : 255;
      B = N2[i-1][j  ] + round(EB * 0.3125);
      N2[i-1][j  ] = ((B & 255) == B) ? B : (B & 32768) ? 0 : 255; //S
      B = N2[i-1][j+1] + round(EG * 0.3125);
      N2[i-1][j+1] = ((B & 255) == B) ? B : (B & 32768) ? 0 : 255;
      B = N2[i-1][j+2] + round(ER * 0.3125);
      N2[i-1][j+2] = ((B & 255) == B) ? B : (B & 32768) ? 0 : 255;
      B = N2[i-1][j+3] + round(EB * 0.0625);
      N2[i-1][j+3] = ((B & 255) == B) ? B : (B & 32768) ? 0 : 255; //SE
      B = N2[i-1][j+4] + round(EG * 0.0625);
      N2[i-1][j+4] = ((B & 255) == B) ? B : (B & 32768) ? 0 : 255;
      B = N2[i-1][j+5] + round(ER * 0.0625);
      N2[i-1][j+5] = ((B & 255) == B) ? B : (B & 32768) ? 0 : 255;
      T2[i][j/3] = QP;
    }
    //Ultima coluna
    j = LarN;
    QP = (round(N2[i][j]/51.0f) * 42) + (round(N2[i][j+1]/42.5f) * 6) + round(N2[i][j+2]/51.0f);
    ER = N2[i][LarN+2] - P[QP][0]; //Erro
    EG = N2[i][LarN+1] - P[QP][1];
    EB = N2[i][LarN  ] - P[QP][2];
    B = N2[i-1][j-3] + round(EB * 0.1875);
    N2[i-1][j-3] = ((B & 255) == B) ? B : (B & 32768) ? 0 : 255; //SO
    B = N2[i-1][j-2] + round(EG * 0.1875);
    N2[i-1][j-2] = ((B & 255) == B) ? B : (B & 32768) ? 0 : 255;
    B = N2[i-1][j-1] + round(ER * 0.1875);
    N2[i-1][j-1] = ((B & 255) == B) ? B : (B & 32768) ? 0 : 255;
    B = N2[i-1][j  ] + round(EB * 0.3125);
    N2[i-1][j  ] = ((B & 255) == B) ? B : (B & 32768) ? 0 : 255; //S
    B = N2[i-1][j+1] + round(EG * 0.3125);
    N2[i-1][j+1] = ((B & 255) == B) ? B : (B & 32768) ? 0 : 255;
    B = N2[i-1][j+2] + round(ER * 0.3125);
    N2[i-1][j+2] = ((B & 255) == B) ? B : (B & 32768) ? 0 : 255;
    T2[i][Larg-1] = QP;
  }
  i = 0; //Ultima linha
  for (j = 0; j < LarN; j+=3) {
    QP = (round(N2[0][j]/51.0f) * 42) + (round(N2[0][j+1]/42.5f) * 6) + round(N2[0][j+2]/51.0f);
    ER = N2[0][j+2] - P[QP][0]; //Erro
    EG = N2[0][j+1] - P[QP][1];
    EB = N2[0][j  ] - P[QP][2];
    B = N2[i  ][j+3] + round(EB * 0.4375);
    N2[i  ][j+3] = ((B & 255) == B) ? B : (B & 32768) ? 0 : 255; //L
    B = N2[i  ][j+4] + round(EG * 0.4375);
    N2[i  ][j+4] = ((B & 255) == B) ? B : (B & 32768) ? 0 : 255;
    B = N2[i  ][j+5] + round(ER * 0.4375);
    N2[i  ][j+5] = ((B & 255) == B) ? B : (B & 32768) ? 0 : 255;
    T2[0][j/3] = QP;
  }
  //Ultimo pixel
  T2[0][Larg-1] = (round(N2[0][LarN]/51.0f) * 42) + (round(N2[0][LarN+1]/42.5f) * 6) + round(N2[0][LarN+2]/51.0f);
}

//Gera a paleta uniforme (Acesso acelerado)
void gerapaleta(){
  int i, k; //br
  double j; //g
  int h = 0; //contador
  for (i = 0; i < 256; i+=51) {
    for (j = 0.0; j < 256.0; j+=42.5) {
      for (k = 0; k < 256; k+=51) {
        P[h][0] = k;
        P[h][1] = round(j);
        P[h][2] = i;
        h++;
      }
    }
  }
}

//Funcao que a segunda Thread roda
DWORD WINAPI ThreadFunc(){
  int k, h, i;
  for (k = 3; k < QArg; k+=2){
    IN2 = fopen(largs[k],"rb"); //Leitura Binaria
    //printf("T2. Leitura de %s\n", largs[k]);
    if (IN2 == NULL){
      return 2;
    }
    fseek(IN2, 10, SEEK_SET);
    fread(&L, sizeof(L), 1, IN2);
    if (L != 54){ //Offset bits tem que ser igual a  para imagem true color (nao trabalhamos com paletas)
      return 5;
    }
    fseek(IN2, 54, SEEK_SET); //Pula Cabecalho
    h = 0;
    for (i = 0; i < Altu; i++){ //Leitura
      fread(&N2[i], 1, Larg * 3, IN2);
      //fwrite(&N[i], 1, Larg * 3, OUT);
    }
    //Quantizacao da imagem
    quantizaT2();
    while (Sin == 1) {
      //Espera ativa
    }
    for (i = 0; i < Altu; i++) {
      //T[i][j] = N[i][j];
      fwrite(&T2[i], 1, Larg, OUT0);
    }
    fflush(OUT0);
    Sin = 1;
    fclose(IN2);
  } //Fim para cada arquivo
  Fini = 0;
  return 0;
}

/////////////////////////////////////////////////////////// MAIN

int main(int argc, char *argv[]){
  int i, j, k, h;
  if (argc < 3){ //Esperado pelo o menos o programa, arquivo de saida e uma imagem
    return 1;
  }
  IN1 = fopen(argv[2],"rb"); //Leitura Binaria
  if (IN1 == NULL){
    return 2;
  }
  OUT0 = fopen(argv[1],"wb"); //Arquivo de saida
  if (OUT0 == NULL){
    return 3;
  }

  //Leitura do cabecalho
  fread(&L, sizeof(L), 1, IN1);
  if ((L & 65535) != 19778){ //BM nao encontrado
    return 4;
  }
  fseek(IN1, 10, SEEK_SET);
  fread(&L, sizeof(L), 1, IN1);
  if (L != 54){ //Offset bits tem que ser igual a para imagem true color (nao trabalhamos com paletas)
    return 5;
  }

  //Tamanho (largura e altura) da imagem
  fseek(IN1, 18, SEEK_SET);
  fread(&Larg, 4, 1, IN1);
  fread(&Altu, 4, 1, IN1);
  if (Larg > 1920 || Altu > 1080){
    return 6;
  }

  //Coisas que so acontecem se tudo der certo
  L = (Larg << 20) + (Altu << 8) + (VER << 4); //Escrita do cabecalho do video
  fwrite(&L, sizeof(int), 1, OUT0);
  gerapaleta();
  QArg = argc;
  largs = argv;
  Sin = 1;

  //Leitura do resto do cabecalho nao importa
  fseek(IN1, 54, SEEK_SET);
  h = 0;
  for (i = 0; i < Altu; i++){ //Leitura da primeira imagem
    fread(&N[i], 1, Larg * 3, IN1);
    //fwrite(&N[i], 1, Larg * 3, OUT);
  }
  LarN = (Larg*3)-3;
  //fflush(OUT);

  //Chamada da thread filha
  HANDLE thread = CreateThread(NULL, 0, ThreadFunc, NULL, 0, NULL);

  //Quantizacao da primeira imagem
  quantiza();
  while (Sin == 0){
    //Espera ativa
  }
  for (i = 0; i < Altu; i++) {
    //T[i][j] = N[i][j];
    fwrite(&T[i], 1, Larg, OUT0);
  }
  fflush(OUT0);
  Sin = 0; //Sinaliza para a outra thread
  fclose(IN1);

  for (k = 4; k < argc; k+=2){
    IN1 = fopen(argv[k],"rb"); //Leitura Binaria
    //printf("T1. Leitura de %s\n", argv[k]);
    if (IN1 == NULL){
      return 2;
    }
    fseek(IN1, 10, SEEK_SET);
    fread(&L, sizeof(L), 1, IN1);
    if (L != 54){ //Offset bits tem que ser igual a  para imagem true color (nao trabalhamos com paletas)
      return 5;
    }
    fseek(IN1, 54, SEEK_SET); //Pula Cabecalho
    h = 0;
    for (i = 0; i < Altu; i++){ //Leitura
      fread(&N[i], 1, Larg * 3, IN1);
      //fwrite(&N[i], 1, Larg * 3, OUT);
    }
    //Quantizacao da imagem
    quantiza();
    while (Sin == 0) {
      //Espera ativa
    }
    for (i = 0; i < Altu; i++) {
      //T[i][j] = N[i][j];
      fwrite(&T[i], 1, Larg, OUT0);
    }
    fflush(OUT0);
    Sin = 0;
    fclose(IN1);
  } //Fim para cada arquivo
  while (Fini) {
    //Espera Ativa
  }
  fclose(OUT0);
  return 0;
}
