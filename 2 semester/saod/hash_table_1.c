#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct Node {
    int key;        // ASCII-код символа
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

// Генерация случайного текста (1 КБ)
void generateRandomText(char* buffer, size_t size) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 ,.!?";
    for (size_t i = 0; i < size - 1; i++) {
        buffer[i] = charset[rand() % (sizeof(charset) - 1)];
    }
    buffer[size - 1] = '\0';
}

int main() {
    srand(time(NULL));

    // Простой набор простых чисел от 11 до 101
    int primes[] = {11, 13, 17, 19, 23, 29, 31, 37, 41, 47};
    int primesCount = sizeof(primes) / sizeof(primes[0]);

    const size_t textSize = 1024; // 1 КБ текста
    char* text = (char*)malloc(textSize + 1);

    if (!text) {
        printf("Ошибка выделения памяти.\n");
        return 1;
    }

   
        generateRandomText(text, textSize + 1);


    printf("\n Хеширование текста (1 КБ) методом прямого связывания \n");
    printf("-----------------------------------------------------------------|\n");
    printf("| Размер хеш-таблицы | Количество символов | Количество коллизий |\n");
    printf("-----------------------------------------------------------------|\n");

    for (int p = 0; p < primesCount; p++) {
        int tableSize = primes[p];

        // Инициализация пустой хеш-таблицы
        Node** table = (Node**)calloc(tableSize, sizeof(Node*));
        if (!table) {
            printf("Ошибка выделения памяти для таблицы.\n");
            free(text);
            return 1;
        }

        int collisionCount = 0;

        // Вставляем все символы текста в хеш-таблицу
        for (size_t i = 0; i < textSize && text[i] != '\0'; i++) {
            collisionCount += insert(table, tableSize, (int)text[i]);
        }

        printf("| %18d | %19zu | %19d |\n", tableSize, strlen(text), collisionCount);

        freeTable(table, tableSize);
    }

    printf("-----------------------------------------------------------------|\n");

    free(text);
    return 0;
}
