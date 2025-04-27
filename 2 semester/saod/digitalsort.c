#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BASE 256

typedef struct Node {
    int value;
    struct Node* next;
} Node;

// Функция для создания нового узла
Node* createNode(int value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) exit(EXIT_FAILURE);
    newNode->value = value;
    newNode->next = NULL;
    return newNode;
}

// Получить разряд числа (от младшего к старшему)
int getDigit(int value, int bytePos) {
    return (value >> (8 * bytePos)) & 0xFF;
}

// Функция цифровой сортировки (по псевдокоду)
Node* digitalSort(Node* head, int maxBytes) {
    Node* bucketsHead[BASE], *bucketsTail[BASE];
    Node *p, *q;
    int i, j, d;

    for (j = 0; j < maxBytes; j++) {
        for (i = 0; i < BASE; i++) {
            bucketsHead[i] = NULL;
            bucketsTail[i] = NULL;
        }

        p = head;
        while (p != NULL) {
            d = getDigit(p->value, j);

            if (bucketsTail[d] == NULL) {
                bucketsHead[d] = bucketsTail[d] = p;
            } else {
                bucketsTail[d]->next = p;
                bucketsTail[d] = p;
            }
            p = p->next;
        }

        p = NULL;
        for (i = 0; i < BASE; i++) {
            if (bucketsHead[i] != NULL) {
                if (p == NULL) {
                    head = bucketsHead[i];
                    p = bucketsTail[i];
                } else {
                    p->next = bucketsHead[i];
                    p = bucketsTail[i];
                }
            }
        }
        if (p != NULL) p->next = NULL;
    }
    return head;
}

// Вспомогательные функции
Node* generateRandomList(int size) {
    Node* head = NULL;
    Node* tail = NULL;
    for (int i = 0; i < size; i++) {
        int value = rand() % 1000000;
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

Node* generateSortedList(int size, int descending) {
    Node* head = NULL;
    Node* tail = NULL;
    for (int i = 0; i < size; i++) {
        int value = descending ? (size - i) : i;
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

void freeList(Node* head) {
    Node* tmp;
    while (head) {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}

// Функция для нахождения максимального количества байт в числе
int maxBytes(Node* head) {
    int maxVal = 0;
    while (head) {
        if (head->value > maxVal)
            maxVal = head->value;
        head = head->next;
    }
    int bytes = 0;
    while (maxVal > 0) {
        bytes++;
        maxVal >>= 8;
    }
    return bytes;
}

// Подсчёт количества операций (M = L(m + n))
long calculateM(int L, int m, int n) {
    return (long)L * (m + n);
}

int main() {
    srand(time(NULL));

    int sizes[] = {100, 200, 300, 400, 500};
    int numSizes = sizeof(sizes) / sizeof(sizes[0]);

    printf("N\tM (теоретич.)\tУбыв.\tСлуч.\tВозр.\n");

    for (int s = 0; s < numSizes; s++) {
        int n = sizes[s];
        Node *listDesc = generateSortedList(n, 1);
        Node *listRand = generateRandomList(n);
        Node *listAsc = generateSortedList(n, 0);

        int L_rand = maxBytes(listRand);
        long M_theoretical = calculateM(L_rand, BASE, n);

        int L_desc = maxBytes(listDesc);
        int L_asc = maxBytes(listAsc);

        long M_fact_desc = calculateM(L_desc, BASE, n);
        long M_fact_rand = calculateM(L_rand, BASE, n);
        long M_fact_asc = calculateM(L_asc, BASE, n);

        printf("%d      %ld        %ld  %ld    %ld\n", n, M_theoretical, M_fact_desc, M_fact_rand, M_fact_asc);

        freeList(listDesc);
        freeList(listRand);
        freeList(listAsc);
    }

    return 0;
}
