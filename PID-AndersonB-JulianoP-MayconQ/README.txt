Alunos: Anderson Bottega da Silva
        Juliano Felipe Prass da Silva
	Maycon Queiroz de Oliveira



Observações:  

- O executável "Codif.exe" tem que estar na mesma pasta que o .jar.
- A pasta "MPID" é o projeto NetBeans do Player
- O arquivo "Codif.c" é referente ao "Codec"
- Há uma pasta com os dois executáveis para comodidade
- A compilação do código em .C é "normal", isto é, "Codif.c -o Codif.exe". 
  Nota-se que o nome do executável "Codif.exe" foi hardcoded no código em Java.


- Erros do código em C são retornados conforme:
  -2  Erro na soma da contagem de frequencias 
  -1  Erro nao catalogado
   0   Sem erros
   1   Quantidade de parametros invalida
   2   Arquivo de entrada nao encontrado
   3   Arquivo de saida nao pode ser aberto
   4   Arquivo sem assinatura de bitmap
   5   Imagem com paleta
   6   Imagem com dimensoes maiores que 1920x1080 nao sao suportadas
