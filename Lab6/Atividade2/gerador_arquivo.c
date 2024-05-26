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
