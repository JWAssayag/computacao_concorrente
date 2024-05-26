// Comando de compilação: gcc -o gera_binario gera_binario.c
// Método de uso: ./gera_binario <arquivo_binario> <quantidade_numeros>
// Exemplo de uso: ./gera_binario arquivo.bin 1000
// Isso gerará um arquivo binário chamado 'arquivo.bin' contendo 1000 números aleatórios.
// Os números são gerados aleatoriamente e podem variar a cada execução do programa.


#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <nome_arquivo> <quantidade_numeros>\n", argv[0]);
        return 1;
    }

    const char *nomeArquivo = argv[1];
    int N = atoi(argv[2]);

    FILE *arquivo = fopen(nomeArquivo, "wb");
    if (arquivo == NULL) {
        perror("Erro ao abrir arquivo para escrita");
        return 1;
    }

    srand(time(NULL));
    for (int i = 0; i < N; i++) {
        int num = rand() % 1000; // Gera números aleatórios entre 0 e 999
        fwrite(&num, sizeof(int), 1, arquivo);
    }

    fclose(arquivo);
    printf("Arquivo binário '%s' gerado com sucesso com %d números.\n", nomeArquivo, N);
    return 0;
}
