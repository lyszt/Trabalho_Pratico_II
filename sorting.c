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
    printf("\nSalvando %s - Swaps: %lld - Comparações: %lld", sort.tipo, sort.trocas, sort.comparacoes );
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

void copia(int *src, int *dst, int n) {
    if (src == NULL || dst == NULL) {
        // Se por algum motivo um ponteiro for NULL, apenas retorna (evita crash).
        return;
    }
    for (int i = 0; i < n; i++) {
        dst[i] = src[i];
    }
}

/* insertionSort:
 * implementação clássica do Insertion Sort (estável)
 * arr: ponteiro para o vetor
 * n: número de elementos
 */
void insertionSort(int *arr, int n) {
    if (arr == NULL || n <= 1) return; // nada a fazer para vetores nulos ou de tamanho 0/1

    for (int i = 1; i < n; i++) {
        int chave = arr[i];      // valor a ser inserido na posição correta
        int j = i - 1;           // índice para varrer os anteriores

        // desloca todos os elementos maiores que 'chave' uma posição à direita
        while (j >= 0 && arr[j] > chave) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }

        // insere a chave na posição correta
        arr[j + 1] = chave;
    }
}

/* selectionSort:
 * implementação clássica do Selection Sort (não estável)
 * arr: ponteiro para o vetor
 * n: número de elementos
 */
void selectionSort(int *arr, int n) {
    if (arr == NULL || n <= 1) return; // nada a fazer para vetores nulos ou de tamanho 0/1

    for (int i = 0; i < n - 1; i++) {
        int minIndex = i; // assume que o menor está na posição i

        // encontra o índice do menor elemento no restante do vetor
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[minIndex]) {
                minIndex = j;
            }
        }

        // troca arr[i] com arr[minIndex] se necessário
        if (minIndex != i) {
            int temp = arr[i];
            arr[i] = arr[minIndex];
            arr[minIndex] = temp;
        }
    }
}
