/* Programa que cria arquivo com uma matriz de valores do tipo float, gerados aleatoriamente 
 * Entrada: dimensoes da matriz (linhas e colunas) e nome do arquivo de saida
 * Saida: arquivo binario com as dimensoes (valores inteiros) da matriz (linhas e colunas), 
 * seguido dos valores (float) de todas as celulas da matriz gerados aleatoriamente
 * */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
   Programa para gerar duas matrizes aleatórias de tamanho especificado
   e salvar cada uma delas em um arquivo binário.

   Uso: ./nome_do_programa <linhas> <colunas> <arquivo_saida1> <arquivo_saida2>

   Parâmetros:
     - linhas: número de linhas das matrizes
     - colunas: número de colunas das matrizes
     - arquivo_saida1: nome do arquivo binário para salvar a primeira matriz
     - arquivo_saida2: nome do arquivo binário para salvar a segunda matriz
*/

// Função para gerar uma matriz aleatória
float** gerarMatriz(int linhas, int colunas) {
    float** matriz = (float**)malloc(linhas * sizeof(float*));
    for (int i = 0; i < linhas; i++) {
        matriz[i] = (float*)malloc(colunas * sizeof(float));
        for (int j = 0; j < colunas; j++) {
            matriz[i][j] = (float)rand() / RAND_MAX; // Valor aleatório entre 0 e 1
        }
    }
    return matriz;
}

// Função para salvar uma matriz em um arquivo binário
void salvarMatriz(float** matriz, int linhas, int colunas, char* nomeArquivo) {
    FILE* arquivo = fopen(nomeArquivo, "wb");
    if (arquivo == NULL) {
        perror("Erro ao criar o arquivo");
        exit(EXIT_FAILURE);
    }

    // Escrevendo as dimensões da matriz no arquivo
    fwrite(&linhas, sizeof(int), 1, arquivo);
    fwrite(&colunas, sizeof(int), 1, arquivo);

    // Escrevendo os elementos da matriz no arquivo
    for (int i = 0; i < linhas; i++) {
        fwrite(matriz[i], sizeof(float), colunas, arquivo);
    }

    fclose(arquivo);
}

// Função para liberar a memória alocada para a matriz
void liberarMatriz(float** matriz, int linhas) {
    for (int i = 0; i < linhas; i++) {
        free(matriz[i]);
    }
    free(matriz);
}

int main(int argc, char *argv[]) {
    if (argc < 5) {
        fprintf(stderr, "Digite: %s <linhas> <colunas> <arquivo_saida1> <arquivo_saida2>\n", argv[0]);
        return 1;
    }

    int linhas = atoi(argv[1]);
    int colunas = atoi(argv[2]);
    char* nomeArquivo1 = argv[3];
    char* nomeArquivo2 = argv[4];

    srand(time(NULL)); // Inicializa o gerador de números aleatórios com o tempo atual

    // Gerando a primeira matriz aleatória
    float** matriz1 = gerarMatriz(linhas, colunas);
    printf("Matriz 1 gerada:\n");
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            printf("%.2f\t", matriz1[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    // Gerando a segunda matriz aleatória
    float** matriz2 = gerarMatriz(linhas, colunas);
    printf("Matriz 2 gerada:\n");
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            printf("%.2f\t", matriz2[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    // Salvando as matrizes em arquivos binários
    salvarMatriz(matriz1, linhas, colunas, nomeArquivo1);
    salvarMatriz(matriz2, linhas, colunas, nomeArquivo2);

    // Liberando memória
    liberarMatriz(matriz1, linhas);
    liberarMatriz(matriz2, linhas);

    return 0;
}

