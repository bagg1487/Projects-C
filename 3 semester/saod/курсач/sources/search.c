#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/search.h"

int compare_strings(const char* s1, const char* s2, int n) {
    for (int i = 0; i < n; i++) {
        if (s1[i] != s2[i]) return s1[i] - s2[i];
        if (s1[i] == '\0') break;
    }
    return 0;
}

QueueNode* binary_search_by_street(Record** records, int count, const char* key_street) {
    if (!records || count == 0 || !key_street) return NULL;
    
    char search_prefix[4];
    strncpy(search_prefix, key_street, 3);
    search_prefix[3] = '\0';
    
    int left = 0;
    int right = count - 1;
    int first_index = -1;
    
    while (left <= right) {
        int mid = (left + right) / 2;
        int cmp = compare_strings(records[mid]->street, search_prefix, 3);
        
        if (cmp == 0) {
            first_index = mid;
            right = mid - 1;
        } else if (cmp < 0) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    
    if (first_index == -1) return NULL;
    
    QueueNode* head = NULL;
    QueueNode* tail = NULL;
    
    for (int i = first_index; i < count; i++) {
        if (compare_strings(records[i]->street, search_prefix, 3) != 0) break;
        
        QueueNode* new_node = malloc(sizeof(QueueNode));
        new_node->data = records[i];
        new_node->next = NULL;
        
        if (!head) head = new_node;
        else tail->next = new_node;
        tail = new_node;
    }
    
    return head;
}

void print_queue_table(QueueNode* head, const char* title) {
    if (!head) {
        printf("Записи не найдены\n");
        return;
    }
    
    printf("\n%s\n", title);
    printf("┌─────┬────────────────────────────────┬──────────────────┬──────┬─────────┬────────────┐\n");
    printf("│ №   │ ФИО                            │ Улица            │ Дом  │Квартира │ Дата       │\n");
    printf("├─────┼────────────────────────────────┼──────────────────┼──────┼─────────┼────────────┤\n");
    
    int counter = 1;
    QueueNode* current = head;
    while (current) {
        printf("│ %-3d │ %-30s │ %-16s │ %-4d │ %-7d │ %-10s │\n",
               counter++, current->data->fio, current->data->street,
               current->data->house, current->data->apartment, current->data->date);
        current = current->next;
    }
    
    printf("└─────┴────────────────────────────────┴──────────────────┴──────┴─────────┴────────────┘\n");
}
void free_queue(QueueNode* head) {
    while (head) {
        QueueNode* next = head->next;
        free(head);
        head = next;
    }
}