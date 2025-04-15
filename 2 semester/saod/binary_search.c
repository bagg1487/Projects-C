#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int binary_search(int array[], int size, int search, int *comparisons) {
    int L = 0;
    int R = size - 1;
    
    while (L <= R) {
        int mid = (L + R) / 2;
        (*comparisons)++; 
        if (array[mid] == search) {
            return mid;
        }
        (*comparisons)++;
        if (array[mid] < search) {
            L = mid + 1;
        } else {
            R = mid - 1;
        }
    }
    return -1;
}

int binary_search_up(int array[], int size, int search, int *comparisons) {
    int L = 0;
    int R = size - 1;

    while (L < R) {
        int mid = L + (R - L) / 2;
        (*comparisons)++;
        if (array[mid] < search) {
            L = mid + 1;
        } else {
            R = mid;
        }
    }
    (*comparisons)++;    
    if (L < size && array[L] == search) {
        return L;
    } else {
        return -1;
    }
}

void filling_in_the_array(int array[], int size) {
    for (int i = 0; i < size; i++) {
        array[i] = i + 1;
    }
}

int main(void) {
    srand(time(NULL));


    int search;
     printf("Введите ключ поиска ");
   
    scanf("%d",&search);
    

    printf("Трудоемкость двоичного поиска элемента\n");
    printf("+--------+-----------------+-----------------+\n");
    printf("|   N    |  Cф I версия    |  Cф II версия   |\n");
    printf("+--------+-----------------+-----------------+\n");

    for (int size = 100; size <= 1000; size += 100) {
        int array[size];
        filling_in_the_array(array, size);

        int comparisons1 = 0;
        int comparisons2 = 0;

        binary_search(array, size, search, &comparisons1);
        binary_search_up(array, size, search, &comparisons2);

        printf("| %5d  | %8d        | %8d        |\n", size, comparisons1, comparisons2);
    }

    printf("+--------+-----------------+-----------------+\n");

    return 0;
}
