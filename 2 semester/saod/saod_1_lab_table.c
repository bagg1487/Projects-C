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
                (*swaps) += 3;
            }
        }
    }
}

// Шейкерная сортировка с подсчетом операций
void shakerSort(int arr[], int n, int *comparisons, int *swaps) {
    *comparisons = 0;
    *swaps = 0;
    int left = 0, right = n - 1;
    int k = n;

    do {
        for (int j = right; j > left; j--) {
            (*comparisons)++;
            if (arr[j] < arr[j - 1]) {
                swap(&arr[j], &arr[j - 1]);
                (*swaps) += 3;
                k = j;
            }
        }
        left = k;

        for (int j = left; j < right; j++) {
            (*comparisons)++;
            if (arr[j] > arr[j + 1]) {
                swap(&arr[j], &arr[j + 1]);
                (*swaps) += 3;
                k = j;
            }
        }
        right = k;
    } while (left < right);
}

// Сортировка вставками с подсчетом операций
void insertSort(int arr[], int n, int *comparisons, int *swaps) {
    *comparisons = 0;
    *swaps = 0;
    for (int i = 1; i < n; i++) {
        int t = arr[i];
        (*swaps)++;
        int j = i - 1;
        while (j >= 0) {
            (*comparisons)++;
            if (t < arr[j]) {
                arr[j + 1] = arr[j];
                (*comparisons)++;
                j--;
            } else {
                break;
            }
        }
        arr[j + 1] = t;
        (*swaps)++;
    }
}

// Сортировка выбором с подсчетом операций
void selectSort(int arr[], int n, int *comparisons, int *swaps) {
    *comparisons = 0;
    *swaps = 0;
    for (int i = 0; i < n - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < n; j++) {
            (*comparisons)++;
            if (arr[j] < arr[minIndex]) {
                minIndex = j;
            }
        }
        if (minIndex != i) {
            swap(&arr[i], &arr[minIndex]);
            (*swaps) += 3;
        }
    }
}

int main() {
    srand(time(NULL));
    int sizes[SIZE] = {100, 200, 300, 400, 500};
    int arr[MAX];
    
    // Первая таблица: SelectSort, BubbleSort, ShakerSort, InsertSort (для случайных чисел)
    printf("| n   | Select Sort (Случ) | Bubble Sort (Случ) | Shaker Sort (Случ) | Insert Sort (Случ) |\n");
    printf("|-----|--------------------|--------------------|--------------------|--------------------|\n");
    
    for (int i = 0; i < SIZE; i++) {
        int n = sizes[i];
        int comparisons, swaps;
        
        // Сортировка выбором
        generateArray(arr, n, 0);
        selectSort(arr, n, &comparisons, &swaps);
        int selectRand = comparisons + swaps;
        
        // Пузырьковая сортировка
        generateArray(arr, n, 0);
        bubbleSort(arr, n, &comparisons, &swaps);
        int bubbleRand = comparisons + swaps;
        
        // Шейкерная сортировка
        generateArray(arr, n, 0);
        shakerSort(arr, n, &comparisons, &swaps);
        int shakerRand = comparisons + swaps;
        
        // Сортировка вставками
        generateArray(arr, n, 0);
        insertSort(arr, n, &comparisons, &swaps);
        int insertRand = comparisons + swaps;
        
        // Вывод результатов
        printf("| %3d | %18d | %18d | %18d | %18d |\n", n, selectRand, bubbleRand, shakerRand, insertRand);
    }

    // Вторая таблица: InsertSort (остается без изменений)
    printf("\n| n   | Insert Sort (Убыв) | Insert Sort (Случ) | Insert Sort (Возр) |\n");
    printf("|-----|--------------------|--------------------|--------------------|\n");
    
    for (int i = 0; i < SIZE; i++) {
        int n = sizes[i];
        int comparisons, swaps;
        
        // Сортировка вставками
        generateArray(arr, n, -1);
        insertSort(arr, n, &comparisons, &swaps);
        int insertDesc = comparisons + swaps;
        
        generateArray(arr, n, 0);
        insertSort(arr, n, &comparisons, &swaps);
        int insertRand = comparisons + swaps;
        
        generateArray(arr, n, 1);
        insertSort(arr, n, &comparisons, &swaps);
        int insertAsc = comparisons + swaps;
        
        // Вывод результатов
        printf("| %3d | %18d | %18d | %18d |\n", n, insertDesc, insertRand, insertAsc);
    }

    return 0;
}
