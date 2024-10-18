#include <stdio.h>
int main() {
    int array[7]={17,53,28,3332,1,1333,66};
    int max_1 = array[0];
    int ave_bef = 0,ave_aft = 0;
    for(int i=0;i<7;i++){
        ave_bef+=array[i];
        if (max_1<array[i])
            max_1=array[i];
        if (array[i]>= 11 && array[i]<=97){
            for(int a=2;a<array[i];a++){
                if (array[i]%a==0)
                    break;
                else{
                    array[i]=0;
                }
            }
        }    
    }
    int max_2 = array[0];
    for(int m;m<7;m++){
        ave_aft+=array[m];
        if (max_2<array[m])
            max_2=array[m];
        }
    if (max_1 != max_2){
        printf("max changed \n");
    } else{
        printf("max is not changed \n");
    }
    printf("average before: %d average after: %d",ave_bef/7,ave_aft/7);//690 680
}
