/*
Este programa realiza a multiplicação de duas matrizes de números em ponto flutuante utilizando múltiplas threads para melhorar o desempenho.
As matrizes de entrada são lidas a partir de arquivos binários, e o resultado da multiplicação é escrito em um terceiro arquivo binário.
O número de threads a serem utilizadas é especificado como um argumento de linha de comando.

Instruções de uso:
Compile o código fornecido com um compilador C compatível e execute o programa da seguinte maneira:
para os teste foi compilado com gcc -o saida multmatconc.c timer.h -lm
./nome_do_executavel <arquivo_matriz1> <arquivo_matriz2> <arquivo_saida> <num_threads>

Substitua <arquivo_matriz1>, <arquivo_matriz2>, <arquivo_saida> e <num_threads> pelos caminhos dos arquivos das duas matrizes de entrada, o arquivo de saída e o número de threads, respectivamente. Certifique-se de que os arquivos de entrada estejam no formato esperado . O programa calculará a multiplicação das duas matrizes utilizando o número de threads especificado e escreverá o resultado no arquivo de saída, além de imprimir o tempo total de execução em segundos.
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h" // Inclui o arquivo timer.h

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

// Função de multiplicação de matriz
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
    if (argc != 5) {
        printf("Uso: %s <arquivo_matriz1> <arquivo_matriz2> <arquivo_saida> <num_threads>\n", argv[0]);
        return 1;
    }

    char *file1 = argv[1];
    char *file2 = argv[2];
    char *outfile = argv[3];
    int num_threads = atoi(argv[4]);

    FILE *fp1, *fp2, *fout;
    int rows1, cols1, rows2, cols2;

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

    float *matrix1 = (float *)malloc(rows1 * cols1 * sizeof(float));
    float *matrix2 = (float *)malloc(rows2 * cols2 * sizeof(float));
    float *result = (float *)malloc(rows1 * cols2 * sizeof(float));

    fread(matrix1, sizeof(float), rows1 * cols1, fp1);
    fread(matrix2, sizeof(float), rows2 * cols2, fp2);

    // Marca o tempo de início da execução
    double start, finish, elapsed;
    GET_TIME(start);

    // Multiplica as matrizes
    multiply(matrix1, matrix2, result, rows1, cols1, cols2);

    // Marca o tempo de fim da execução
    GET_TIME(finish);

    // Calcula o tempo de execução
    elapsed = finish - start;

    printf("Tempo de execução: %.6lf segundos\n", elapsed); // Modificado para usar vírgula

    // Escreve a matriz resultante no arquivo de saída
    fwrite(&rows1, sizeof(int), 1, fout);
    fwrite(&cols2, sizeof(int), 1, fout);
    fwrite(result, sizeof(float), rows1 * cols2, fout);

    fclose(fp1);
    fclose(fp2);
    fclose(fout);

    free(matrix1);
    free(matrix2);
    free(result);

    return 0;
}
