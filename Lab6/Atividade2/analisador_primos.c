// Comando de compilação: gcc -o analisador_primos analisador_primos.c -lm -pthread
// Método de uso: ./analisador_primos <arquivo_binario>
// Exemplo de uso: ./analisador_primos arquivo.bin
// Isso processará os números contidos no arquivo binário fornecido e imprimirá os primos encontrados.

// Observação todas as linhas de print de inserção e retirada no buffer foram comentadas para evitar a poluição visual de informação dorante os testes e tbm as linhas de comando de orint de determinação se o numero é primo ou não
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <math.h>

#define TAMANHO_BUFFER 10

int buffer[TAMANHO_BUFFER];
int entrada = 0, saida = 0;
int totalPrimos = 0;
sem_t vazio, cheio, mutex;

int *primos; // Array para armazenar primos encontrados
int indicePrimos = 0; // Índice para o array de primos

// Função para verificar se um número é primo
int ehPrimo(long long int n) {
    int i;
    if (n <= 1) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    for (i = 3; i <= sqrt(n); i += 2) {
        if (n % i == 0) return 0;
    }
    return 1;
}

void *produtor(void *arg) {
    const char *nomeArquivo = (const char *)arg;
    FILE *arquivo = fopen(nomeArquivo, "rb");
    if (arquivo == NULL) {
        perror("Erro ao abrir arquivo");
        exit(EXIT_FAILURE);
    }

    int num;
    printf("Valores no arquivo:\n");
    while (fread(&num, sizeof(int), 1, arquivo) > 0) {
        printf("%d ", num);
        sem_wait(&vazio);
        sem_wait(&mutex);

        buffer[entrada] = num;
        //printf("\nProdutor: inseriu %d no buffer\n", num); 
        entrada = (entrada + 1) % TAMANHO_BUFFER;

        sem_post(&mutex);
        sem_post(&cheio);
    }
    printf("\n");

    // Colocar um sinalizador de término no buffer para os consumidores
    for (int i = 0; i < TAMANHO_BUFFER; i++) {
        sem_wait(&vazio);
        sem_wait(&mutex);

        buffer[entrada] = -1;  // -1 sinaliza o término
        entrada = (entrada + 1) % TAMANHO_BUFFER;

        sem_post(&mutex);
        sem_post(&cheio);
    }

    fclose(arquivo);
    pthread_exit(NULL);
}

void *consumidor(void *arg) {
    while (1) {
        sem_wait(&cheio);
        sem_wait(&mutex);

        int num = buffer[saida];
        saida = (saida + 1) % TAMANHO_BUFFER;

        sem_post(&mutex);
        sem_post(&vazio);

        if (num == -1) break; // Condição de término

        //printf("Consumidor: retirou %d do buffer\n", num);
        if (ehPrimo(num)) {
            //printf("Consumidor: %d é primo\n", num);
            primos[indicePrimos++] = num;
            totalPrimos++;
        } else {
            //printf("Consumidor: %d não é primo\n", num);
        }
    }

    pthread_exit(NULL);
}

int contar_numeros(const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "rb");
    if (arquivo == NULL) {
        perror("Erro ao abrir arquivo");
        exit(EXIT_FAILURE);
    }

    fseek(arquivo, 0, SEEK_END);
    long tamanhoArquivo = ftell(arquivo);
    fclose(arquivo);

    return tamanhoArquivo / sizeof(int);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <nome_arquivo>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *nomeArquivo = argv[1];
    int numValores = contar_numeros(nomeArquivo);

    // Aloca dinamicamente o array de primos com base no número de valores no arquivo
    primos = (int *)malloc(numValores * sizeof(int));
    if (primos == NULL) {
        perror("Erro ao alocar memória para o array de primos");
        exit(EXIT_FAILURE);
    }

    // Inicialização de semáforos
    sem_init(&vazio, 0, TAMANHO_BUFFER);
    sem_init(&cheio, 0, 0);
    sem_init(&mutex, 0, 1);

    // Criação de threads
    pthread_t produtor_thread, consumidor_thread;
    if (pthread_create(&produtor_thread, NULL, produtor, (void *)nomeArquivo) != 0) {
        fprintf(stderr, "Erro ao criar thread produtora.\n");
        exit(EXIT_FAILURE);
    }
    if (pthread_create(&consumidor_thread, NULL, consumidor, NULL) != 0) {
        fprintf(stderr, "Erro ao criar thread consumidora.\n");
        exit(EXIT_FAILURE);
    }

    // Aguarda o término das threads
    pthread_join(produtor_thread, NULL);
    pthread_join(consumidor_thread, NULL);

    // Exibe o total de números primos encontrados
    printf("Total de números primos encontrados: %d\n", totalPrimos);
    printf("Números primos encontrados:\n");
    for (int i = 0; i < totalPrimos; i++) {
        printf("%d ", primos[i]);
    }
    printf("\n");

    // Libera memória alocada para o array de primos
    free(primos);

    // Destroi semáforos
    sem_destroy(&vazio);
    sem_destroy(&cheio);
    sem_destroy(&mutex);

    return 0;
}
