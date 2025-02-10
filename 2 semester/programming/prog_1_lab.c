#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#define N 100
#define MAX 300

struct Student{
    char name[64];
    int math,phy,inf,sum;
};

struct Student AddStudent(char *name, int math, int phy, int inf){
    struct Student NewStudent;
    strncpy(NewStudent.name,name,sizeof(NewStudent.name)-1);
    NewStudent.name[sizeof(NewStudent.name)-1] = '\0';
    NewStudent.math = math;
    NewStudent.phy = phy;
    NewStudent.inf = inf;
    NewStudent.sum = math + inf + phy;
    return NewStudent;
    }

void printStudentInfo(struct Student student){
    printf("Имя студента: %s\n", student.name);
    printf("Оценка по математике: %d\n", student.math);
    printf("Оценка по физике: %d\n", student.phy);
    printf("Оценка по информатике: %d\n", student.inf);
    printf("Общий балл: %d\n", student.sum);
} 

void generateRandomName(char *mas, int len){
    char alf[] = "QWERTYUIOPLKMJNHBGVFCDXSZA";
    for (int i = 0; i < len - 1; i++) {
        mas[i] = alf[rand() % (sizeof(alf) - 1)];
    }
    mas[len - 1] = '\0';
}

void SelectSort(int n,struct Student *students){
    for (int i = 0; i < n; i++) printStudentInfo(students[i]);
    printf("-----------------------------------------------------------------------\n");

    int index = 0;
    struct Student swap;
    for(int i = 0; i < n - 1; i++){
        index = i;
        for(int j = i + 1; j  < n; j++){
            if(students[j].sum > students[index].sum)
                index = j;}
        if (index != i){
            swap = students[index];
            students[index] = students[i];
            students[i] = swap;   
        }
    }
    for (int i = 0; i < n; i++) printStudentInfo(students[i]);
}

void CountingSort(int n,struct Student *students) {
    for (int i = 0; i < n; i++) printStudentInfo(students[i]);
    printf("-----------------------------------------------------------------------\n");

    int* count = (int*)calloc(MAX + 1, sizeof(int));

    for (int i = 0; i < n; i++) 
        count[students[i].sum]++;
    int index = 0;
    for (int i = MAX; i >= 0; i--) {
        while (count[i] > 0) {
            students[index++].sum = i;
            count[i]--;
        }
    }
    free(count);
    for (int i = 0; i < n; i++) printStudentInfo(students[i]);
}


int main(){
    srand(time(NULL));

    struct Student students[N]; 
    int random_number = 2 + rand() % (63);
    char randName[random_number];
    int n = 5;
    
    for (int i = 0; i < N; i++) {
        char randomName[10];
        generateRandomName(randomName, sizeof(randomName));
        int math = rand() % 101;       
        int phy = rand() % 101;
        int inf = rand() % 101;
        students[i] = AddStudent(randomName, math, phy, inf);
    }

    
    CountingSort(n,students);
    
}
