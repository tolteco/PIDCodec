#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <process.h>
#define VER 1

/*
* Algoritmo de codificacao de imagens bitmap em arquivo de video formato tvf
*/

FILE *IN; //Arquivo de entrada
FILE *OUT; //Arquivo de saida

int L; //4 bytes sendo lidos
int X; //Auxiliar de L
int Larg, Altu, LarN; //Largura e altura padrao
//Matrizes: Matriz[Altura][Largura]
unsigned char N[1080][5760]; //Matriz da imagem atual (sendo processada)
unsigned char T[1080][1920]; //Matriz da imagem anterior (Ja processada)
unsigned char P[255][3]; //Paleta de cores para acesso direto (sem calculos)
unsigned char O[1080][1920]; //Matriz com a imagem antiga (para diferenca)
unsigned short V[2073600];

//Efetua quantizacao por Floyd–Steinberg (Paleta uniforme)
void quantiza(){
  int i, j, h;
  unsigned char NR, NG, NB, QP; //Novos R, G e B, Posicao de quantizacao da paleta
  char ER, EG, EB; //Erros de quantizacao
  short B; //Calculo de sinal
  LarN = (Larg*3)-3;
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
  i = 0;
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
  T[0][Larg-1] = (round(N[0][LarN]/51.0f) * 42) + (round(N[0][LarN+1]/42.5f) * 6) + round(N[0][LarN+2]/51.0f);
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

//Calcula a diferenca entra a imagem nova(T) e a anterior(O) e
void diferenca(){
  int i, j, Aux, h;
  h = 0;
  for (i = 0; i < Altu; i++){
    for (j = 0; j < Larg; j++){ //Algum desses ifs deve poder cair !!!!! OLHAR
      if (O[i][j] == T[i][j]){
        T[i][j] = 252; //252 codigo para "cor nao muda" ja que a paleta so vai ate 251
      } else {
        O[i][j] = T[i][j];
      }
      if(i == 0 && j == 0){ //Me doi mas nao deu de outro jeito
        V[0] = T[0][0];
      }
      if ((V[h] & 255) == T[i][j]){
        if ((V[h] & 65280) == 65280){ //Limite
          h++;
          V[h] = T[i][j] + 256;
        }
        V[h] += 256; //Acrescenta um a contagem de igualdade
      } else {
        h++;
        V[h] = T[i][j] + 256;
      }
    }
  }
  X = h;
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
  L = (Larg << 20) + (Altu << 8) + (VER << 4); //Escrita do cabecalho do video
  fwrite(&L, sizeof(int), 1, OUT);
  gerapaleta();

  //Leitura do resto do cabecalho nao importa
  fseek(IN, 54, SEEK_SET);
  h = 0;
  for (i = 0; i < Altu; i++){ //Leitura da primeira imagem
    fread(&N[i], 1, Larg * 3, IN);
    //fwrite(&N[i], 1, Larg * 3, OUT);
  }
  //fflush(OUT);

  //Quantizacao da primeira imagem
  quantiza();
  for (i = 0; i < Altu; i++) { //Preenche a primeira imagem
    for (j = 0; j < Larg; j++) {
      O[i][j] = T[i][j];
    }
  }
  for (i = 0; i < Altu; i++) {
    //T[i][j] = N[i][j];
    fwrite(&T[i], 1, Larg, OUT);
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
    system("ECHO %TIME%");
    quantiza();
    /*system("ECHO %TIME%");
    diferenca();
    system("ECHO %TIME%");
    fwrite(&V[i], sizeof(unsigned short), X, OUT);*/
    for (i = 0; i < Altu; i++) {
      //T[i][j] = N[i][j];
      fwrite(&T[i], 1, Larg, OUT);
    }
    fflush(OUT);
    fclose(IN);
  } //Fim para cada arquivo
  fclose(OUT);
  return 0;
}
