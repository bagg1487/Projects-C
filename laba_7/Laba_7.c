//Определить, есть ли в заданном предложении цифры. И если есть, найти их сумму.
#include <stdio.h>
#include <string.h>
#include <ctype.h>
int main(){
     char input_str[256];
     int sum=0;
    fgets(input_str,256,stdin);
    for(int i = 0;i<strlen(input_str);i++){
        if (isdigit(input_str[i])) sum+=input_str[i]-'0';
        } printf("%d\n",sum);  
    }
