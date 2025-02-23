// #include <stdio.h>
// #include <stdlib.h>
// #include <time.h>

// void bubbleSort(int arr[], int n, int *comparisons, int *swaps) {
//     int i, j, temp;
//     *comparisons = 0;
//     *swaps = 0;
//     for (i = 0; i < n-1; i++) {
//         for (j = 0; j < n-i-1; j++) {
//             (*comparisons)++;
//             if (arr[j] > arr[j+1]) {
//                 temp = arr[j];
//                 arr[j] = arr[j+1];
//                 arr[j+1] = temp;
//                 (*swaps)+=3;
//             }
//         }
//     }
// }

// void generateArray(int arr[], int n, int type) {
//     for (int i = 0; i < n; i++) {
//         if (type == 0) {
//             arr[i] = i; // Возрастающий
//         } else if (type == 1) {
//             arr[i] = n - i; // Убывающий
//         } else {
//             arr[i] = rand() % 1000; // Случайный
//         }
//     }
// }

// int main() {
//     srand(time(NULL));
//     int sizes[] = {100, 200, 300, 400, 500};
//     int numSizes = sizeof(sizes) / sizeof(sizes[0]);

//     printf("N  M+C теоретич.  M+C факт. (Убыв.)  M+C факт. (Случ.)  M+C факт. (Возр.)\n");

//     for (int i = 0; i < numSizes; i++) {
//         int n = sizes[i];
//         int arr[n];
//         int comparisons, swaps;

//         // Убывающий массив
//         generateArray(arr, n, 1);
//         bubbleSort(arr, n, &comparisons, &swaps);
//         int totalDesc = comparisons + swaps;

//         // Случайный массив
//         generateArray(arr, n, 2);
//         bubbleSort(arr, n, &comparisons, &swaps);
//         int totalRand = comparisons + swaps;

//         // Возрастающий массив
//         generateArray(arr, n, 0);
//         bubbleSort(arr, n, &comparisons, &swaps);
//         int totalAsc = comparisons + swaps;
//         int theoretical = 5 * n * (n - 1) / 4;
//         printf("%d\t%d\t\t%d\t\t%d\t\t%d\n", n, theoretical, totalDesc, totalRand, totalAsc);
//     }

//     return 0;
// }

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 5
#define MAX 500

// Функция для обмена значений
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Генерация массива
void generateArray(int arr[], int n, int type) {
    if (type == -1) { // Убывающий порядок
        for (int i = 0; i < n; i++)
            arr[i] = n - i;
    } else if (type == 1) { // Возрастающий порядок
        for (int i = 0; i < n; i++)
            arr[i] = i + 1;
    } else { // Случайный порядок
        for (int i = 0; i < n; i++)
            arr[i] = rand() % MAX;
    }
}

// Пузырьковая сортировка с подсчетом операций
void bubbleSort(int arr[], int n, int *comparisons, int *swaps) {
    *comparisons = 0;
    *swaps = 0;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            (*comparisons)++;
            if (arr[j] > arr[j + 1]) {
                swap(&arr[j], &arr[j + 1]);
                (*swaps)+=3;
            }
        }
    }
}

// Шейкерная сортировка с подсчетом операций
void shakerSort(int arr[], int n, int *comparisons, int *swaps) {
    *comparisons = 0;
    *swaps = 0;
    int left = 0, right = n - 1;
    while (left < right) {
        for (int i = left; i < right; i++) {
            (*comparisons)++;
            if (arr[i] > arr[i + 1]) {
                swap(&arr[i], &arr[i + 1]);
                (*swaps)+=3;
            }
        }
        right--;
        for (int i = right; i > left; i--) {
            (*comparisons)++;
            if (arr[i] < arr[i - 1]) {
                swap(&arr[i], &arr[i - 1]);
                (*swaps)+=3;
            }
        }
        left++;
    }
}

int main() {
    srand(time(NULL));
    int sizes[SIZE] = {100, 200, 300, 400, 500};
    int arr[MAX];
    
    printf("| n   | Bubble Sort (Убыв) | Bubble Sort (Случ) | Bubble Sort (Возр) | Shaker Sort (Убыв) | Shaker Sort (Случ) | Shaker Sort (Возр) |\n");
    printf("|-----|--------------------|--------------------|--------------------|--------------------|--------------------|--------------------|\n");
    
    for (int i = 0; i < SIZE; i++) {
        int n = sizes[i];
        int comparisons, swaps;
        
        // Пузырьковая сортировка
        generateArray(arr, n, -1);
        bubbleSort(arr, n, &comparisons, &swaps);
        int bubbleDesc = comparisons + swaps;
        
        generateArray(arr, n, 0);
        bubbleSort(arr, n, &comparisons, &swaps);
        int bubbleRand = comparisons + swaps;
        
        generateArray(arr, n, 1);
        bubbleSort(arr, n, &comparisons, &swaps);
        int bubbleAsc = comparisons + swaps;
        
        // Шейкерная сортировка
        generateArray(arr, n, -1);
        shakerSort(arr, n, &comparisons, &swaps);
        int shakerDesc = comparisons + swaps;
        
        generateArray(arr, n, 0);
        shakerSort(arr, n, &comparisons, &swaps);
        int shakerRand = comparisons + swaps;
        
        generateArray(arr, n, 1);
        shakerSort(arr, n, &comparisons, &swaps);
        int shakerAsc = comparisons + swaps;
        
        // Вывод результатов
        printf("| %3d | %18d | %18d | %18d | %18d | %18d | %18d |\n", n, bubbleDesc, bubbleRand, bubbleAsc, shakerDesc, shakerRand, shakerAsc);
    }
    return 0;
}
