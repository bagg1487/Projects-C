//Тинькофф 4
#include <stdio.h>
#include <math.h>
int simple_num(int num){
    // if (num<2) num=2;
    for (int i = 2; i<=sqrt(num);i++){
        if(num%i==0) return 0;
    }return 1;}
int count_del(int num){ 
    int count=0;
    for (int i = 1;i<=num;i++){
        if (num % i == 0){
            count++;
        }}return count;}
int main(){
    int min,max,count=0,c_del;
    scanf("%d %d",&min,&max);
    for(int i=min;i<=max;i++){
        if (simple_num(i) == 0) {
            if (simple_num(count_del(i))) count++;}
    }
    printf("%d\n",count);
}