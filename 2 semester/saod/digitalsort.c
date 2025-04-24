#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

// Структура для узла списка
typedef struct Node {
    int data;
    struct Node* next;
} Node;

// Структура для очереди
typedef struct Queue {
    Node* front;
    Node* rear;
} Queue;

// Функции для работы с очередью
Queue* createQueue() {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->front = q->rear = NULL;
    return q;
}

void enqueue(Queue* q, int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = NULL;
    
    if (q->rear == NULL) {
        q->front = q->rear = newNode;
        return;
    }
    
    q->rear->next = newNode;
    q->rear = newNode;
}

int dequeue(Queue* q) {
    if (q->front == NULL) return INT_MIN;
    
    Node* temp = q->front;
    int data = temp->data;
    
    q->front = q->front->next;
    
    if (q->front == NULL)
        q->rear = NULL;
    
    free(temp);
    return data;
}

int isEmpty(Queue* q) {
    return q->front == NULL;
}

// Функция для создания списка из массива
Node* createList(int arr[], int n) {
    Node* head = NULL;
    Node* tail = NULL;
    
    for (int i = 0; i < n; i++) {
        Node* newNode = (Node*)malloc(sizeof(Node));
        newNode->data = arr[i];
        newNode->next = NULL;
        
        if (head == NULL) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }
    
    return head;
}

// Функция для печати списка
void printList(Node* head) {
    while (head != NULL) {
        printf("%d ", head->data);
        head = head->next;
    }
    printf("\n");
}

// Цифровая сортировка для 16-битных чисел
void digitalSort16(Node** headRef, int* moves) {
    const int BITS = 16;
    const int RADIX = 256; // 8 бит за проход (2^8 = 256)
    
    Node* head = *headRef;
    Queue* buckets[RADIX];
    
    for (int i = 0; i < RADIX; i++) {
        buckets[i] = createQueue();
    }
    
    for (int shift = 0; shift < BITS; shift += 8) {
        // Распределение по корзинам
        Node* current = head;
        while (current != NULL) {
            int bucketIndex = (current->data >> shift) & 0xFF;
            enqueue(buckets[bucketIndex], current->data);
            (*moves)++;
            current = current->next;
        }
        
        // Сборка списка обратно
        Node* newHead = NULL;
        Node* newTail = NULL;
        
        for (int i = 0; i < RADIX; i++) {
            while (!isEmpty(buckets[i])) {
                int data = dequeue(buckets[i]);
                (*moves)++;
                
                Node* newNode = (Node*)malloc(sizeof(Node));
                newNode->data = data;
                newNode->next = NULL;
                
                if (newHead == NULL) {
                    newHead = newTail = newNode;
                } else {
                    newTail->next = newNode;
                    newTail = newNode;
                }
            }
        }
        
        // Освобождение старого списка
        current = head;
        while (current != NULL) {
            Node* temp = current;
            current = current->next;
            free(temp);
        }
        
        head = newHead;
    }
    
    *headRef = head;
}

// Цифровая сортировка для 32-битных чисел
void digitalSort32(Node** headRef, int* moves) {
    const int BITS = 32;
    const int RADIX = 256; // 8 бит за проход
    
    Node* head = *headRef;
    Queue* buckets[RADIX];
    
    for (int i = 0; i < RADIX; i++) {
        buckets[i] = createQueue();
    }
    
    for (int shift = 0; shift < BITS; shift += 8) {
        // Распределение по корзинам
        Node* current = head;
        while (current != NULL) {
            int bucketIndex = (current->data >> shift) & 0xFF;
            enqueue(buckets[bucketIndex], current->data);
            (*moves)++;
            current = current->next;
        }
        
        // Сборка списка обратно
        Node* newHead = NULL;
        Node* newTail = NULL;
        
        for (int i = 0; i < RADIX; i++) {
            while (!isEmpty(buckets[i])) {
                int data = dequeue(buckets[i]);
                (*moves)++;
                
                Node* newNode = (Node*)malloc(sizeof(Node));
                newNode->data = data;
                newNode->next = NULL;
                
                if (newHead == NULL) {
                    newHead = newTail = newNode;
                } else {
                    newTail->next = newNode;
                    newTail = newNode;
                }
            }
        }
        
        // Освобождение старого списка
        current = head;
        while (current != NULL) {
            Node* temp = current;
            current = current->next;
            free(temp);
        }
        
        head = newHead;
    }
    
    *headRef = head;
}

// Функция для проверки правильности сортировки
int checkSorted(Node* head) {
    if (head == NULL || head->next == NULL) return 1;
    
    Node* current = head;
    while (current->next != NULL) {
        if (current->data > current->next->data) {
            return 0;
        }
        current = current->next;
    }
    return 1;
}

// Функция для подсчета контрольной суммы
long long checkSum(Node* head) {
    long long sum = 0;
    while (head != NULL) {
        sum += head->data;
        head = head->next;
    }
    return sum;
}

// Функция для подсчета числа серий
int countSeries(Node* head) {
    if (head == NULL) return 0;
    
    int count = 1;
    Node* current = head;
    
    while (current->next != NULL) {
        if (current->data > current->next->data) {
            count++;
        }
        current = current->next;
    }
    
    return count;
}

// Функция для генерации тестовых данных
void generateTestData(int arr[], int n, int type) {
    switch (type) {
        case 0: // Случайные числа
            for (int i = 0; i < n; i++) {
                arr[i] = rand() % 10000;
            }
            break;
        case 1: // Возрастающая последовательность
            for (int i = 0; i < n; i++) {
                arr[i] = i;
            }
            break;
        case 2: // Убывающая последовательность
            for (int i = 0; i < n; i++) {
                arr[i] = n - i - 1;
            }
            break;
    }
}

int main() {
    srand(time(NULL));
    
    const int sizes[] = {100, 200, 300, 400, 500};
    const char* types[] = {"Случайные", "Возрастающие", "Убывающие"};
    
    printf("Трудоемкость цифровой сортировки (16 бит)\n");
    printf("| N  | Теоретич. M | Мф (Убыв.) | Мф (Случ.) | Мф (Возр.) |\n");
    
    for (int i = 0; i < 5; i++) {
        int n = sizes[i];
        int theoretical = 16 * n * 2; // 2 прохода (распределение и сборка)
        
        printf("| %d | %d", n, theoretical);
        
        for (int type = 0; type < 3; type++) {
            int* arr = (int*)malloc(n * sizeof(int));
            generateTestData(arr, n, type);
            
            Node* head = createList(arr, n);
            long long sumBefore = checkSum(head);
            
            int moves = 0;
            digitalSort16(&head, &moves);
            
            long long sumAfter = checkSum(head);
            int isSorted = checkSorted(head);
            int series = countSeries(head);
            
            if (!isSorted || sumBefore != sumAfter) {
                printf(" | Ошибка сортировки");
            } else {
                printf(" | %d", moves);
            }
            
            free(arr);
            
            // Освобождение списка
            Node* current = head;
            while (current != NULL) {
                Node* temp = current;
                current = current->next;
                free(temp);
            }
        }
        
        printf(" |\n");
    }
    
    return 0;
}
