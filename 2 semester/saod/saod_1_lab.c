#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void FillInc(int n,int *A){
    for(int i = 0; i < n ; i++)
        A[i]=i+1;}

void FillDec(int n,int *A){
    for(int i = 0; i < n ; i++)
        A[i]=n-i;}

void FillRand(int n, int *A){
    srand(time(NULL));
    for(int i = 0; i < n; i++) 
        A[i]=rand() % 10;}

int CheckSum(int n,int *A){
    int sum = 0;
    for(int i = 0; i < n; i++)
        sum+=A[i];
    printf("контрольная сумма ");
    return sum;}

void RunNumber(int n, int *A){
    int SumSeriy = 1;
    for(int i = 0; i < n-1; i++){
        if(A[i]>=A[i+1])
            SumSeriy++;
    }
    printf("количество серий %d\n",SumSeriy);}

void PrintMas(int n, int *A){
    for(int i = 0; i < n; i++)
        printf("%d ",A[i]);
    printf("\n");}

void SelectSort(int n,int *A){
    int swap = 0, index = 0,C = 0, M = 0;
    for(int i = 0; i < n - 1; i++){
        index = i;
        for(int j = i + 1; j  < n; j++){
            C++;
            if(A[j] < A[index])
                index = j;    
        }
        swap = A[index];
        A[index] = A[i];
        A[i] = swap;
        M+=1;
        
    }
    printf("Обычный SelectSort\nСравнения: %d Пересылки: %d Сумма: %d\n",C,M, C + M);
}

void UpdatedSelectSort(int n,int *A){
    int swap = 0, index = 0,C = 0, M = 0;
    for(int i = 0; i < n - 1; i++){
        index = i;
        for(int j = i + 1; j  < n; j++){
            C++;
            if(A[j] < A[index])
                index = j;    
        }
        if (index != i){
            swap = A[index];
            A[index] = A[i];
            A[i] = swap;
            M+=1;
        }
    }
    printf("Улучшенный SelectSort\nСравнения: %d Пересылки: %d Сумма: %d\n",C,M,C + M);
}

int main(){
    const int n = 100;
    int array[100]={0};
    int checksum;
    // теоретическое число сравнений для массива с n = 10: C = n * (n - 1) / 2 = 45; число пересылок M = n - 1 = 9
    // теоретическое число сравнений для массива с n = 100: C = n * (n - 1) / 2 = 4950; число пересылок M = n - 1 = 99
    printf("возрастание\n");
    FillInc(n, array); 
    PrintMas(n, array);
    printf("%d\n",CheckSum(n,array));
    RunNumber(n,array);
    SelectSort(n,array);
    printf("%d\n",CheckSum(n,array));
    RunNumber(n,array);

    FillInc(n, array); 
    PrintMas(n, array);
    printf("%d\n",CheckSum(n,array));
    RunNumber(n,array);
    UpdatedSelectSort(n,array);
    printf("%d\n",CheckSum(n,array));
    RunNumber(n,array);

    printf("\nубывание\n");
    FillDec(n, array);
    PrintMas(n, array);
    printf("%d\n",CheckSum(n,array));
    RunNumber(n,array);
    SelectSort(n,array);
    printf("%d\n",CheckSum(n,array));
    RunNumber(n,array);

    FillDec(n, array);
    PrintMas(n, array);
    printf("%d\n",CheckSum(n,array));
    RunNumber(n,array);
    UpdatedSelectSort(n,array);
    printf("%d\n",CheckSum(n,array));
    RunNumber(n,array);


    printf("\nрандом\n");

    FillRand(n, array);
    PrintMas(n, array);
    printf("%d\n",CheckSum(n,array));
    RunNumber(n,array);
    SelectSort(n,array);
    printf("%d\n",CheckSum(n,array));
    RunNumber(n,array);

    FillRand(n, array);
    PrintMas(n, array);
    printf("%d\n",CheckSum(n,array));
    RunNumber(n,array);
    UpdatedSelectSort(n,array);
    printf("%d\n",CheckSum(n,array));
    RunNumber(n,array);
}
