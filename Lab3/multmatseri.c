/*
Este programa realiza a multiplicação de duas matrizes de números em ponto flutuante e armazena o resultado em um arquivo de saída.
As matrizes de entrada são lidas a partir de arquivos binários, e o resultado da multiplicação é escrito em um terceiro arquivo binário.
O programa também calcula o tempo total de execução da multiplicação das matrizes.

Instruções de uso:
Compile o código fornecido com um compilador C compatível e execute o programa da seguinte maneira:
para os teste foi compilado com gcc -o saida multmatseri.c timer.h -lm
./nome_do_executavel <arquivo_matriz1> <arquivo_matriz2> <arquivo_saida>

Substitua <arquivo_matriz1>, <arquivo_matriz2> e <arquivo_saida> pelos caminhos dos arquivos das duas matrizes de entrada e o arquivo de saída, respectivamente. Certifique-se de que os arquivos de entrada estejam no formato esperado (matriz representada como um array de floats, seguido pelo número de linhas e colunas como inteiros de 4 bytes cada). O programa calculará a multiplicação das duas matrizes e escreverá o resultado no arquivo de saída, além de imprimir o tempo total de execução em segundos.
*/
#include <stdio.h>
#include <stdlib.h>
#include "time.h" // Inclui a biblioteca time.h

// Função para multiplicação de matrizes
void multiply(float *matrix1, float *matrix2, float *result, int rows1, int cols1, int cols2) {
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols2; j++) {
            float sum = 0.0;
            for (int k = 0; k < cols1; k++) {
                sum += matrix1[i * cols1 + k] * matrix2[k * cols2 + j];
            }
            result[i * cols2 + j] = sum;
        }
    }
}

int main(int argc, char *argv[]) {
    // Verifica se foram fornecidos os argumentos corretos na linha de comando
    if (argc != 4) {
        printf("Uso: %s <arquivo_matriz1> <arquivo_matriz2> <arquivo_saida>\n", argv[0]);
        return 1;
    }

    char *file1 = argv[1];
    char *file2 = argv[2];
    char *outfile = argv[3];

    FILE *fp1, *fp2, *fout;
    int rows1, cols1, rows2, cols2; // Variáveis para as dimensões das matrizes
    fp1 = fopen(file1, "rb");
    fp2 = fopen(file2, "rb");
    fout = fopen(outfile, "wb");

    // Lê as dimensões da primeira matriz
    fread(&rows1, sizeof(int), 1, fp1);
    fread(&cols1, sizeof(int), 1, fp1);

    // Lê as dimensões da segunda matriz
    fread(&rows2, sizeof(int), 1, fp2);
    fread(&cols2, sizeof(int), 1, fp2);

    // Verifica se as dimensões das matrizes são compatíveis para a multiplicação
    if (cols1 != rows2) {
        printf("Erro: O número de colunas da primeira matriz não é igual ao número de linhas da segunda matriz.\n");
        return 1;
    }

    // Aloca memória para as matrizes e a matriz resultante
    float *matrix1 = (float *)malloc(rows1 * cols1 * sizeof(float));
    float *matrix2 = (float *)malloc(rows2 * cols2 * sizeof(float));
    float *result = (float *)malloc(rows1 * cols2 * sizeof(float)); // A matriz resultante não precisa ser inicializada com zeros

    fread(matrix1, sizeof(float), rows1 * cols1, fp1);
    fread(matrix2, sizeof(float), rows2 * cols2, fp2);

    // Início da contagem do tempo total
    clock_t start, finish;
    double elapsed;

    start = clock();

    // Calcula a multiplicação das matrizes sequencialmente
    multiply(matrix1, matrix2, result, rows1, cols1, cols2);

    // Fim da contagem do tempo total
    finish = clock();
    elapsed = (double)(finish - start) / CLOCKS_PER_SEC;

    printf("Tempo total: %.6lf segundos\n", elapsed);

    // Escreve as dimensões da matriz resultante no arquivo de saída
    fwrite(&rows1, sizeof(int), 1, fout);
    fwrite(&cols2, sizeof(int), 1, fout);
    // Escreve os valores da matriz resultante no arquivo de saída
    fwrite(result, sizeof(float), rows1 * cols2, fout);

    // Fecha os arquivos e libera a memória alocada
    fclose(fp1);
    fclose(fp2);
    fclose(fout);
    free(matrix1);
    free(matrix2);
    free(result);

    return 0;
}
