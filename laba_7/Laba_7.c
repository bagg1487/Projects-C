//Записать слова в предложении в обратном порядке.
#include <stdio.h>
#include <string.h>
int main(){
    char input_str[256],a;
    fgets(input_str,256,stdin);
    for(int i = strlen(input_str);i>=0;i--){
        char a = input_str[i];
        printf("%c",a); 
    }
}
