#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_STR 2  // Для хранения символов (1 символ + '\0')
#define PRIME_COUNT 10
#define EMPTY ""
#define DELETED "DEL"

typedef struct {
    char key[MAX_STR];
    int deleted;
} HashSlot;

typedef struct {
    HashSlot* slots;
    int size;
    int colls;
} HashTable;

typedef struct Node {
    int key;
    struct Node* next;
} Node;

int insert(Node** table, int tableSize, int key) {
    int index = key % tableSize;
    Node* head = table[index];
    int collision = (head != NULL);
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) exit(EXIT_FAILURE);
    newNode->key = key;
    newNode->next = head;
    table[index] = newNode;
    return collision;
}

void freeTableChain(Node** table, int tableSize) {
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

const int primes[PRIME_COUNT] = {11, 13, 17, 19, 23, 29, 31, 37, 41, 47};

HashTable* createTable(int size) {
    HashTable* t = (HashTable*)malloc(sizeof(HashTable));
    if(!t) return NULL;
    
    t->size = size;
    t->colls = 0;
    t->slots = (HashSlot*)calloc(size, sizeof(HashSlot));
    if(!t->slots) {
        free(t);
        return NULL;
    }
    
    for(int i = 0; i < size; i++) {
        strcpy(t->slots[i].key, EMPTY);
        t->slots[i].deleted = 0;
    }
    return t;
}

int hashFunc(const char* key, int size) {
    if(!key || size <= 0) return 0;
    int h = 0;
    while(*key) h = h * 31 + *key++;
    return abs(h % size);
}

void insertLinear(HashTable* t, const char* key) {
    if(!t || !key) return;
    
    int idx = hashFunc(key, t->size);
    int i = 0;
    
    while(i < t->size && strcmp(t->slots[idx].key, EMPTY) != 0 && strcmp(t->slots[idx].key, DELETED) != 0) {
        if(strcmp(t->slots[idx].key, key) == 0) return;  // Дубликат
        t->colls++;
        idx = (idx + 1) % t->size;
        i++;
    }
    
    if(i < t->size) {
        strncpy(t->slots[idx].key, key, MAX_STR-1);
        t->slots[idx].key[MAX_STR-1] = '\0';
        t->slots[idx].deleted = 0;
    }
}

void insertQuad(HashTable* t, const char* key) {
    if(!t || !key) return;
    int idx = hashFunc(key, t->size);
    int i = 0;
    int orig = idx;
    
    while(i < t->size && strcmp(t->slots[idx].key, EMPTY) != 0 && strcmp(t->slots[idx].key, DELETED) != 0) {
        if(strcmp(t->slots[idx].key, key) == 0) return;  // Дубликат
        t->colls++;
        i++;
        idx = (orig + i*i) % t->size;
    }
    
    if(i < t->size) {
        strncpy(t->slots[idx].key, key, MAX_STR-1);
        t->slots[idx].key[MAX_STR-1] = '\0';
        t->slots[idx].deleted = 0;
    }
}

void freeTable(HashTable* t) {
    if(!t) return;
    if(t->slots) free(t->slots);
    free(t);
}

void generateRandomText(char* buffer, size_t size) {
    const char charset[] = "Every year we go to Florida. We like to go to the beach."
                          "My favorite beach is called Emerson Beach. It is very long, "
                          "with soft sand and palm trees. It is very beautiful. I like "
                          "to make sandcastles and watch the sailboats go by. Sometimes "
                          "there are dolphins and whales in the water!";
    for(size_t i = 0; i < size - 1; i++) {
        buffer[i] = charset[rand() % (sizeof(charset) - 1)];
    }
    buffer[size - 1] = '\0';
}

int main() {
    srand(time(NULL));
    const size_t textSize = 1024;
    char* text = (char*)malloc(textSize + 1);
    generateRandomText(text, textSize + 1);
    
    printf("\nАнализ коллизий в тексте (1 КБ) с разными методами разрешения:\n");
    printf(" __________________________________________________\n");
    printf("|      |               |        Кол-во коллизий    |\n");
    printf("|Размер|Кол-во символов|___________________________|\n");
    printf("|      |               |  Линейные  | Квадратичные |\n");
    printf("|______|_______________|____________|______________|\n");
    
    for(int p = 0; p < PRIME_COUNT; p++) {
        int tableSize = primes[p];
        
        // Линейное пробирование
        HashTable* lin = createTable(tableSize);
        // Квадратичное пробирование
        HashTable* quad = createTable(tableSize);
        // Метод цепочек (из второго примера)
        Node** chainTable = (Node**)calloc(tableSize, sizeof(Node*));
        int chainCollisions = 0;
        
        for(size_t i = 0; i < textSize && text[i] != '\0'; i++) {
            char key[2] = {text[i], '\0'};
            insertLinear(lin, key);
            insertQuad(quad, key);
            chainCollisions += insert(chainTable, tableSize, (int)text[i]);
        }
        
        printf("|%6d|%15zu|%12d|%14d|\n", 
               tableSize, strlen(text), lin->colls, quad->colls);
        printf("|______|_______________|____________|______________|\n");
        
        freeTable(lin);
        freeTable(quad);
        freeTableChain(chainTable, tableSize);
    }
    
    free(text);
    return 0;
}
