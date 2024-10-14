#include <stdio.h>

int f(unsigned int a){
    unsigned int b = a;
    int count =0;
    b=b>>16;
    return b&0xFF; 
    }
int main(){
    int x,res;
    scanf("%x",&x);
    res=f(x);
    printf("%x",res);
}
