#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Student {
    char name[64];
    int group;
    int marks[3];
} Stud;

void createFile(char *filename){

    Stud students[] = {
        {"Иванов", 101, {5, 4, 3}},
        {"Петров", 102, {4, 4, 5}},
        {"Кушнарев", 101, {3, 3, 4}},
        {"Демин", 103, {5, 5, 5}},
        {"Чвяк", 102, {4, 3, 4}}
    };

    FILE *file;
    file = fopen(filename, "wb");

    if (file == NULL){
        printf("ошибка в открытии файла");
        exit(1);
    }

    for(int i = 0; i < sizeof(students) / sizeof(students[0]); i++)
        fwrite(&students[i], sizeof(Stud), 1, file);
    
    fclose(file);
}

void searchName(char *filename, char *name){
    FILE *file;
    file = fopen(filename, "rb");

    if (file == NULL){
        printf("ошибка в чтении файла");
        exit(1);
    }

    Stud student;

    int t = 0;
    while(fread(&student,sizeof(Stud),1,file) == 1){
        if (strcmp(student.name,name) == 0) {
            printf("Найденный студент: %s Группа: %d Оценки: %d %d %d\n", student.name, student.group, student.marks[0], student.marks[1], student.marks[2]);
            t = 1;
        }
    }

    if (t == 0)
        printf("Студент потерялся \n");

    fclose(file);
}

void searchGroup(char *filename, int group){
    FILE *file;
    file = fopen(filename, "rb");

    if (file == NULL){
        printf("ошибка в чтении файла");
        exit(1);
    }

    Stud student;

    int t = 0;
    while(fread(&student,sizeof(Stud), 1, file) == 1){
        if (student.group == group){
            printf("Найденный студент: %s Группа: %d Оценки: %d %d %d\n", student.name, student.group, student.marks[0], student.marks[1], student.marks[2]);
            t = 1;
        }
    }

    if (t == 0)
        printf("Группа исчезла \n");

    fclose(file);
}

void searchMark(char *filename, int mark){
    FILE *file;
    file = fopen(filename, "rb");

    if (file == NULL){
        printf("ошибка в чтении файла");
        exit(1);
    }

    Stud student;

    int t = 0;
    while(fread(&student, sizeof(Stud), 1, file) == 1){
        for (int i = 0; i < sizeof(student.marks); i++){
            if (student.marks[i] == mark){
                printf("Найденный студент: %s Группа: %d Оценки: %d %d %d\n", student.name, student.group, student.marks[0], student.marks[1], student.marks[2]);
                t = 1;
                break;
            }
        }
    }

    if (t == 0)
        printf("Оценки испарились \n");

    fclose(file);
}

void choiceStudent(char *filename, int choice){
    switch(choice){
        case 1:
            char name[64];
            printf("Введите фамилию: ");
            getchar();
            fgets(name, sizeof(name), stdin);
            name[strcspn(name, "\n")] = '\0';
            searchName(filename, name);
            break;
        case 2:
            int group;
            printf("Введите группу: ");
            scanf(" %d", &group);
            searchGroup(filename, group);
            break;
        case 3:
            int mark;
            printf("Введите оценку: ");
            scanf(" %d", &mark);
            searchMark(filename, mark);
            break;
    }
}


int main(){
    char *filename = "Wedomost.dat";
    createFile(filename);

    int choice;
    printf("Выберите поле для поиска \n 1 - Имя \n 2 - Группа \n 3 - Оценка \n");
    scanf(" %d", &choice);
    choiceStudent(filename, choice);
}
