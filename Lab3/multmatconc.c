/*
Este programa realiza a multiplicação de duas matrizes de números em ponto flutuante utilizando múltiplas threads para melhorar o desempenho.
As matrizes de entrada são lidas a partir de arquivos binários, e o resultado da multiplicação é escrito em um terceiro arquivo binário.
O número de threads a serem utilizadas é especificado como um argumento de linha de comando.

Instruções de uso:
execute o programa da seguinte maneira:
para os teste foi compilado com gcc -o saida multmatconc.c timer.h -lm
./nome_do_executavel <arquivo_matriz1> <arquivo_matriz2> <arquivo_saida> <num_threads>

Substitua <arquivo_matriz1>, <arquivo_matriz2>, <arquivo_saida> e <num_threads> pelos caminhos dos arquivos das duas matrizes de entrada, o arquivo de saída e o número de threads, respectivamente. O programa calculará a multiplicação das duas matrizes utilizando o número de threads especificado e escreverá o resultado no arquivo de saída, além de imprimir o tempo total de execução em segundos.
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

typedef struct {
    int row_start;
    int row_end;
    float *matrix1;
    float *matrix2;
    float *result;
    int rows1;
    int cols1;
    int cols2;
} thread_args;

void *multiply(void *arg) {
    thread_args *args = (thread_args *)arg;
    int row_start = args->row_start;
    int row_end = args->row_end;
    float *matrix1 = args->matrix1;
    float *matrix2 = args->matrix2;
    float *result = args->result;
    int rows1 = args->rows1;
    int cols1 = args->cols1;
    int cols2 = args->cols2;

    for (int i = row_start; i < row_end; i++) {
        for (int j = 0; j < cols2; j++) {
            float sum = 0.0;
            for (int k = 0; k < cols1; k++) {
                sum += matrix1[i * cols1 + k] * matrix2[k * cols2 + j];
            }
            result[i * cols2 + j] = sum;
        }
    }

    pthread_exit(NULL);
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

    float *matrix1 = (float *)malloc(rows1 * cols1 * sizeof(float));
    float *matrix2 = (float *)malloc(rows2 * cols2 * sizeof(float));
    float *result = (float *)malloc(rows1 * cols2 * sizeof(float)); // A matriz resultante não precisa ser inicializada com zeros

    fread(matrix1, sizeof(float), rows1 * cols1, fp1);
    fread(matrix2, sizeof(float), rows2 * cols2, fp2);

    pthread_t threads[num_threads];
    thread_args args[num_threads];

    // Início da contagem do tempo de inicialização
    clock_t start_init = clock();

    // Divide as linhas da primeira matriz entre as threads
    int rows_per_thread = rows1 / num_threads;
    int remaining_rows = rows1 % num_threads;
    int row_start = 0;
    for (int i = 0; i < num_threads; i++) {
        int row_end = row_start + rows_per_thread;
        if (i == num_threads - 1) {
            // A última thread fica com as linhas restantes
            row_end += remaining_rows;
        }
        args[i].row_start = row_start;
        args[i].row_end = row_end;
        args[i].matrix1 = matrix1;
        args[i].matrix2 = matrix2;
        args[i].result = result;
        args[i].rows1 = rows1; // Usamos as linhas da primeira matriz
        args[i].cols1 = cols1; // Usamos as colunas da primeira matriz
        args[i].cols2 = cols2; // Usamos as colunas da segunda matriz

        pthread_create(&threads[i], NULL, multiply, (void *)&args[i]);
        
        row_start = row_end;
    }

    // Fim da contagem do tempo de inicialização
    clock_t end_init = clock();
    double elapsed_init = ((double)(end_init - start_init)) / CLOCKS_PER_SEC;

    // Início da contagem do tempo de processamento
    clock_t start_process = clock();

    // Espera todas as threads terminarem
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // Fim da contagem do tempo de processamento
    clock_t end_process = clock();
    double elapsed_process = ((double)(end_process - start_process)) / CLOCKS_PER_SEC;

    // Início da contagem do tempo de finalização
    clock_t start_finalize = clock();

    fwrite(&rows1, sizeof(int), 1, fout); // Escreve as linhas da matriz resultante no arquivo de saída
    fwrite(&cols2, sizeof(int), 1, fout); // Escreve as colunas da matriz resultante no arquivo de saída
    fwrite(result, sizeof(float), rows1 * cols2, fout); // Escreve os valores da matriz resultante no arquivo de saída

    fclose(fp1);
    fclose(fp2);
    fclose(fout);

    free(matrix1);
    free(matrix2);
    free(result);

    // Fim da contagem do tempo de finalização
    clock_t end_finalize = clock();
    double elapsed_finalize = ((double)(end_finalize - start_finalize)) / CLOCKS_PER_SEC;

    printf("Tempo de inicialização: %.6f segundos\n", elapsed_init);
    printf("Tempo de processamento: %.6f segundos\n", elapsed_process);
    printf("Tempo de finalização: %.6f segundos\n", elapsed_finalize);

    return 0;
}
