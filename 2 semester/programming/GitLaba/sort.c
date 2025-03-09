#include <stdio.h>
#include <stdlib.h>
void swap(struct Student *a, struct Student *b) {
    struct Student temp = *a;
    *a = *b;
    *b = temp;
}

int partition(struct Student *students, int low, int high, int (*compare)(struct Student, struct Student)) {
    int pivot = students[high].sum; 
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (compare(students[j], students[high]) < 0) { 
            i++;
            swap(&students[i], &students[j]);
        }
    }
    swap(&students[i + 1], &students[high]);
    return i + 1;
}

void quicksort(struct Student *students, int low, int high, int (*compare)(struct Student, struct Student)) {
    if (low < high) {
        int pi = partition(students, low, high, compare);
        quicksort(students, low, pi - 1, compare);
        quicksort(students, pi + 1, high, compare);
    }
}

void QuickSort(int n, struct Student *students, int (*compare)(struct Student, struct Student)) {
    for (int i = 0; i < n; i++) printStudentInfo(students[i]);
    printf("-----------------------------------------------------------------------\n");
    quicksort(students, 0, n - 1, compare);
    for (int i = 0; i < n; i++) printStudentInfo(students[i]);
}
int main(){
    
}