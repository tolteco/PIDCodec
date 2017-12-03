#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define VER 0

unsigned char *P; //Vetor para rapida leitura

int main() {
  int i;
  P = calloc(1000000, 1000);
  for (i = 0; i < 1000000; i++) {
    P[i] = i;
  }
  printf("sizeof = %i", sizeof(P)/sizeof(P[0]));
  return 0;
}
