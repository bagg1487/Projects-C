#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

typedef struct {
    Node* top;
} Stack;

typedef struct {
    Node* front;
    Node* rear;
} Queue;

void initStack(Stack* s) {
    s->top = NULL;
}

void initQueue(Queue* q) {
    q->front = q->rear = NULL;
}

void push(Stack* s, int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = s->top;
    s->top = newNode;
}

void enqueue(Queue* q, int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = NULL;
    if (q->rear == NULL) {
        q->front = q->rear = newNode;
    } else {
        q->rear->next = newNode;
        q->rear = newNode;
    }
}

void fillStackAscending(Stack* s, int n) {
    for (int i = 1; i <= n; i++) {
        push(s, i);
    }
}

void fillStackDescending(Stack* s, int n) {
    for (int i = n; i >= 1; i--) {
        push(s, i);
    }
}

void fillStackRandom(Stack* s, int n, int minVal, int maxVal) {
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        int num = minVal + rand() % (maxVal - minVal + 1);
        push(s, num);
    }
}

void fillQueueAscending(Queue* q, int n) {
    for (int i = 1; i <= n; i++) {
        enqueue(q, i);
    }
}

void fillQueueDescending(Queue* q, int n) {
    for (int i = n; i >= 1; i--) {
        enqueue(q, i);
    }
}

void fillQueueRandom(Queue* q, int n, int minVal, int maxVal) {
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        int num = minVal + rand() % (maxVal - minVal + 1);
        enqueue(q, num);
    }
}

void printList(Node* head) {
    Node* current = head;
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

void printListForwardRecursive(Node* head) {
    if (head == NULL) {
        printf("\n");
        return;
    }
    printf("%d ", head->data);
    printListForwardRecursive(head->next);
}

void printListBackwardRecursive(Node* head) {
    if (head == NULL) {
        return;
    }
    printListBackwardRecursive(head->next);
    printf("%d ", head->data);
    if (head == NULL) {
        printf("\n");
    }
}

int calculateChecksum(Node* head) {
    int sum = 0;
    Node* current = head;
    while (current != NULL) {
        sum += current->data;
        current = current->next;
    }
    return sum;
}

int countSeries(Node* head) {
    if (head == NULL) return 0;
    int series = 1;
    Node* current = head;
    while (current->next != NULL) {
        if (current->data > current->next->data) {
            series++;
        }
        current = current->next;
    }
    return series;
}

void clearQueue(Queue* q) {
    Node* current = q->front;
    while (current != NULL) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }
    q->front = q->rear = NULL;
}

void clearStack(Stack* s) {
    Node* current = s->top;
    while (current != NULL) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }
    s->top = NULL;
}int main() {
    Stack s;
    Queue q;
    initStack(&s);
    initQueue(&q);

    printf("Операции со стеком\n");
    
    printf("\nСтек по убыванию:\n");
    fillStackAscending(&s, 5);
    printf("Элементов: ");
    printList(s.top);
    printf("Контрольная сумма: %d\n", calculateChecksum(s.top));
    printf("Количество серий: %d\n", countSeries(s.top));
    printf("Прямой порядок: ");
    printListForwardRecursive(s.top);
    printf("Обратный порядок: ");
    printListBackwardRecursive(s.top);
    clearStack(&s);
    printf("\n");

    printf("\nСтек по возрастанию:\n");
    fillStackDescending(&s, 5);
    printf("Элементов: ");
    printList(s.top);
    printf("Контрольная сумма: %d\n", calculateChecksum(s.top));
    printf("Количество серий: %d\n", countSeries(s.top));
    printf("Прямой порядок: ");
    printListForwardRecursive(s.top);
    printf("Обратный порядок: ");
    printListBackwardRecursive(s.top);
    clearStack(&s);
    printf("\n");

    printf("\nРандомный стек:\n");
    fillStackRandom(&s, 5, 1, 10);
    printf("Элементов: ");
    printList(s.top);
    printf("Контрольная сумма: %d\n", calculateChecksum(s.top));
    printf("Количество серий: %d\n", countSeries(s.top));
    printf("Прямой порядок: ");
    printListForwardRecursive(s.top);
    printf("Обратный порядок: ");
    printListBackwardRecursive(s.top);
    clearStack(&s);
    printf("\n");

    printf("\nОперации с очередью\n");
    
    printf("\nОчередь по убыванию:\n");
    fillQueueAscending(&q, 5);
    printf("Элементов: ");
    printList(q.front);
    printf("Контрольная сумма: %d\n", calculateChecksum(q.front));
    printf("Количество серий: %d\n", countSeries(q.front));
    printf("Прямой порядок: ");
    printListForwardRecursive(q.front);
    printf("Обратный порядок: ");
    printListBackwardRecursive(q.front);
    clearQueue(&q);
    printf("\n");

    printf("\nОчередь по возрастанию:\n");
    fillQueueDescending(&q, 5);
    printf("Элементов: ");
    printList(q.front);
    printf("Контрольная сумма: %d\n", calculateChecksum(q.front));
    printf("Количество серий: %d\n", countSeries(q.front));
    printf("Прямой порядок: ");
    printListForwardRecursive(q.front);
    printf("Обратный порядок: ");
    printListBackwardRecursive(q.front);
    clearQueue(&q);
    printf("\n");

    printf("\nРандомная очередь:\n");
    fillQueueRandom(&q, 5, 1, 10);
    printf("Элементов: ");
    printList(q.front);
    printf("Контрольная сумма: %d\n", calculateChecksum(q.front));
    printf("Количество серий: %d\n", countSeries(q.front));
    printf("Прямой порядок: ");
    printListForwardRecursive(q.front);
    printf("Обратный порядок: ");
    printListBackwardRecursive(q.front);
    clearQueue(&q);
    printf("\n");

    return 0;
}
