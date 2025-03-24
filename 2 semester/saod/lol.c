#include <stdio.h>
#include <string.h>

    struct phone_direct {
        char name[50];
        char surname[50];
        char phoneNumber[15];
        int age;
    };
    typedef struct phone_direct phone_direct;

    enum key{
        sort_name,
        sort_surname,
        sort_phoneNumber,
        sort_age
    };
    enum direction{
        sort_increasing,
        sort_decreasing
    };

    typedef enum key key;
    typedef enum direction dir;

    // Функция для сравнения двух записей по составному ключу (surname + name)
    int compareRecords(const phone_direct *a, const phone_direct *b, key key, dir dir){
        int result = 0;

        switch (key) {
            case sort_name:
                result = strcmp(a->name, b->name);
                break;
            case sort_surname:
                result = strcmp(a->surname, b->surname);
                break;
            case sort_phoneNumber:
                result = strcmp(a->phoneNumber, b->phoneNumber);
                break;
            case sort_age:
                result = a->age - b->age;
                break;
        }
        if (dir == sort_decreasing){
            result = -result;

        }
        return result;
    }

   void SelectSort(phone_direct arr[], int n, key key, dir dir){
    for(int i = 0; i < n - 1; i ++){
        int k = i;
        for (int j = i + 1; j < n; j ++){
            if (compareRecords(&arr[j], &arr[k], key, dir) < 0){
                k = j;
            }
        } 
        phone_direct temp = arr[i];
        arr[i] = arr[k];
        arr[k] = temp;
        }
    }
 
    void printRecords(phone_direct arr[], int n){
        for (int i = 0; i < n; i ++){
            printf("%s %s: %s, Возраст: %d\n", arr[i].surname, arr[i].name, arr[i].phoneNumber, arr[i].age);
        }
    }

int main(){

    phone_direct records[] = {
        {"Сергей", "Демин", "8-983-123-78-73", 20},
        {"Петр", "Демин", "8-929-383-22-06", 17},
        {"Майкл", "Синицын", "8-952-949-91-31", 26},
        {"Андрюша", "Кутенков", "8-905-095-79-81", 19}
    };

    int n = sizeof(records) / sizeof(records[0]); 

    printf("Исходный массив:\n");
    printRecords(records, n);
    
    key key;
    printf("\nВыберите ключ сортировки:\n");
    printf("1 - По имени\n");
    printf("2 - По фамилии\n");
    printf("3 - По номеру телефона\n");
    printf("4 - По возрасту\n");
    int key_choice;
    scanf("%d", &key_choice);
    switch (key_choice) {
        case 1:
            key = sort_name;
            break;
        case 2:
            key = sort_surname;
            break;
        case 3:
            key = sort_phoneNumber;
            break;
        case 4:
            key = sort_age;
            break;
    }

    
    dir direction;
    printf("\nВыберите направление сортировки:\n");
    printf("1 - По возрастанию\n");
    printf("2 - По убыванию\n");
    int dir_choice;
    scanf("%d", &dir_choice);
    switch (dir_choice) {
        case 1:
            direction = sort_increasing;
            break;
        case 2:
            direction = sort_decreasing;
            break;
    }

    SelectSort(records, n, key, direction);

    printf("\nОтсортированный массив:\n");
    printRecords(records, n);
}
