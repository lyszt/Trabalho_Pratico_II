#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sorting.h"


void copia(int *A, int *V, int size){
    int i;
    for (i = 0 ; i < size ; i++)
        V[i] = A[i];
}


void bubbleSort(int *A, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (A[j] > A[j + 1]) {
                int temp = A[j];
                A[j] = A[j + 1];
                A[j + 1] = temp;
            }
        }
    }
}