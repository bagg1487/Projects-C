#include <stdio.h>

int main() {
    int n;
    scanf("%d",&n);
    for (int i = n; i > 0; i--) {
         printf("\n");
        for (int m = 0;m<n-i;m++){
            printf(" ");
        }for (int j = 0; j < i; j++) {
            printf("*");} 
    }
}
