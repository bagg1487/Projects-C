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
    printf("Контрольная сумма ");
    return sum;}

void RunNumber(int n, int *A){
    int SumSeriy = 1;
    for(int i = 0; i < n-1; i++){
        if(A[i]>=A[i+1])
            SumSeriy++;
    }
    printf("Количество серий %d \n\n",SumSeriy);}

void PrintMas(int n, int *A){
    for(int i = 0; i < n; i++)
        printf("%d ",A[i]);
    printf("\n");}

void SelectSort(int n,int *A){
    printf("Массив до сортировки\n");
    PrintMas(n, A);
    printf("%d\n",CheckSum(n,A));
    RunNumber(n,A);
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
        M+=3;  
    }
    printf("Обычный SelectSort\n Сравнения: %d Пересылки: %d Сумма: %d\n",C,M, C + M);
    printf("%d\n",CheckSum(n,A));
    RunNumber(n,A);}

void UpdatedSelectSort(int n,int *A){
    printf("Массив до сортировки\n");
    PrintMas(n, A);
    printf("%d\n",CheckSum(n,A));
    RunNumber(n,A);
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
            M+=3;
        }
    }
    printf("Улучшенный SelectSort\n Сравения: %d Пересылки: %d Сумма: %d\n",C,M,C + M);
    printf("%d\n",CheckSum(n,A));
    RunNumber(n,A);}

void BubbleSort(int n, int *A) {
    printf("Массив до сортировки\n");
    PrintMas(n, A);
    printf("%d\n",CheckSum(n,A));
    RunNumber(n,A);
    int C = 0, M = 0;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            C++;
            if (A[j] > A[j + 1]) {
                int tmp = A[j + 1];
                A[j + 1] = A[j];
                A[j] = tmp;
                M += 3;
            }
        }
    }
    printf("BubbleSort\n Сравения: %d Пересылки: %d Сумма: %d\n",C ,M ,C + M);
    printf("%d\n",CheckSum(n, A));
    RunNumber(n, A);}

    void ShakerSort(int n, int *A) {
        printf("Массив до сортировки\n");
        PrintMas(n, A);
        printf("%d\n", CheckSum(n, A));
        RunNumber(n, A);
        int left = 0, right = n - 1;
        int C = 0, M = 0;
        int swap_flag;
        do {
            swap_flag = 0;
            for (int i = left; i < right; i++) {
                C++;
                if (A[i] > A[i + 1]) {
                    int tmp = A[i];
                    A[i] = A[i + 1];
                    A[i + 1] = tmp;
                    M += 3;
                    swap_flag = 1;
                }
            }
            right--;
            for (int i = right; i > left; i--) {
                C++;
                if (A[i] < A[i - 1]) {
                    int tmp = A[i];
                    A[i] = A[i - 1];
                    A[i - 1] = tmp;
                    M += 3;
                    swap_flag = 1;
                }
            }
            left++;
        } while (swap_flag);
        printf("ShakerSort\n Сравнения: %d Пересылки: %d Сумма: %d\n", C, M, C + M);
        printf("%d\n", CheckSum(n, A));
        
    }

void Increase(int n, int *A){
    printf("Возрастание\n");

    // FillInc(n, A); 
    // SelectSort(n, A);
    
    // FillInc(n, A); 
    // UpdatedSelectSort(n,A);

    // FillInc(n, A); 
    // BubbleSort(n, A);
        FillInc(n, A); 
        ShakerSort(n, A);
}

void Decrease(int n, int *A){
    printf("\n Убывание\n");

    // FillDec(n, A);
    // SelectSort(n, A);
    
    // FillDec(n, A);
    // UpdatedSelectSort(n, A);

    // FillDec(n, A); 
    // BubbleSort(n, A);
        FillDec(n, A); 
        ShakerSort(n, A);
    }

void Random(int n, int *A){
    printf("\n Рандом\n");
    
    // FillRand(n, A);
    // SelectSort(n, A);

    // FillRand(n, A);
    // UpdatedSelectSort(n, A);
    
    // FillRand(n, A); 
    // BubbleSort(n, A);
        FillRand(n, A); 
        ShakerSort(n, A);
}



int main(){
    const int n = 100;
    int array[100]={0};
    int checksum;
  
    Increase(n,array);
    Decrease(n,array);
    Random(n,array);
    
}
