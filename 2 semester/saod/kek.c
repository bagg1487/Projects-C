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

void printArray(int index[]) {
    for (int i = 0; i < SIZE; i++) {
        printf("%d", index[i]);
    }
    printf("\n");
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

void sortIndexByAge(struct Contact contacts[], int index[], int size) {
    for (int i = 1; i < size; i++) {
        int temp = index[i];
        int j = i - 1;
        
        while (j >= 0 && (contacts[index[j]].age > contacts[temp].age || 
                         (contacts[index[j]].age == contacts[temp].age &&
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

    int indexByName[SIZE], indexByAge[SIZE], index[SIZE];
    for (int i = 0; i < SIZE; i++) {
        indexByName[i] = i;
        indexByAge[i] = i;
        index[i] = i;
    }

    printf("Исходный справочник:\n");
    printArray(index);
    printContacts(contacts, index, SIZE);

    sortIndexByName(contacts, indexByName, SIZE);
    sortIndexByAge(contacts, indexByAge, SIZE);

    printf("Справочник отсортирован по фамилии и имени:\n");
    printArray(indexByName);
    printContacts(contacts, indexByName, SIZE);

    printf("Справочник отсортирован по возрасту и номеру:\n");
    printArray(indexByAge);
    printContacts(contacts, indexByAge, SIZE);
    
    return 0;
}
