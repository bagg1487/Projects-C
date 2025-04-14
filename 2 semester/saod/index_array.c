#include <stdio.h>
#include <string.h>

#define SIZE 4

struct Contact {
    char name[50];
    char lastname[50];
    int age;
    int number;
};

void printContacts(struct Contact contacts[], int index[], int size) {
    for (int i = 0; i < size; i++) {
        struct Contact c = contacts[index[i]];
        printf("%s %s, Возраст: %d, Номер: %d\n", c.lastname, c.name, c.age, c.number);
    }
    printf("\n");
}

void printIndexArray(int index[], int size) {
    printf("Индексный массив: [ ");
    for (int i = 0; i < size; i++) {
        printf("%d ", index[i]);
    }
    printf("]\n");
}

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

void sortIndexByLastNameAndNumber(struct Contact contacts[], int index[], int size) {
    for (int i = 1; i < size; i++) {
        int temp = index[i];
        int j = i - 1;
        
        while (j >= 0 && (strcmp(contacts[index[j]].lastname, contacts[temp].lastname) > 0 || 
                         (strcmp(contacts[index[j]].lastname, contacts[temp].lastname) == 0 &&
                          contacts[index[j]].number > contacts[temp].number))) {
            index[j + 1] = index[j];
            j--;
        }
        index[j + 1] = temp;
    }
}

void sortIndexByLastNameAndAge(struct Contact contacts[], int index[], int size) {
    for (int i = 1; i < size; i++) {
        int temp = index[i];
        int j = i - 1;
        
        while (j >= 0 && (strcmp(contacts[index[j]].lastname, contacts[temp].lastname) > 0 || 
                         (strcmp(contacts[index[j]].lastname, contacts[temp].lastname) == 0 &&
                          contacts[index[j]].age > contacts[temp].age))) {
            index[j + 1] = index[j];
            j--;
        }
        index[j + 1] = temp;
    }
}

void sortIndexByNameAndNumber(struct Contact contacts[], int index[], int size) {
    for (int i = 1; i < size; i++) {
        int temp = index[i];
        int j = i - 1;
        
        while (j >= 0 && (strcmp(contacts[index[j]].name, contacts[temp].name) > 0 || 
                         (strcmp(contacts[index[j]].name, contacts[temp].name) == 0 &&
                          contacts[index[j]].number > contacts[temp].number))) {
            index[j + 1] = index[j];
            j--;
        }
        index[j + 1] = temp;
    }
}

int main() {
    struct Contact contacts[SIZE] = {
        {"Влад", "Погорелов", 18, 893213},
        {"Илья", "Погорелов", 33, 123513},
        {"Артем", "Добромилов", 18, 763421},
        {"Рома", "Петров", 19, 345635},
    };

    int index[SIZE], index1[SIZE], index2[SIZE], index3[SIZE];
    
    // Инициализация индексных массивов
    for (int i = 0; i < SIZE; i++) {
        index[i] = i;
        index1[i] = i;
        index2[i] = i;
        index3[i] = i;
    }

    printf("Исходный справочник:\n");
    printContacts(contacts, index, SIZE);
    printf("Исходные индексы:\n");
    printIndexArray(index, SIZE);

    // Сортировка по фамилии и имени
    sortIndexByName(contacts, index, SIZE);
    printf("\nСортировка по фамилии и имени:\n");
    printIndexArray(index, SIZE);
    printContacts(contacts, index, SIZE);

    // Сортировка по фамилии и номеру
    sortIndexByLastNameAndNumber(contacts, index1, SIZE);
    printf("Сортировка по фамилии и номеру:\n");
    printIndexArray(index1, SIZE);
    printContacts(contacts, index1, SIZE);


    // Сортировка по имени и номеру
    sortIndexByNameAndNumber(contacts, index3, SIZE);
    printf("Сортировка по имени и номеру:\n");
    printIndexArray(index3, SIZE);
    printContacts(contacts, index3, SIZE);
    
    return 0;
}
