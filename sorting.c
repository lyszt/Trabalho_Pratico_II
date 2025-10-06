#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sorting.h"


// Geral
void copia(int *A, int *V, int size){
    int i;
    for (i = 0 ; i < size ; i++)
        V[i] = A[i];
}

// Foi necessário aplicar o packed pra facilitar a leitura pelo python
// Python espera 26 bytes, mas sem o packed aqui fica 32 bytes
typedef struct __attribute__((packed)) sort {
    char tipo[10];
    long long trocas;
    long long comparacoes;
} Sort;

// Manipulação de arquivos

// Escreve qual modelo de sorting foi usado e quantas trocas e comparações fez


// Cria uma estrutura de sort com os detalhes
Sort createSortItem(char* tipo, long long swaps, long long comparacoes) {
    Sort sort;
    sort.trocas = swaps;
    sort.comparacoes = comparacoes;
    memset(sort.tipo, 0, sizeof(sort.tipo));
    strncpy(sort.tipo, tipo, sizeof(sort.tipo)-1);
    return sort;
}


// Adiciona as informações da execução n
void writeSortingData(char* filename, Sort sort) {
    FILE *f = fopen(filename, "ab");
    if (f == NULL) {
        printf("Falhou ao abrir o arquivo '%s'.\n", filename);
        return;
    }
    printf("Salvando %s - Swaps: %lld - Comparações: %lld", sort.tipo, sort.trocas, sort.comparacoes );
    fseek(f, 0, SEEK_END);
    fwrite(&sort, sizeof(Sort), 1, f);
    fclose(f);
}

// Funções de ordenação

void bubbleSort(int *A, int size) {
    long long swaps = 0;
    long long comparisons = 0;
    for (long long i = 0; i < size; i++) {
        for (long long j = 0; j < size - i - 1; j++) {
            comparisons++;
            if (A[j] > A[j + 1]) {
                long long temp = A[j];
                A[j] = A[j + 1];
                A[j + 1] = temp;
                swaps++;
            }
        }
    }
    // Toda execução ele escreve no arquivo binário final os dados
    // EX: BUBBLE 6 15
    // Isso vai ser posteriormente lido pela leitura binária do Python
    writeSortingData("./executions/bubble.bin", createSortItem("bubble", swaps, comparisons));
}