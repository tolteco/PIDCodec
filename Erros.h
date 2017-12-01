#include <string.h>
#include <stdlib.h>

int Erro(int A){
  if (A == 0){
    printf ("Erro nao catalogado\n");
    return 1;
  }
  if (A == 1){
    printf ("Quantidade de parametros invalida\n");
    return 1;
  }
  if (A == 2){
    printf ("Arquivo de entrada nao encontrado\n");
    return 1;
  }
  if (A == 3){
    printf ("Arquivo de saida nao pode ser aberto\n");
    return 1;
  }
  if (A == 4){
    printf ("Arquivo sem assinatura de bitmap\n");
    return 1;
  }
  if (A == 5){
    printf ("Imagem com paleta\n");
    return 1;
  }
  if (A == 3){
    printf ("\n");
    return 1;
  }
}
