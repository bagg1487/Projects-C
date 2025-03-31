#include "laba_10_header.h"
int main(){
    char sign;
    struct fraction a1,a2;
    printf("числитель первой дроби: "); scanf("%f",&a1.a);
    printf("знаменатель первой дроби: "); scanf("%f",&a1.b);
    printf("числитель второй дроби: "); scanf("%f",&a2.a);
    printf("знаменатель второй дроби: "); scanf("%f",&a2.b);
    printf("знак / или *: ");scanf("%s",&sign);
    if (a1.b == 0 || a2.b == 0) printf("Деление на 0((((\n");
    else{
    if  (sign == '/') division(a1.a/a1.b,a2.a/a2.b);
    else if (sign == '*')  multipication(a1.a/a1.b,a2.a/a2.b);
    else printf("знак не тот\n");}
}