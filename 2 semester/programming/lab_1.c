#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#define MAX 300
#define N 100000

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
    clock_t start = clock();
    // for (int i = 0; i < n; i++) printStudentInfo(students[i]);
    // printf("-----------------------------------------------------------------------\n");

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
    clock_t end = clock();
    // for (int i = 0; i < n; i++) printStudentInfo(students[i]);
    printf("Время работы программы: %f секунд \n Размер данных: %ld байт",(double)(end - start) / CLOCKS_PER_SEC,n*sizeof(int));
}

void CountingSort(int n,struct Student *students) {
    clock_t start = clock();
    // for (int i = 0; i < n; i++) printStudentInfo(students[i]);
    // printf("-----------------------------------------------------------------------\n");

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
    clock_t end = clock();
    // for (int i = 0; i < n; i++) printStudentInfo(students[i]);
    printf("Время работы программы: %f секунд \n Размер данных: %ld байт",(double)(end - start) / CLOCKS_PER_SEC,n*sizeof(int));
}

void CpuInfo(){
    system("grep 'model name' /proc/cpuinfo | uniq");
    system("grep 'cpu MHz' /proc/cpuinfo | uniq");
}
void RandomStudents(struct Student *students){
    for (int i = 0; i < N; i++) {
        char randomName[10];
        generateRandomName(randomName, sizeof(randomName));
        int math = rand() % 101;
        int phy = rand() % 101;
        int inf = rand() % 101;
        students[i] = AddStudent(randomName, math, phy, inf);
    }
}

void swap(struct Student *a, struct Student *b) {
    struct Student temp = *a;
    *a = *b;
    *b = temp;
}

int partition(struct Student *students, int low, int high, int (*compare)(struct Student, struct Student)) {
    int pivot = students[high].sum; 
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (compare(students[j], students[high]) < 0) { 
            i++;
            swap(&students[i], &students[j]);
        }
    }
    swap(&students[i + 1], &students[high]);
    return i + 1;
}

void quicksort(struct Student *students, int low, int high, int (*compare)(struct Student, struct Student)) {
    if (low < high) {
        int pi = partition(students, low, high, compare);
        quicksort(students, low, pi - 1, compare);
        quicksort(students, pi + 1, high, compare);
    }
}

void QuickSort(int n, struct Student *students, int (*compare)(struct Student, struct Student)) {
    for (int i = 0; i < n; i++) printStudentInfo(students[i]);
    printf("-----------------------------------------------------------------------\n");
    quicksort(students, 0, n - 1, compare);
    for (int i = 0; i < n; i++) printStudentInfo(students[i]);
}

int compareBySum(struct Student a, struct Student b) {return b.sum - a.sum;}
int compareByPhy(struct Student a, struct Student b) {return b.phy - a.phy;}
int compareByMath(struct Student a, struct Student b) {return b.math - a.math;}
int compareByInf(struct Student a, struct Student b) {return b.inf - a.inf;} 
int main(){
    srand(time(NULL));

    struct Student students[N]; 
    int n = 5;
    RandomStudents(students);

    CpuInfo();
    QuickSort(n, students, compareByMath);
    
}
