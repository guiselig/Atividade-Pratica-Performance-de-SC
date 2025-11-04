#include <stdio.h>
#include <pthread.h>
#include <time.h>

#define MAX_TAREFAS 1000
#define MAX_WORKERS 10   // número fixo de cores no sistema, para o M4 da Apple

int tarefasRestantes;   // contador global de tarefas
pthread_mutex_t trava;  // trava para evitar conflito entre threads

void executarTarefa() {
    long soma = 0;
    for (long i = 0; i < 100000000; i++) {
        soma += i % 3;
    }
}

void* worker(void* arg) {
    while (1) {
        pthread_mutex_lock(&trava);
        if (tarefasRestantes <= 0) {
            pthread_mutex_unlock(&trava);
            break;
        }
        tarefasRestantes--;
        pthread_mutex_unlock(&trava);

        executarTarefa();
    }
    return 0;
}

int main() {
    
    int numTarefas;

    printf("\nDigite o número de tarefas (máx %d): ", MAX_TAREFAS);
    scanf("%d", &numTarefas);

    if (numTarefas > MAX_TAREFAS) numTarefas = MAX_TAREFAS;
    if (numTarefas < 1) numTarefas = 1;

    tarefasRestantes = numTarefas;
    pthread_t workers[MAX_WORKERS];
    pthread_mutex_init(&trava, NULL);

    struct timespec inicio, fim;
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    for (int i = 0; i < MAX_WORKERS; i++) {
        pthread_create(&workers[i], NULL, worker, NULL);
    }
    for (int i = 0; i < MAX_WORKERS; i++) {
        pthread_join(workers[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);

    double tempo = (fim.tv_sec - inicio.tv_sec) * 1000.0;
    tempo += (fim.tv_nsec - inicio.tv_nsec) / 1000000.0;

    printf("\nTempo total: %.2f ms\n", tempo);

    pthread_mutex_destroy(&trava);
    return 0;
}