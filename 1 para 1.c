#include <stdio.h>
#include <pthread.h>
#include <time.h>

void* tarefa() {
    
    //O valor que será interpretado na thread
    int soma = 0;
    for (int i = 0; i < 100000000; i++) {
        soma += i % 3;
    }
    return 0;
}

int main() {

    int numThreads;

    printf("\nDigite o número de threads: ");
    scanf("%d", &numThreads);

    //Reserva partes da memória para os Threads serem alocados
    pthread_t memAlocada[numThreads];

    //Cria um Struct com o inicio e o fim dos tempos cronometrados
    struct timespec inicio, fim;

    //Inicia a contagem do tempo
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    //Cria as threads e as associa com a área da memória alocada
    for (int i = 0; i < numThreads; i++) {
        pthread_create(&memAlocada[i], 0, tarefa, 0);
    }

    //Espera todas as therads (?)
    for (int i = 0; i < numThreads; i++) {
        pthread_join(memAlocada[i], 0);
    }

    //Finaliza a Contagem do tempo
    clock_gettime(CLOCK_MONOTONIC, &fim);

    //Faz a subtração dos tempos em segundos
    double tempoClockSeg = (fim.tv_sec - inicio.tv_sec);
    //Faz a substração dos tempos em nanosegundos, os transforma para segundos e os soma a váriavel
    tempoClockSeg += (fim.tv_nsec - inicio.tv_nsec) / 1000000000.0;
    
    //Faz a conversão de seg para mseg
    double tempoClockMS = tempoClockSeg * 1000.0;

    printf("\nTempo total: %.2f ms\n\n", tempoClockMS);

    return 0;
}