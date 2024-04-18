/*
Instruções de uso:
A entrada deve ser: nome_do_programa <quantidade_de_threads> <arquivo_de_entrada>/<valor>
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_THREADS 16 // Defina o máximo de threads permitido
#define MAX_TAMANHO_VETOR 10000 // Defina o tamanho máximo do vetor

// Estrutura para armazenar os parâmetros das threads
typedef struct {
    float vetor[MAX_TAMANHO_VETOR];
    int tamanho;
    int id_thread;
    int num_threads;
    float soma_parcial;
} ArgumentosThread;

// Função que cada thread executará
void *thread_soma(void *arg) {
    ArgumentosThread *args = (ArgumentosThread *)arg;
    int tamanho_por_parte = args->tamanho / args->num_threads;
    int inicio = args->id_thread * tamanho_por_parte;
    int fim = (args->id_thread + 1) * tamanho_por_parte;
    fim = (fim < args->tamanho) ? fim : args->tamanho;
    
    float soma = 0;
    for (int i = inicio; i < fim; i++) {
        soma += args->vetor[i];
    }

    args->soma_parcial = soma;

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    // Verifica se foram fornecidos argumentos corretos
    if (argc != 2) {
        printf("Uso: %s <arquivo_de_entrada>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Abre o arquivo de entrada
    FILE *arquivo_entrada = fopen(argv[1], "r+");
    if (!arquivo_entrada) {
        printf("Erro ao abrir o arquivo de entrada.\n");
        exit(EXIT_FAILURE);
    }

    // Lê o tamanho do vetor a partir do arquivo de entrada
    int tamanho;
    fscanf(arquivo_entrada, "%d", &tamanho);
    if (tamanho > MAX_TAMANHO_VETOR) {
        printf("Tamanho do vetor excede o máximo permitido.\n");
        exit(EXIT_FAILURE);
    }

    // Lê os elementos do vetor a partir do arquivo de entrada
    float vetor[MAX_TAMANHO_VETOR];
    for (int i = 0; i < tamanho; i++) {
        fscanf(arquivo_entrada, "%f", &vetor[i]);
    }

    // Inicializa as threads
    int num_threads = 4; // Defina o número de threads desejado
    pthread_t threads[MAX_THREADS];
    ArgumentosThread args[MAX_THREADS];
    for (int i = 0; i < num_threads; i++) {
        args[i].id_thread = i;
        args[i].num_threads = num_threads;
        args[i].tamanho = tamanho;
        for (int j = 0; j < tamanho; j++) {
            args[i].vetor[j] = vetor[j];
        }
        pthread_create(&threads[i], NULL, thread_soma, (void *)&args[i]);
    }

    // Aguarda todas as threads terminarem
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // Soma os resultados parciais
    float soma_total = 0;
    for (int i = 0; i < num_threads; i++) {
        soma_total += args[i].soma_parcial;
    }

    // Escreve a soma total no arquivo de entrada
    fseek(arquivo_entrada, 0, SEEK_END);
    fprintf(arquivo_entrada, "\nSoma total dos elementos: %.2f\n", soma_total);
    fclose(arquivo_entrada);

    return 0;
}
