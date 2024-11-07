#include <stdio.h>
void spiral(int n) {
    int matrix[n][n],left = 0, right = n - 1, top = 0, bottom = n - 1,num = 1;
    while (num <= n * n) {
        for (int i = left; i <= right; i++)
            matrix[top][i] = num++;
        top++;
        for (int i = top; i <= bottom; i++) 
            matrix[i][right] = num++;
        right--;
        for (int i = right; i >= left; i--)
            matrix[bottom][i] = num++;
        bottom--;
        for (int i = bottom; i >= top; i--) 
            matrix[i][left] = num++;
        left++;}
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) 
            printf("%2d ", matrix[i][j]);
        printf("\n");
    }
}
int main(){
    int n;
    printf("Размер матрицы: ");
    scanf("%d",&n);
    spiral(n);
}