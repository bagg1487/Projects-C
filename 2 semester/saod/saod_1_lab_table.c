#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void generateKnuthSequence(int n, int sequence[], int *size) {
    int h = 1, i = 0;
    while (h < n) {
        sequence[i++] = h;
        h = 3 * h + 1;
    }
    *size = i;
}

void shellSort(int arr[], int n, int *M, int *C) {
    int sequence[20], size;
    generateKnuthSequence(n, sequence, &size);
    
    for (int g = size - 1; g >= 0; g--) {
        int gap = sequence[g];
        for (int i = gap; i < n; i++) {
            int temp = arr[i];
            int j;
            (*M)++;
            for (j = i; j >= gap && arr[j - gap] > temp; j -= gap) {
                (*C)++;
                arr[j] = arr[j - gap];
                (*M)++;
            }
            arr[j] = temp;
            (*M)++;
        }
    }
}

void insertionSort(int arr[], int n, int *M, int *C) {
    for (int i = 1; i < n; i++) {
        int temp = arr[i];
        int j = i - 1;
        (*M)++;
        while (j >= 0 && arr[j] > temp) {
            (*C)++;
            arr[j + 1] = arr[j];
            (*M)++;
            j--;
        }
        arr[j + 1] = temp;
        (*M)++;
    }
}

void generateArray(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 1000;
    }
}

int main() {
    srand(time(NULL));
    int sizes[] = {100, 200, 300, 400, 500};
    int numSizes = sizeof(sizes) / sizeof(sizes[0]);
    
    printf("| %-6s | %-25s | %-15s | %-15s |\n", "n", "h1 … hm по формуле Д.Кнута", "Insert Mф+Cф", "Shell Mф+Cф");
    
    
    for (int i = 0; i < numSizes; i++) {
        int n = sizes[i];
        int arr1[n], arr2[n];
        int M_insert = 0, C_insert = 0, M_shell = 0, C_shell = 0;
        
        generateArray(arr1, n);
        for (int j = 0; j < n; j++) arr2[j] = arr1[j];
        
        insertionSort(arr1, n, &M_insert, &C_insert);
        shellSort(arr2, n, &M_shell, &C_shell);
        
        int sequence[20], size;
        generateKnuthSequence(n, sequence, &size);
        
        printf("| %-6d | ", n);
        for (int j = 0; j < size; j++) {
            printf("%d ", sequence[j]);
        }
        printf("%*s | %-15d | %-7d |\n", 25 - (size * 2), "", M_insert + C_insert, M_shell + C_shell);
    }
    
    
    return 0;
}
