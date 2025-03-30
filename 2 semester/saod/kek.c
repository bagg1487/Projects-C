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
                          contacts[index[j]].number > contacts[temp].number))) {
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
    printf("Ваш выбор: ");
}

int main() {
    struct Contact contacts[SIZE] = {
        {"Иван", "Петров", 25, 91357},
        {"Кирилл", "Кушнарев", 31, 83469},
        {"Иван", "Алексеев", 95, 71235},
        {"Дмитрий", "Петров", 13, 26494},
    };

    int index[SIZE];
    for (int i = 0; i < SIZE; i++) {
        index[i] = i;
    }

    int choice;

    printf("\nТекущий справочник:\n");
    printContacts(contacts, index, SIZE);
    
    displayMenu();
    scanf("%d", &choice);
    
    // Восстанавливаем исходный порядок перед каждой сортировкой
    for (int i = 0; i < SIZE; i++) {
        index[i] = i;
    }
    
    switch(choice) {
        case 1:
            sortIndexByName(contacts, index, SIZE);
            printf("\nСправочник отсортирован по фамилии и имени:\n");
            break;
        case 2:
            sortIndexByLastNameAndAge(contacts, index, SIZE);
            printf("\nСправочник отсортирован по фамилии и возрасту:\n");
            break;
        case 3:
            sortIndexByLastNameAndNumber(contacts, index, SIZE);
            printf("\nСправочник отсортирован по фамилии и номеру:\n");
            break;
        case 4:
            sortIndexByNameAndNumber(contacts, index, SIZE);
            printf("\nСправочник отсортирован по имени и номеру:\n");
            break;
        
        default:
            printf("Неверный выбор. Попробуйте снова.\n");
    }
    
    if (choice >= 1 && choice <= 6) {
        printContacts(contacts, index, SIZE);
    }

}
