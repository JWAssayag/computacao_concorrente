#include <stdio.h>
#include <pthread.h>

#define NTHREAD 10
int vetor[NTHREAD];

void inicializar_vetor() {
    for (int i = 0; i < NTHREAD; i++) {
        vetor[i] = i + 1;
    }
}

void *elevar_ao_quadrado(void *arq) {
    int ident = *(int *)arq;
    vetor[ident] *= vetor[ident];
    printf("Thread %d!\n", (ident+1));
    pthread_exit(NULL);
}

void imprimir_vetor() {
    for (int i = 0; i < NTHREAD; i++) {
        printf("%d ", vetor[i]);
    }
    printf("\n");
}

int main(void) {
    inicializar_vetor();
    printf("Vetor Inicial!\n");
    imprimir_vetor();
    pthread_t tid[NTHREAD];
    int ident[NTHREAD];
    for (int i = 0; i < NTHREAD; i++) {
        ident[i] = i;
        if (pthread_create(&tid[i], NULL, elevar_ao_quadrado, (void *)&ident[i])) {
            printf("ERRO -- pthread create\n");
        }
    }

    for (int i = 0; i < NTHREAD; i++) {
        if (pthread_join(tid[i], NULL)) {
            printf("ERRO -- pthread join\n");
        }
    }
    printf("Vetor Final!\n");
    for (int i = 0; i < NTHREAD; i++) {
        printf("%d ", vetor[i]);
    }
    printf("\n");

    printf("Thread principal!\n");
    pthread_exit(NULL);

    return 0;
}
