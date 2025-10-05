#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sorting.h"

int main(){

    int i;
    int vetor[] = {1, 20, -10, 30, 5, 7};
    int tamanhoVetor = (int)sizeof(vetor)/sizeof(int);
    printf("\nVetor original: ");
    for (i = 0 ; i < tamanhoVetor ; i++)
        printf("%d ", vetor[i]);

    printf("\nVetor tamanho = %d\n", tamanhoVetor);

    // bubble sort
    int bubbleVec[tamanhoVetor];
    copia(vetor, bubbleVec, tamanhoVetor);
    bubbleSort(bubbleVec, tamanhoVetor);
    printf("\nBubble sort: ");
    for (i = 0 ; i < tamanhoVetor ; i++)
        printf("%d ", bubbleVec[i]);
    printf("\n");

    // insertion sort

    // quick sort

    return 0;
}
