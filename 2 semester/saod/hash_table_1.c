#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct Node {
    int key;        // ASCII-код символа
    struct Node* next;
} Node;

// Вставка в хеш-таблицу (считаем коллизии только для разных символов в одной ячейке)
int insert(Node** table, int tableSize, int key, int* isNewKey) {
    int index = key % tableSize;
    Node* head = table[index];
    *isNewKey = 1;  // Предполагаем, что ключ новый

    // Проверяем, есть ли уже такой ключ в списке
    Node* current = head;
    while (current != NULL) {
        if (current->key == key) {
            *isNewKey = 0;  // Ключ уже был, это дубликат
            return 0;        // Дубликаты не считаем коллизией
        }
        current = current->next;
    }

    // Если список непустой, но ключ новый — это коллизия
    int collision = (head != NULL);

    // Вставляем новый узел в начало списка
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
    const char charset[] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.  Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";
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

    const size_t textSize = 1000; // 1 КБ текста (1000 символов)
    char* text = (char*)malloc(textSize + 1);
    generateRandomText(text, textSize + 1);

    printf("\n Хеширование текста (1 КБ) методом прямого связывания \n");
    printf("-----------------------------------------------------------------|\n");
    printf("| Размер хеш-таблицы | Уникальных символов | Количество коллизий |\n");
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
        int uniqueSymbols = 0;

        // Вставляем все символы текста в хеш-таблицу
        for (size_t i = 0; i < textSize && text[i] != '\0'; i++) {
            int isNewKey;
            collisionCount += insert(table, tableSize, (int)text[i], &isNewKey);
            if (isNewKey) uniqueSymbols++;
        }

        printf("| %18d | %19d | %19d |\n", tableSize, uniqueSymbols, collisionCount);

        freeTable(table, tableSize);
    }

    printf("-----------------------------------------------------------------|\n");

    free(text);
    return 0;
}
