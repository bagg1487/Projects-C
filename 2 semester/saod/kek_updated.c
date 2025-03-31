#include <stdio.h>
#include <string.h>

#define SIZE 4

struct Contact {
    char name[50];
    char lastname[50];
    int age;
    int number;
};

// Вывод контактов через индексный массив
void printContacts(struct Contact contacts[], int index[], int size) {
    for (int i = 0; i < size; i++) {
        struct Contact c = contacts[index[i]];
        printf("%s %s, Возраст: %d, Номер: %d\n", c.lastname, c.name, c.age, c.number);
    }
    printf("\n");
}


// Вывод индексного массива
void printIndexArray(int index[], int size) {
    printf("Индексный массив: [ ");
    for (int i = 0; i < size; i++) {
        printf("%d ", index[i]);
    }
    printf("]\n");
}

// Сортировка вставками по имени и фамилии
void sortIndexByName(struct Contact contacts[], int index[], int size) {
    for (int i = 1; i < size; i++) {
        int temp = index[i];
        int j = i - 1;
        
        while (j >= 0 && (strcmp(contacts[index[j]].lastname, contacts[temp].lastname) > 0 || 
                         (strcmp(contacts[index[j]].lastname, contacts[temp].lastname) == 0 &&
                          strcmp(contacts[index[j]].name, contacts[temp].name) > 0))) {
            index[j + 1] = index[j];
            j--;
        }
        index[j + 1] = temp;
    }
}

// Сортировка вставками по возрасту
void sortIndexByAge(struct Contact contacts[], int index[], int size) {
    for (int i = 1; i < size; i++) {
        int temp = index[i];
        int j = i - 1;
        
        while (j >= 0 && contacts[index[j]].age > contacts[temp].age) {
            index[j + 1] = index[j];
            j--;
        }
        index[j + 1] = temp;
    }
}

int main() {
    struct Contact contacts[SIZE] = {
        {"Иван", "Петров", 25, 91357},
        {"Кирилл", "Кушнарев", 31, 83469},
        {"Иван", "Алексеев", 95, 71235},
        {"Дмитрий", "Петров", 13, 26494},
    };

    // Два индексных массива
    int indexByName[SIZE];
    int indexByAge[SIZE];

    // Инициализация индексных массивов
    for (int i = 0; i < SIZE; i++) {
        indexByName[i] = i;
        indexByAge[i] = i;
    }

    printf("\nИсходный справочник:\n");
    printContacts(contacts, indexByName, SIZE);

    printf("Индексный массив (до сортировки по имени):\n");
    printIndexArray(indexByName, SIZE);

    printf("Индексный массив (до сортировки по возрасту):\n");
    printIndexArray(indexByAge, SIZE);

    // Сортировка по имени
    sortIndexByName(contacts, indexByName, SIZE);
    printf("\nПосле сортировки по имени и фамилии:\n");
    printContacts(contacts, indexByName, SIZE);
    printIndexArray(indexByName, SIZE);

    // Сортировка по возрасту
    sortIndexByAge(contacts, indexByAge, SIZE);
    printf("\nПосле сортировки по возрасту:\n");
    printContacts(contacts, indexByAge, SIZE);
    printIndexArray(indexByAge, SIZE);

    return 0;
}
