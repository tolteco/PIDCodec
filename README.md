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
Codif saida arquivos
arquivos : Lista de arquivos bitmap que serão convertidos par arquivo de vídeo
saida    : Nome do arquivo de saida (extensão .tvf requerida)
```

### Versões

| Numero | Compressão | Descrição |
| ------ | ---------- | --------- |
| [0](https://github.com/tolteco/PIDCodec/releases/tag/0)     | 1:1        | Bitmaps escritos em sequência |
| [1](https://github.com/tolteco/PIDCodec/releases/tag/1)     | 3:1        | Bitmaps comprimidos de acordo com o algoritmo de Floyd-Steinberg |
| [2](https://github.com/tolteco/PIDCodec/releases/tag/2)     | 3:1        | Implementação _Multi-Thread_, onde cada thread trabalha com um arquivo |
