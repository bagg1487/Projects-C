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

void printIndexArrays(int indexByName[], int indexByAge[], int size) {
    printf("Индексный массив (по имени): [ ");
    for (int i = 0; i < size; i++) printf("%d ", indexByName[i]);
    printf("]\n");
    
    printf("Индексный массив (по возрасту): [ ");
    for (int i = 0; i < size; i++) printf("%d ", indexByAge[i]);
    printf("]\n\n");
}

void sortIndexByName(struct Contact contacts[], int index[], int size) {
    for (int i = 1; i < size; i++) {
        int temp = index[i];
        int j = i - 1;
        
        while (j >= 0 && (strcmp(contacts[index[j]].lastname, contacts[temp].lastname) > 0 || 
             (strcmp(contacts[index[j]].lastname, contacts[temp].lastname) == 0 &&
              strcmp(contacts[index[j]].name, contacts[temp].name) > 0)) {
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

void sortIndexByNameAndNumber(struct Contact contacts[], int index[], int size) {
    for (int i = 1; i < size; i++) {
        int temp = index[i];
        int j = i - 1;
        
        while (j >= 0 && (strcmp(contacts[index[j]].name, contacts[temp].name) > 0 || 
             (strcmp(contacts[index[j]].name, contacts[temp].name) == 0 &&
              contacts[index[j]].number > contacts[temp].number)) {
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
        
        while (j >= 0 && contacts[index[j]].age > contacts[temp].age) {
            index[j + 1] = index[j];
            j--;
        }
        index[j + 1] = temp;
    }
}

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

    int indexByName[SIZE], indexByAge[SIZE];
    for (int i = 0; i < SIZE; i++) {
        indexByName[i] = i;
        indexByAge[i] = i;
    }

    printf("\nТекущий справочник:\n");
    printContacts(contacts, indexByName, SIZE);
    printf("Индексные массивы ДО сортировки:\n");
    printIndexArrays(indexByName, indexByAge, SIZE);

    int choice;
    displayMenu();
    scanf("%d", &choice);

    // Создаем копии для сохранения исходного порядка
    int originalIndexByName[SIZE], originalIndexByAge[SIZE];
    memcpy(originalIndexByName, indexByName, sizeof(indexByName));
    memcpy(originalIndexByAge, indexByAge, sizeof(indexByAge));

    switch(choice) {
        case 1:
            sortIndexByName(contacts, indexByName, SIZE);
            printf("\nСортировка по фамилии и имени:\n");
            break;
        case 2:
            sortIndexByLastNameAndAge(contacts, indexByName, SIZE);
            printf("\nСортировка по фамилии и возрасту:\n");
            break;
        case 3:
            sortIndexByLastNameAndNumber(contacts, indexByName, SIZE);
            printf("\nСортировка по фамилии и номеру:\n");
            break;
        case 4:
            sortIndexByNameAndNumber(contacts, indexByName, SIZE);
            printf("\nСортировка по имени и номеру:\n");
            break;
        case 5:
            sortIndexByAge(contacts, indexByAge, SIZE);
            printf("\nСортировка по возрасту:\n");
            break;
        case 6:
            sortIndexByNumber(contacts, indexByName, SIZE);
            printf("\nСортировка по номеру:\n");
            break;
        default:
            printf("Неверный выбор!\n");
            return 1;
    }

    // Вывод результатов
    if (choice >= 1 && choice <= 4 || choice == 6) {
        printContacts(contacts, indexByName, SIZE);
    } else if (choice == 5) {
        printContacts(contacts, indexByAge, SIZE);
    }

    printf("\nИндексные массивы ПОСЛЕ сортировки:\n");
    printIndexArrays(indexByName, indexByAge, SIZE);

    // Восстановление исходных массивов
    memcpy(indexByName, originalIndexByName, sizeof(indexByName));
    memcpy(indexByAge, originalIndexByAge, sizeof(indexByAge));

    return 0;
}
