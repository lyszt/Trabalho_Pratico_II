#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sorting.h"
#include <time.h>
#include <unistd.h>


// Necessário esvaziar os arquivos no inicio da execução
void clean_files() {
    char* sorting_algorithms[] = {"bubble", "selection", "insertion", "quicksort"};
    for (int i = 0; i < 4; i++) {
        char filename[30];

        sprintf(filename, "./executions/%s.bin", sorting_algorithms[i]);
        FILE *file = fopen(filename, "w");
        fclose(file);
    }
}


int main(){
    // No inicio de execução ele limpa os arquivos.
    clean_files();

    int tamanhos[] = {100, 1000, 5000, 10000, 50000, 100000};
    // Calcula quantos tamanhos tem no array
    int num_tamanhos = sizeof(tamanhos) / sizeof(tamanhos[0]);
    // Gerador de numeros aleatórios, inicialização
    srand(time(NULL));

    for (int i = 0; i < num_tamanhos; i++) {
        int tamanho_atual = tamanhos[i];
        printf("\nIniciando testes para vetores de tamanho %d...\n", tamanho_atual);

        // Todo o trabalho para um tamanho específico vai acontecer aqui dentro...

        int *vetor_original = (int *)malloc(tamanho_atual * sizeof(int));
        if (vetor_original == NULL) {
            printf("Falha ao alocar memoria. Abortando.\n");
            return 1;
        }
        for (int j = 0; j < tamanho_atual; j++) {
            // Função demorada se o vetor for grande demais
            vetor_original[j] = rand() % 10000;
        }

        //  Teste para o Bubble Sort 
        int *copia_para_bubble = (int *)malloc(tamanho_atual * sizeof(int));
        copia(vetor_original, copia_para_bubble, tamanho_atual);
        bubbleSort(copia_para_bubble, tamanho_atual);
        free(copia_para_bubble);

        //  Teste para o Insertion Sort
        int *copia_para_insertion = (int *)malloc(tamanho_atual * sizeof(int));
        copia(vetor_original, copia_para_insertion, tamanho_atual);
        insertionSort(copia_para_insertion, tamanho_atual);
        free(copia_para_insertion);

        //  Teste para o Quick Sort 

        int *copia_para_quicksort = (int *)malloc(tamanho_atual * sizeof(int));
        copia(vetor_original, copia_para_quicksort, tamanho_atual);
        quickSort(copia_para_quicksort, tamanho_atual);
        free(copia_para_quicksort);

        // Selection Sort

        int *copia_para_selection = (int *)malloc(tamanho_atual * sizeof(int));
        copia(vetor_original, copia_para_selection, tamanho_atual);
        selectionSort(copia_para_selection, tamanho_atual);
        free(copia_para_selection);
    }



    return 0;
}
