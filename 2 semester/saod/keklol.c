#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int comparisons = 0;
int swaps = 0;

void reset_counters() {
    comparisons = 0;
    swaps = 0;
}

void heapify(int arr[], int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n) {
        comparisons++;
        if (arr[left] > arr[largest]) {
            largest = left;
        }
    }

    if (right < n) {
        comparisons++;
        if (arr[right] > arr[largest]) {
            largest = right;
        }
    }

    if (largest != i) {
        swaps++;
        int temp = arr[i];
        arr[i] = arr[largest];
        arr[largest] = temp;
        heapify(arr, n, largest);
    }
}

void build_heap(int arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }
}

void heap_sort(int arr[], int n) {
    build_heap(arr, n);
    for (int i = n - 1; i > 0; i--) {
        swaps++;
        int temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;
        heapify(arr, i, 0);
    }
}

void print_array(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void generate_decreasing(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = n - i;
    }
}

void generate_random(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 1000;
    }
}

void generate_increasing(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = i + 1;
    }
}

int main() {
    srand(time(NULL));
    int sizes[] = {100, 200, 300, 400, 500};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    printf("Трудоемкость построения пирамиды:\n");
    printf("| N    | M+C теоретич. | Мфакт + Cфакт (Убыв.) | Мфакт + Cфакт (Случ.) | Мфакт + Cфакт (Возр.) |\n");
    printf("|------|---------------|-----------------------|-----------------------|-----------------------|\n");

    for (int i = 0; i < num_sizes; i++) {
        int n = sizes[i];
        int* arr = (int*)malloc(n * sizeof(int));

        // Убывающий массив
        generate_decreasing(arr, n);
        reset_counters();
        build_heap(arr, n);
        int dec_mc = swaps + comparisons;

        // Случайный массив
        generate_random(arr, n);
        reset_counters();
        build_heap(arr, n);
        int rand_mc = swaps + comparisons;

        // Возрастающий массив
        generate_increasing(arr, n);
        reset_counters();
        build_heap(arr, n);
        int inc_mc = swaps + comparisons;

        printf("| %-4d | %-13d | %-21d | %-21d | %-21d |\n", n, 2 * n, dec_mc, rand_mc, inc_mc);
        free(arr);
    }

    printf("\nТрудоемкость пирамидальной сортировки:\n");
    printf("| N    | HeapSort (Mф + Cф) (Убыв.) | HeapSort (Mф + Cф) (Случ.) | HeapSort (Mф + Cф) (Возр.) |\n");
    printf("|------|----------------------------|----------------------------|----------------------------|\n");

    for (int i = 0; i < num_sizes; i++) {
        int n = sizes[i];
        int* arr = (int*)malloc(n * sizeof(int));

        // Убывающий массив
        generate_decreasing(arr, n);
        reset_counters();
        heap_sort(arr, n);
        int dec_mc = swaps + comparisons;

        // Случайный массив
        generate_random(arr, n);
        reset_counters();
        heap_sort(arr, n);
        int rand_mc = swaps + comparisons;

        // Возрастающий массив
        generate_increasing(arr, n);
        reset_counters();
        heap_sort(arr, n);
        int inc_mc = swaps + comparisons;

        printf("| %-4d | %-26d | %-26d | %-26d |\n", n, dec_mc, rand_mc, inc_mc);
        free(arr);
    }

    return 0;
}
