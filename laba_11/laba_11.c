// #include <stdio.h>  
// #include <stdlib.h>  
// //тк делается копия массива при заведении его в функцию, мы делаем указатель на массив, чтобы передавать сам массив и изменять его элементы(мы передаем копию и возвращается не копия, а изначальный массив)  
// void init(int** arr, int n)  
// {  
// *arr = malloc(n * sizeof(int));      
// int i;  
// for (i = 0; i < n; ++i) {  
//         (*arr)[i] = i;  
//     }  
// }   
// int main()  {      
// int* arr = NULL;      
// int n = 10;      
// init(&arr, n);  
// int i;  
// for (i = 0; i < n; ++i) {  
// printf("%d\n", arr[i]);  
// }      
// return 0; 
// } 
#include <stdio.h>  
typedef struct {  
char str[3];      
int num;  
} NumberRepr;  
  
void format(NumberRepr* number) {
    sprintf(number->str, "%3d", number->num);}

int main() {  
NumberRepr number = { .num = 1025 };  
format(&number);  
printf("str: %s\n", number.str);      
printf("num: %d\n", number.num);  
return 0;}  
