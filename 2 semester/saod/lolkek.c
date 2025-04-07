#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    int comparisons;
    int swaps;
} Metrics;

void heapify(int arr[], int n, int i, Metrics *m) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n) {
        m->comparisons++;
        if (arr[left] > arr[largest]) 
            largest = left;
    }

    if (right < n) {
        m->comparisons++;
        if (arr[right] > arr[largest]) 
            largest = right;
    }

    if (largest != i) {
        m->swaps++;
        int temp = arr[i];
        arr[i] = arr[largest];
        arr[largest] = temp;
        
        heapify(arr, n, largest, m);
    }
}

void buildHeap(int arr[], int n, Metrics *m) {
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i, m);
}

void heapSort(int arr[], int n, Metrics *m) {
    buildHeap(arr, n, m);
    
    for (int i = n - 1; i > 0; i--) {
        m->swaps++;
        int temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;

        heapify(arr, i, 0, m);
    }
}

void generateArray(int arr[], int n, int type) {
    if (type == 0) { // случайный
        for (int i = 0; i < n; i++) 
            arr[i] = rand() % 1000;
    } else if (type == 1) { // убывающий
        for (int i = 0; i < n; i++) 
            arr[i] = n - i;
    } else { // возрастающий
        for (int i = 0; i < n; i++) 
            arr[i] = i + 1;
    }
}

void printTableHeader(const char *title) {
    printf("\n%s\n", title);
    printf("+------+-------+---------------+-------------+---------------+\n");
    printf("|  N   | Теор  | Случайный     |  Убывающий  | Возрастающий  |\n");
    printf("+------+-------+---------------+-------------+---------------+\n");
}

void printTableRow(int n, int teor, int random, int descending, int ascending) {
    printf("| %4d | %d | %10d | %10d | %10d |\n", n, teor, random, descending, ascending);
}

void printTableFooter() {
    printf("+------+-------+---------------+--------------+--------------+\n");
}

/*
void printTableHeader1(const char *title) {
    printf("\n%s\n", title);
    printf("+------+---------------+-------------+---------------+\n");
    printf("|  N   |  Случайный    |  Убывающий   | Возрастающий |\n");
    printf("+------+---------------+-------------+---------------+\n");
}

void printTableRow1(int n, int random, int descending, int ascending) {
    printf("| %4d | %10d | %10d | %10d |\n", n, random, descending, ascending);
}

void printTableFooter1() {
    printf("+------+---------------+--------------+--------------+\n");
}
*/

int main() {
    srand(time(NULL));

    int sizes[] = {100, 200, 300, 400, 500};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    
    printTableHeader("Трудоемкость построения пирамиды:");
    
    for (int i = 0; i < num_sizes; i++) {
        int n = sizes[i];
        int results[3] = {0};
        int teor[] = {350, 700, 1050, 1400, 1750};
        
        for (int t = 0; t < 3; t++) {
            int arr[n];
            Metrics m = {0, 0};
            
            generateArray(arr, n, t);
            buildHeap(arr, n, &m);
            
            results[t] = m.comparisons + m.swaps;
        }
        
        printTableRow(n, teor[i], results[0], results[1], results[2]);
    }
    printTableFooter();
    
    printTableHeader("Трудоемкость пирамидальной сортировки:");
    
    for (int i = 0; i < num_sizes; i++) {
        int n = sizes[i];
        int results[3] = {0};
        int teor[] = {2741, 6084, 9653, 13370, 17196};
        
        for (int t = 0; t < 3; t++) {
            int arr[n];
            Metrics m = {0, 0};
            
            generateArray(arr, n, t);
            heapSort(arr, n, &m);
            
            results[t] = m.comparisons + m.swaps;
        }
        
        printTableRow(n, teor[i], results[0], results[1], results[2]);
    }
    printTableFooter();
    
    return 0;
}
