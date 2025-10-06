#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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
    double tempo;
} Sort;

// Manipulação de arquivos

// Escreve qual modelo de sorting foi usado e quantas trocas e comparações fez


// Cria uma estrutura de sort com os detalhes
Sort createSortItem(char* tipo, long long swaps, long long comparacoes, double tempo) {
    Sort sort;
    sort.trocas = swaps;
    sort.comparacoes = comparacoes;
    sort.tempo = tempo;
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
    clock_t start = clock();

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
    clock_t end = clock();
    double tempo = (double)(end - start) / CLOCKS_PER_SEC;
    writeSortingData("./executions/bubble.bin", createSortItem("bubble", swaps, comparisons, tempo));
}


// --- Implementação do Quicksort ---

// Função auxiliar para trocar dois elementos e contar a troca
void swap(int* a, int* b, long long* swaps) {
    int t = *a;
    *a = *b;
    *b = t;
    (*swaps)++;
}

/* seleciona o último
   elemento como pivô e o posiciona em seu lugar correto no array ordenado,
   movendo todos os elementos menores para a esquerda e os maiores para a direita. */
int partition(int* A, int low, int high, long long* swaps, long long* comparisons) {
    int pivot = A[high]; // Pivô
    int i = (low - 1);   // Índice do menor elemento

    for (int j = low; j <= high - 1; j++) {
        // Cada iteração do loop realiza uma comparação com o pivô.
        (*comparisons)++;
        if (A[j] < pivot) {
            i++;
            swap(&A[i], &A[j], swaps);
        }
    }
    swap(&A[i + 1], &A[high], swaps);
    return (i + 1);
}

void quicksort_recursive(int* A, int low, int high, long long* swaps, long long* comparisons) {
    if (low < high) {
        // pi é o índice da partição, A[pi] está agora no lugar certo
        int pi = partition(A, low, high, swaps, comparisons);

        // Ordena separadamente os elementos antes e depois da partição
        quicksort_recursive(A, low, pi - 1, swaps, comparisons);
        quicksort_recursive(A, pi + 1, high, swaps, comparisons);
    }
}

void quickSort(int *A, int size) {
    long long swaps = 0;
    long long comparisons = 0;
    clock_t start = clock();

    // Chamada inicial para a função recursiva que faz o trabalho
    quicksort_recursive(A, 0, size - 1, &swaps, &comparisons);

    clock_t end = clock();
    double tempo = (double)(end - start) / CLOCKS_PER_SEC;

    // Salva os dados no arquivo específico do Quicksort
    writeSortingData("./executions/quicksort.bin", createSortItem("quicksort", swaps, comparisons, tempo));
}