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

// Сортировка вставками по фамилии и имени
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

// Сортировка вставками по фамилии и возрасту
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

// Сортировка вставками по фамилии и номеру
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

// Сортировка вставками по имени и номеру
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

// Сортировка вставками по номеру
void sortIndexByNumber(struct Contact contacts[], int index[], int size) {
    for (int i = 1; i < size; i++) {
        int temp = index[i];
        int j = i - 1;
        
        while (j >= 0 && contacts[index[j]].number > contacts[temp].number) {
            index[j + 1] = index[j];
            j--;
        }
        index[j + 1] = temp;
    }
}

void displayMenu() {
    printf("\nВыберите вариант сортировки:\n");
    printf("1. По фамилии и имени\n");
    printf("2. По фамилии и возрасту\n");
    printf("3. По фамилии и номеру\n");
    printf("4. По имени и номеру\n");
    printf("5. По возрасту\n");
    printf("6. По номеру\n");
    printf("Ваш выбор: ");
}

int main() {
    struct Contact contacts[SIZE] = {
        {"Иван", "Петров", 25, 91357},
        {"Кирилл", "Кушнарев", 31, 83469},
        {"Иван", "Алексеев", 95, 71235},
        {"Дмитрий", "Петров", 13, 26494},
    };

    // Два индексных массива (для разных сортировок)
    int indexByName[SIZE];
    int indexByAge[SIZE];

    // Инициализация индексных массивов
    for (int i = 0; i < SIZE; i++) {
        indexByName[i] = i;
        indexByAge[i] = i;
    }

    printf("\nТекущий справочник:\n");
    printContacts(contacts, indexByName, SIZE);

    // Вывод индексных массивов до сортировки
    printf("Индексный массив (до сортировки по имени):\n");
    printIndexArray(indexByName, SIZE);
    printf("Индексный массив (до сортировки по возрасту):\n");
    printIndexArray(indexByAge, SIZE);

    int choice;
    displayMenu();
    scanf("%d", &choice);

    // Восстанавливаем исходные индексы перед сортировкой
    for (int i = 0; i < SIZE; i++) {
        indexByName[i] = i;
    }

    switch(choice) {
        case 1:
            sortIndexByName(contacts, indexByName, SIZE);
            printf("\nСправочник отсортирован по фамилии и имени:\n");
            break;
        case 2:
            sortIndexByLastNameAndAge(contacts, indexByName, SIZE);
            printf("\nСправочник отсортирован по фамилии и возрасту:\n");
            break;
        case 3:
            sortIndexByLastNameAndNumber(contacts, indexByName, SIZE);
            printf("\nСправочник отсортирован по фамилии и номеру:\n");
            break;
        case 4:
            sortIndexByNameAndNumber(contacts, indexByName, SIZE);
            printf("\nСправочник отсортирован по имени и номеру:\n");
            break;
        case 5:
            sortIndexByAge(contacts, indexByAge, SIZE);
            printf("\nСправочник отсортирован по возрасту:\n");
            break;
        case 6:
            sortIndexByNumber(contacts, indexByName, SIZE);
            printf("\nСправочник отсортирован по номеру:\n");
            break;
        default:
            printf("Неверный выбор. Попробуйте снова.\n");
            return 1;
    }

    // Вывод отсортированного справочника и индексного массива
    if (choice >= 1 && choice <= 4 || choice == 6) {
        printContacts(contacts, indexByName, SIZE);
        printf("Индексный массив после сортировки:\n");
        printIndexArray(indexByName, SIZE);
    } else if (choice == 5) {
        printContacts(contacts, indexByAge, SIZE);
        printf("Индексный массив после сортировки:\n");
        printIndexArray(indexByAge, SIZE);
    }

    return 0;
}
