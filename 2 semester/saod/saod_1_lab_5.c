// #include <stdio.h>
// #include <stdlib.h>
// #include <time.h>


// void selectionSort(int arr[], int n, int *comparisons, int *swaps) {
//     *comparisons = 0;
//     *swaps = 0;
//     for (int i = 0; i < n - 1; i++) {
//         int min_idx = i;
//         for (int j = i + 1; j < n; j++) {
//             (*comparisons)++; 
//             if (arr[j] < arr[min_idx]) {
//                 min_idx = j;
//             }
//         }
//         if (min_idx != i) {
//             int temp = arr[min_idx];
//             arr[min_idx] = arr[i];
//             arr[i] = temp;
//             (*swaps)++; 
//         }
//     }
// }
// void generateRandomArray(int arr[], int n) {
//     for (int i = 0; i < n; i++) {
//         arr[i] = rand() % 1000;
//     }
// }
// int main() {
//     srand(time(NULL)); 
//     FILE *dataFile = fopen("data.txt", "w");
//     if (dataFile == NULL) {
//         printf("Ошибка открытия файла!\n");
//         return 1;
//     }
//     int start_n = 10;
//     int end_n = 1000;
//     int step = 10;
//     for (int n = start_n; n <= end_n; n += step) {
//         int arr[n];
//         generateRandomArray(arr, n);
//         int comparisons, swaps;
//         selectionSort(arr, n, &comparisons, &swaps); 
//         fprintf(dataFile, "%d %d\n", n, comparisons + swaps);
//     }

//     fclose(dataFile);
//     FILE *gnuplot = popen("gnuplot -persistent", "w");
//     if (gnuplot == NULL) {
//         printf("Ошибка запуска GNUplot!\n");
//         return 1;
//     }
//     fprintf(gnuplot, "set title 'Зависимость Mф + Cф от размера массива n'\n");
//     fprintf(gnuplot, "set xlabel 'Размер массива (n)'\n");
//     fprintf(gnuplot, "set ylabel 'Mф + Cф'\n");
//     fprintf(gnuplot, "plot 'data.txt' with lines title 'Mф + Cф'\n");
//     pclose(gnuplot);
//     return 0;
// }
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Функция для пузырьковой сортировки
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
                (*swaps)++;
            }
        }
    }
}

// Функция для сортировки прямым выбором
void selectSort(int arr[], int n, int *comparisons, int *swaps) {
    int i, j, min_idx, temp;
    *comparisons = 0;
    *swaps = 0;
    for (i = 0; i < n-1; i++) {
        min_idx = i;
        for (j = i+1; j < n; j++) {
            (*comparisons)++;
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            temp = arr[i];
            arr[i] = arr[min_idx];
            arr[min_idx] = temp;
            (*swaps)++;
        }
    }
}

// Функция для генерации случайного массива
void generateArray(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 1000;
    }
}

int main() {
    srand(time(NULL));
    int sizes[] = {100, 200, 300, 400, 500, 600, 700, 800, 900, 1000};
    int numSizes = sizeof(sizes) / sizeof(sizes[0]);

    FILE *fp = fopen("data.txt", "w");
    if (fp == NULL) {
        printf("Ошибка открытия файла!\n");
        return 1;
    }

    fprintf(fp, "N\tBubbleSort_M+C\tSelectSort_M+C\n");

    for (int i = 0; i < numSizes; i++) {
        int n = sizes[i];
        int arr[n];
        int comparisons, swaps;

        // BubbleSort
        generateArray(arr, n);
        bubbleSort(arr, n, &comparisons, &swaps);
        int totalBubble = comparisons + swaps;

        // SelectSort
        generateArray(arr, n);
        selectSort(arr, n, &comparisons, &swaps);
        int totalSelect = comparisons + swaps;

        fprintf(fp, "%d\t%d\t%d\n", n, totalBubble, totalSelect);
    }

    fclose(fp);

    // Использование gnuplot для построения графика
    FILE *gnuplotPipe = popen("gnuplot -persistent", "w");
    if (gnuplotPipe) {
        fprintf(gnuplotPipe, "set title 'Зависимость M+C от размера массива n'\n");
        fprintf(gnuplotPipe, "set xlabel 'Размер массива n'\n");
        fprintf(gnuplotPipe, "set ylabel 'M+C'\n");
        fprintf(gnuplotPipe, "plot 'data.txt' using 1:2 with lines title 'BubbleSort', \
                              'data.txt' using 1:3 with lines title 'SelectSort'\n");
        fclose(gnuplotPipe);
    } else {
        printf("Ошибка при вызове gnuplot!\n");
    }

    return 0;
}
