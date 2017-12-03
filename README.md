# PIDCodec

## Formato tvf

### Cabeçalho

Contém 4 bytes e é composto por:

| Offset (Bits) | Tamanho (bits) | Descrição |
| ------------- | ------------- | -------------- |
| 0 | 4 | Versão do arquivo |
| 4 | 4 | Reservado |
| 8 | 12 | Largura do quadro |
| 20 | 12 | Altura do quadro |

## Codificador
Codifica arquivos em formato bitmap em arquivo de video de formato tvf

### Chamada
```
Codif [opções] saida arquivos
arquivos : Lista de arquivos bitmap que serão convertidos par arquivo de vídeo
saida    : Nome do arquivo de saida (extensão .tvf requerida)
opções   :
  Sem opções até o momento
```

### Estruturas de programação

Estrutura de cada pixel (alocação realizada ao início, com a largura e altura da primeira imagem)
```
typedef struct pikcel{
    unsigned char B;
    unsigned char G;
    unsigned char R;
}pikcel;
```
