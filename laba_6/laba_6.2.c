#include <stdio.h>
int main() {
    int A[5]={2,33,4,56,23};
    int B[5]={-34,25,-98,23,-3};
    int C[5]={0};
    int max_A = A[0], negative_B=0,sum_C=0;
    for(int i;i<5;i++){
        if (B[i]<0){
           negative_B++;}
        C[i]=A[i]+B[i];
        sum_C += C[i];
        if (max_A<A[i]){
            max_A=A[i];}
    } printf("Max in A: %d \n Count negative in B: %d \n Average summary C: %.3lf \n",max_A,negative_B,(double)sum_C/5);
}
