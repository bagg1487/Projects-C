#ifndef SEARCH_H
#define SEARCH_H

#include "database.h"

typedef struct QueueNode {
    Record* data;
    struct QueueNode* next;
} QueueNode;

QueueNode* binary_search_by_street(Record** records, int count, const char* key_street);
void print_queue_table(QueueNode* head, const char* title);
void free_queue(QueueNode* head);

#endif