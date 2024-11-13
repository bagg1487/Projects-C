//16 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
void print_matrix(float **arr,int N){
     for(int i=0;i<N;i++){
        for(int j=0;j<N;j++)
            printf("%8.2lf",arr[i][j]);
        printf("\n");}}
// 1)Элементы  строк  матрицы увеличить на средние значения положительных элементов своих строк. 
void average_string(float **arr,int N){
    float *sum_string = (float *)calloc(N, sizeof(float));
    float sum=0;
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            if (arr[i][j]>=0)
                sum+=arr[i][j];
            }
        sum=sum/N;
        sum_string[i]=sum;
            }
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++)
            arr[i][j]+=sum_string[i];
        }free(sum_string);}
// 2)Определить сумму всех элементов матрицы. 
void sum_matrix(float **arr,int N){
    float sum=0;
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++)
            sum+=arr[i][j];} 
printf("Сумма всех элементов матрицы равна: %.2lf\n",sum);}
// 3)Сформировать массив из элементов, расположенных выше главной и побочной диагоналей. 
void new_array(float **arr,int N){
    int max_size;
    max_size=(N*N)-N;
    float *array = (float *)calloc(max_size, sizeof(float));
    int index=0;
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            if (j < N - i - 1)
                array[index++]=arr[i][j];    
        }}
    index=((N*N)-N)/2;
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            if (j > i)
                array[index++]=arr[i][j];    
        }}
    for(int i=0;i<max_size;i++) printf("%8.2lf",array[i]);
    printf("\n");
    free(array);
}
// 4)Вычислить количество отрицательных элементов полученного массива.
void count_negative(float **arr,int N){
    int count=0;
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            if (arr[i][j]<0) count++;}}
    printf("Количество отрицательных элементов матрицы равно: %d\n",count);
}
int main(){
    int N;
    printf("Введите размер матрицы ");
    scanf("%d",&N);
    srand(time(NULL));
    if(N<2 || N>15) printf("uncorrect number\n  ");
    else{
    float **arr2 = (float **)malloc(N * sizeof(float *)); 
    if (arr2 == NULL) {
        printf("Error in malloc arr2!\n");
        return -1;
    }
    for (int i = 0; i < N; i++) {
        arr2[i] = (float *)malloc(N * sizeof(float)); 
        if (arr2[i] == NULL) {
            printf("Error in malloc arr2!\n");
            return -1;
        }}
    
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++)
            arr2[i][j]=-10 + rand()%20;}
        
    print_matrix(arr2,N);
    printf("--------------------------------------------------\n");
    average_string(arr2,N);
    print_matrix(arr2,N);
    sum_matrix(arr2,N);
    count_negative(arr2,N);
    new_array(arr2,N);

    for (int i = 0; i < N; i++) {
        free(arr2[i]);
        arr2[i] = NULL;}
    free(arr2);
    arr2 = NULL;
    }
}
