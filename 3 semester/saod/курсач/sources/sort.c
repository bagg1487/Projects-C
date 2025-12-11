#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/sort.h"

int compare_records(const Record* a, const Record* b) {
    int street_cmp = strcmp(a->street, b->street);
    if (street_cmp != 0) return street_cmp;
    return a->house - b->house;
}

void sift_down(Record** records, int start, int end) {
    int root = start;
    while (2 * root + 1 <= end) {
        int child = 2 * root + 1;
        int swap = root;
        
        if (compare_records(records[swap], records[child]) < 0) swap = child;
        if (child + 1 <= end && compare_records(records[swap], records[child + 1]) < 0) swap = child + 1;
        
        if (swap == root) return;
        
        Record* temp = records[root];
        records[root] = records[swap];
        records[swap] = temp;
        root = swap;
    }
}

void heap_sort(Record** records, int n) {
    for (int start = (n - 2) / 2; start >= 0; start--) {
        sift_down(records, start, n - 1);
    }
    for (int end = n - 1; end > 0; end--) {
        Record* temp = records[end];
        records[end] = records[0];
        records[0] = temp;
        sift_down(records, 0, end - 1);
    }
}