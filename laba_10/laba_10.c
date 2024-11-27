#include <stdio.h>
struct fraction {float a,b;};
void multipication(float a,float b){printf("%f", a*b);}
void division (float a,float b){printf("%f", a/b);}
int main(){
    char sign;
    struct fraction a1,a2;
    scanf("%f",&a1.a);
    scanf("%f",&a1.b);
    scanf("%f",&a2.a);
    scanf("%f",&a2.b);
    scanf("%s",&sign);
    if  (sign == '/') division(a1.a/a1.b,a2.a/a2.b);
    else multipication(a1.a/a1.b,a2.a/a2.b);
}

