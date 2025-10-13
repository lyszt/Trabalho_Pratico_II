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
    long long total_swaps = 0;
    long long total_comparisons = 0;
    double total_time = 0.0;

    // Executa 3 vezes para obter o tempo médio
    for (int exec = 0; exec < 3; exec++) {
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

        clock_t end = clock();
        total_time += (double)(end - start) / CLOCKS_PER_SEC;
        total_swaps += swaps;
        total_comparisons += comparisons;
    }

    // Calcula as médias
    double tempo = total_time / 3.0;
    long long avg_swaps = total_swaps / 3;
    long long avg_comparisons = total_comparisons / 3;

    // Toda execução ele escreve no arquivo binário final os dados
    // EX: BUBBLE 6 15
    // Isso vai ser posteriormente lido pela leitura binária do Python
    // Corrigido para passar a variável 'swaps' que foi usada na função
    writeSortingData("./executions/bubble.bin", createSortItem("bubble", avg_swaps, avg_comparisons, tempo));
}



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
    long long total_swaps = 0;
    long long total_comparisons = 0;
    double total_time = 0.0;

    // Executa 3 vezes para obter o tempo médio
    for (int exec = 0; exec < 3; exec++) {
        long long swaps = 0;
        long long comparisons = 0;

        clock_t start = clock();

        // Chamada inicial para a função recursiva que faz o trabalho
        quicksort_recursive(A, 0, size - 1, &swaps, &comparisons);

        clock_t end = clock();
        total_time += (double)(end - start) / CLOCKS_PER_SEC;
        total_swaps += swaps;
        total_comparisons += comparisons;
    }

    // Calcula as médias
    double tempo = total_time / 3.0;
    long long avg_swaps = total_swaps / 3;
    long long avg_comparisons = total_comparisons / 3;

    // Salva os dados no arquivo específico do Quicksort
    writeSortingData("./executions/quicksort.bin", createSortItem("quicksort", avg_swaps, avg_comparisons, tempo));
}

// Parte do vinicius abaixo

/* insertionSort:
 * implementação clássica do Insertion Sort (estável)
 * arr: ponteiro para o vetor
 * n: número de elementos
 */
void insertionSort(int *arr, int n) {
    if (arr == NULL || n <= 1) return; // nada a fazer

    long long total_trocas = 0;
    long long total_comparacoes = 0;
    double total_time = 0.0;

    // Executa 3 vezes para obter o tempo médio
    for (int exec = 0; exec < 3; exec++) {
        long long trocas = 0;
        long long comparacoes = 0;

        clock_t start = clock();

        for (int i = 1; i < n; i++) {
            int chave = arr[i];
            int j = i - 1;

            // Desloca todos os elementos maiores que 'chave' uma posição à direita.
            // O laço while contém a principal fonte de comparações.
            while (j >= 0 && (comparacoes++, arr[j] > chave)) {
                arr[j + 1] = arr[j];
                trocas++; // Cada deslocamento é contado como uma "troca"/movimentação.
                j = j - 1;
            }

            // Insere a chave na posição correta.
            arr[j + 1] = chave;
            // O insertion sort não faz uma "troca" clássica, mas sim uma inserção.
            // Contamos os deslocamentos como "trocas" para medir a movimentação de dados.
        }

        clock_t end = clock();
        total_time += (double)(end - start) / CLOCKS_PER_SEC;
        total_trocas += trocas;
        total_comparacoes += comparacoes;
    }

    // Calcula as médias
    double tempo = total_time / 3.0;
    long long avg_trocas = total_trocas / 3;
    long long avg_comparacoes = total_comparacoes / 3;

    writeSortingData("./executions/insertion.bin", createSortItem("insertion", avg_trocas, avg_comparacoes, tempo));
}

/* selectionSort:
 * implementação clássica do Selection Sort (não estável)
 * arr: ponteiro para o vetor
 * n: número de elementos
 */
void selectionSort(int *arr, int n) {

    if (arr == NULL || n <= 1) return;

    long long total_trocas = 0;
    long long total_comparacoes = 0;
    double total_time = 0.0;

    // Executa 3 vezes para obter o tempo médio
    for (int exec = 0; exec < 3; exec++) {
        long long trocas = 0;
        long long comparacoes = 0;

        clock_t start = clock();

        for (int i = 0; i < n - 1; i++) {
            int minIndex = i; // assume que o menor está na posição i

            // Encontra o índice do menor elemento no restante do vetor
            for (int j = i + 1; j < n; j++) {
                comparacoes++; // Uma comparação é feita a cada iteração do laço interno.
                if (arr[j] < arr[minIndex]) {
                    minIndex = j;
                }
            }

            // Troca arr[i] com arr[minIndex] se um menor elemento foi encontrado.
            if (minIndex != i) {
                int temp = arr[i];
                arr[i] = arr[minIndex];
                arr[minIndex] = temp;
                trocas++;
            }
        }

        clock_t end = clock();
        total_time += (double)(end - start) / CLOCKS_PER_SEC;
        total_trocas += trocas;
        total_comparacoes += comparacoes;
    }

    // Calcula as médias
    double tempo = total_time / 3.0;
    long long avg_trocas = total_trocas / 3;
    long long avg_comparacoes = total_comparacoes / 3;

    writeSortingData("./executions/selection.bin", createSortItem("selection", avg_trocas, avg_comparacoes, tempo));
}

/* heapify:
 * Função que organiza o heap (árvore)
 * arr: vetor
 * n: tamanho do heap
 * i: posição do nó pai
 * swaps: contador de trocas
 * comparisons: contador de comparações
 */
void heapify(int *arr, int n, int i, long long *swaps, long long *comparisons) {
    int maior = i;           // O pai começa como o maior
    int esquerda = 2 * i + 1;  // Filho da esquerda
    int direita = 2 * i + 2;   // Filho da direita

    // Verifica se filho da esquerda é maior que o pai
    if (esquerda < n) {
        (*comparisons)++;
        if (arr[esquerda] > arr[maior]) {
            maior = esquerda;
        }
    }

    // Verifica se filho da direita é maior que o atual maior
    if (direita < n) {
        (*comparisons)++;
        if (arr[direita] > arr[maior]) {
            maior = direita;
        }
    }

    // Se o maior não é o pai, troca e continua organizando
    if (maior != i) {
        // Troca o pai com o maior filho
        int temp = arr[i];
        arr[i] = arr[maior];
        arr[maior] = temp;
        (*swaps)++;

        // Organiza a parte que foi afetada pela troca
        heapify(arr, n, maior, swaps, comparisons);
    }
}

/* heapSort:
 * Algoritmo de ordenação Heap Sort
 * arr: vetor para ordenar
 * n: tamanho do vetor
 * - Organiza o vetor como uma árvore onde cada pai é maior que seus filhos
 * - Pega o maior elemento (topo da árvore) e coloca no final
 * - Reorganiza o resto da árvore
 * - Repete até ordenar tudo
 */
void heapSort(int *arr, int n) {
    if (arr == NULL || n <= 1) return;

    long long total_trocas = 0;
    long long total_comparacoes = 0;
    double total_time = 0.0;

    // Executa 3 vezes para obter o tempo médio
    for (int exec = 0; exec < 3; exec++) {
        long long trocas = 0;
        long long comparacoes = 0;

        clock_t start = clock();

        // Organiza o vetor do meio para o início
        for (int i = n / 2 - 1; i >= 0; i--) {
            heapify(arr, n, i, &trocas, &comparacoes);
        }

        // Pega o maior (primeiro) e coloca no final, depois reorganiza
        for (int i = n - 1; i > 0; i--) {
            // Move o maior elemento para o final
            int temp = arr[0];
            arr[0] = arr[i];
            arr[i] = temp;
            trocas++;

            // Reorganiza a árvore sem o elemento que já foi ordenado
            heapify(arr, i, 0, &trocas, &comparacoes);
        }

        clock_t end = clock();
        total_time += (double)(end - start) / CLOCKS_PER_SEC;
        total_trocas += trocas;
        total_comparacoes += comparacoes;
    }

    // Calcula as médias
    double tempo = total_time / 3.0;
    long long avg_trocas = total_trocas / 3;
    long long avg_comparacoes = total_comparacoes / 3;

    writeSortingData("./executions/heapsort.bin", createSortItem("heapsort", avg_trocas, avg_comparacoes, tempo));
}
