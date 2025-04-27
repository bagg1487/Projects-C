#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Node {
    int key;
    struct Node* next;
} Node;

// Вставка в хеш-таблицу
int insert(Node** table, int tableSize, int key) {
    int index = key % tableSize;
    Node* head = table[index];

    // Если список непустой, это коллизия
    int collision = (head != NULL);

    // Создаем новый элемент и вставляем в начало списка
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) exit(EXIT_FAILURE);
    newNode->key = key;
    newNode->next = head;
    table[index] = newNode;

    return collision;
}

// Очистка хеш-таблицы
void freeTable(Node** table, int tableSize) {
    for (int i = 0; i < tableSize; i++) {
        Node* current = table[i];
        while (current) {
            Node* tmp = current;
            current = current->next;
            free(tmp);
        }
    }
    free(table);
}

int main() {
    srand(time(NULL));

    // Простой набор простых чисел от 11 до 101
    int primes[] = {11, 13, 17, 19, 23, 29, 31, 37, 41, 47};
    int primesCount = sizeof(primes) / sizeof(primes[0]);

    int numKeys = 500; // Количество исходных символов (элементов)
    int* keys = (int*)malloc(numKeys * sizeof(int));

    if (!keys) {
        printf("Ошибка выделения памяти.\n");
        return 1;
    }

    // Генерируем случайные ключи
    for (int i = 0; i < numKeys; i++) {
        keys[i] = rand();
    }

    printf("-----------------------------------------------------------------|\n");
    printf("| Размер хеш-таблицы | Количество символов | Количество коллизий |\n");
    printf("-----------------------------------------------------------------|\n");

    for (int p = 0; p < primesCount; p++) {
        int tableSize = primes[p];

        // Инициализация пустой хеш-таблицы
        Node** table = (Node**)calloc(tableSize, sizeof(Node*));
        if (!table) {
            printf("Ошибка выделения памяти для таблицы.\n");
            free(keys);
            return 1;
        }

        int collisionCount = 0;

        // Вставляем все ключи в хеш-таблицу
        for (int i = 0; i < numKeys; i++) {
            collisionCount += insert(table, tableSize, keys[i]);
        }

        printf("| %18d | %19d | %19d |\n", tableSize, numKeys, collisionCount);

        freeTable(table, tableSize);
    }

   printf("-----------------------------------------------------------------|\n");

    free(keys);
    return 0;
}
