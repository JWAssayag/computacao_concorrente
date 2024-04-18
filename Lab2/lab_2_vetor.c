#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_TAMANHO_VETOR 10000 

void gerar_vetor_e_salvar(const char *nome_arquivo) {
    srand(time(NULL));
    int tamanho = rand() % MAX_TAMANHO_VETOR + 1;

    // Abrir arquivo para escrita
    FILE *arquivo = fopen(nome_arquivo, "w");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo para escrita.\n");
        exit(EXIT_FAILURE);
    }

    // Escrever o tamanho do vetor no arquivo
    fprintf(arquivo, "%d\n", tamanho);

    for ( int i; i < tamanho; i++) {
        fprintf(arquivo, "%d\n", i);
    }

    // Fechar o arquivo
    fclose(arquivo);

    printf("Vetor gerado e salvo em '%s'.\n", nome_arquivo);
}

int main() {
    const char *nome_arquivo = "vetor_aleatorio.txt";
    gerar_vetor_e_salvar(nome_arquivo);
    return 0;
}
