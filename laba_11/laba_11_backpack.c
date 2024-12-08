#include <stdio.h>
#include <stdlib.h>
void bubble_sort(int n, int *array) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (array[j] < array[j + 1]) {
                int tmp = array[j + 1];
                array[j + 1] = array[j];
                array[j] = tmp;
}}}}

int sum_weight(int n,int *array){
    int sum=0;
    for (int i = 0; i < n; i++) {
        sum+=array[i];
    }return sum;}

int main(){
    int ingot_count,max_weight;
    printf("вместимость рюкзака и количество слитков: ");
    scanf("%d %d",&max_weight,&ingot_count);
    int *ingot_weight = calloc(ingot_count, sizeof(int));
    int *ingot_backpack = calloc(ingot_count, sizeof(int));
    
    for (int i = 0; i < ingot_count; i++){
        printf("вес %d слитка:", i+1);
        scanf("%d",&ingot_weight[i]);}

    bubble_sort(ingot_count,ingot_weight);

    for (int i = 0; i < ingot_count; i++) {
        if (sum_weight(i, ingot_backpack) + ingot_weight[i] <= max_weight) {
             
            ingot_backpack[i] = ingot_weight[i]; 
        }
    }
    printf("%d\n",sum_weight(ingot_count,ingot_backpack));

    free(ingot_weight);  
    free(ingot_backpack);
}


