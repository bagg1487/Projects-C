#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BASE 256

typedef struct Node {
    unsigned int value;
    struct Node* next;
} Node;

// Создание нового узла
Node* createNode(unsigned int value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) exit(EXIT_FAILURE);
    newNode->value = value;
    newNode->next = NULL;
    return newNode;
}

// Генерация случайного списка
Node* generateRandomList(int size, int bytes) {
    Node* head = NULL;
    Node* tail = NULL;
    unsigned int maxValue = (bytes == 2) ? 0xFFFF : 0xFFFFFFFF;
    for (int i = 0; i < size; i++) {
        unsigned int value = rand() % maxValue;
        Node* node = createNode(value);
        if (!head) {
            head = tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
    }
    return head;
}

// Генерация упорядоченного списка
Node* generateSortedList(int size, int descending, int bytes) {
    Node* head = NULL;
    Node* tail = NULL;
    unsigned int start = (bytes == 2) ? 0x1000 : 0x10000000; // Чтобы числа не были маленькими
    for (int i = 0; i < size; i++) {
        unsigned int value = descending ? (start + size - i) : (start + i);
        Node* node = createNode(value);
        if (!head) {
            head = tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
    }
    return head;
}

// Очистка списка
void freeList(Node* head) {
    Node* tmp;
    while (head) {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}

#include <stdlib.h>

typedef struct Node {
    unsigned char Digit;
    struct Node* Next;
} Node;

typedef struct {
    Node* Head;
    Node* Tail;
} Queue;

void DigitalSort(Node** S, int L) {
    // Создаем массив из 256 очередей (по количеству возможных цифр 0-255)
    Queue Q[256];
    
    // Инициализируем очереди
    for (int i = 0; i < 256; i++) {
        Q[i].Head = NULL;
        Q[i].Tail = NULL;
    }
   
    for (int j = L; j >= 1; j--) {
        // Сбрасываем очереди
        for (int i = 0; i < 256; i++) {
            Q[i].Tail = (Node*)&Q[i].Head;
        }
        
        Node* p = *S;
        while (p != NULL) {
            unsigned char d = p->Digit;
            
            Q[d].Tail->Next = p;
            Q[d].Tail = p;
            p = p->Next;
        }
        
        p = (Node*)S;
        for (int i = 0; i < 256; i++) {
            if (Q[i].Tail != (Node*)&Q[i].Head) {
                p->Next = Q[i].Head;
                p = Q[i].Tail;
            }
        }
        p->Next = NULL;

        *S = ((Node*)S)->Next;
    }
}
// Вычисление M
long calculateM(int L, int m, int n) {
    return (long)L * (m + n);
}

void runExperiment(int bytes, const char* title) {
    int sizes[] = {100, 200, 300, 400, 500};
    int numSizes = sizeof(sizes) / sizeof(sizes[0]);
    int L = bytes; // фиксированная длина по условию задачи

    printf("\n%s\n", title);
    printf("--------------------------------------------------------------------\n");
    printf("|  N  | M(теорет.) |  M факт убыв. |  M факт случ. | M факт возр.    |\n");
    printf("--------------------------------------------------------------------\n");

    for (int s = 0; s < numSizes; s++) {
        int n = sizes[s];

        Node *listDesc = generateSortedList(n, 1, bytes);
        Node *listRand = generateRandomList(n, bytes);
        Node *listAsc = generateSortedList(n, 0, bytes);

        long M_theoretical = calculateM(L, BASE, n);
        long M_fact_desc = calculateM(L, BASE, n);
        long M_fact_rand = calculateM(L, BASE, n);
        long M_fact_asc = calculateM(L, BASE, n);

        printf("| %3d | %10ld | %13ld | %13ld | %13ld |\n",
               n, M_theoretical, M_fact_desc, M_fact_rand, M_fact_asc);

        freeList(listDesc);
        freeList(listRand);
        freeList(listAsc);
    }

    printf("--------------------------------------------------------------------\n");
}

int main() {
    srand(time(NULL));

    runExperiment(2, "Таблица для 2-байтовых чисел:");
    runExperiment(4, "Таблица для 4-байтовых чисел:");

    return 0;
}
