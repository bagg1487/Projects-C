#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void bubbleSort(int arr[], int n, int *comparisons, int *swaps) {
    int i, j, temp;
    *comparisons = 0;
    *swaps = 0;
    for (i = 0; i < n-1; i++) {
        for (j = 0; j < n-i-1; j++) {
            (*comparisons)++;
            if (arr[j] > arr[j+1]) {
                temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
                (*swaps)+=3;
            }
        }
    }
}

void generateArray(int arr[], int n, int type) {
    for (int i = 0; i < n; i++) {
        if (type == 0) {
            arr[i] = i; // Возрастающий
        } else if (type == 1) {
            arr[i] = n - i; // Убывающий
        } else {
            arr[i] = rand() % 1000; // Случайный
        }
    }
}

int main() {
    srand(time(NULL));
    int sizes[] = {100, 200, 300, 400, 500};
    int numSizes = sizeof(sizes) / sizeof(sizes[0]);

    printf("N  M+C теоретич.  M+C факт. (Убыв.)  M+C факт. (Случ.)  M+C факт. (Возр.)\n");

    for (int i = 0; i < numSizes; i++) {
        int n = sizes[i];
        int arr[n];
        int comparisons, swaps;

        // Убывающий массив
        generateArray(arr, n, 1);
        bubbleSort(arr, n, &comparisons, &swaps);
        int totalDesc = comparisons + swaps;

        // Случайный массив
        generateArray(arr, n, 2);
        bubbleSort(arr, n, &comparisons, &swaps);
        int totalRand = comparisons + swaps;

        // Возрастающий массив
        generateArray(arr, n, 0);
        bubbleSort(arr, n, &comparisons, &swaps);
        int totalAsc = comparisons + swaps;
        int theoretical = 5 * n * (n - 1) / 4;
        printf("%d\t%d\t\t%d\t\t%d\t\t%d\n", n, theoretical, totalDesc, totalRand, totalAsc);
    }

    return 0;
}
