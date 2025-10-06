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

typedef struct sort {
    char tipo[10];
    int trocas;
    int comparacoes;
} Sort;

// Manipulação de arquivos

// Escreve qual modelo de sorting foi usado e quantas trocas e comparações fez


// Cria uma estrutura de sort com os detalhes
Sort createSortItem(char* tipo, long long int swaps, long long int comparacoes) {
    Sort sort;
    sort.comparacoes = comparacoes;
    sort.trocas = swaps;
    strcpy(sort.tipo, tipo);
    return sort;
}

// Adiciona as informações da execução n
void writeSortingData(char* filename, Sort sort) {
    FILE *f = fopen(filename, "ab");
    if (f == NULL) {
        printf("Falhou ao abrir o arquivo '%s'.\n", filename);
        return;
    }
    printf("Salvando %s - Swaps: %d - Comparações: %d", sort.tipo, sort.trocas, sort.comparacoes );
    fseek(f, 0, SEEK_END);
    fwrite(&sort, sizeof(Sort), 1, f);
    fclose(f);
}

// Funções de ordenação

void bubbleSort(int *A, int size) {
    long long int swaps = 0;
    long long int comparisons = 0;
    for (long long int i = 0; i < size; i++) {
        for (long long int j = 0; j < size - i - 1; j++) {
            comparisons++;
            if (A[j] > A[j + 1]) {
                long long int temp = A[j];
                A[j] = A[j + 1];
                A[j + 1] = temp;
                swaps++;
            }
        }
    }
    // Toda execução ele escreve no arquivo binário final os dados
    // EX: BUBBLE 6 15
    // Isso vai ser posteriormente lido pela leitura binária do Python
    writeSortingData("../executions/bubble.bin", createSortItem("bubble", swaps, comparisons));
}