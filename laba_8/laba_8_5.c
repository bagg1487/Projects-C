#include <stdio.h>
#define M 15  
double determinant(double matrix[M][M], int n){ 
    double det = 0;
    if (n == 1) {
        return matrix[0][0];} 
    else if (n == 2) {
        return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];} 
    else {
        double submatrix[M][M];
        for (int x = 0; x < n; x++) {
        int subi = 0;
        for (int i = 1; i < n; i++) {
        int subj = 0;
        for (int j = 0; j < n; j++) {
        if (j == x) continue;
        submatrix[subi][subj] = matrix[i][j];
        subj++;}
        subi++;}
        det += (x%2==0?1:-1) * matrix[0][x] * determinant(submatrix, n - 1);
        }
    }
    return det;
}

int main() {
    int n;
    double matrix[M][M],pos[M],new[M];
    printf("Введите размер матрицы: ");
    scanf("%d", &n);
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++){
            printf("Введите (%d,%d) элемент матрицы:\n",j+1,i+1); 
            scanf("%lf", &matrix[i][j]);
        } 
            }
    
    for (int i = 0; i < n; i++){
        printf("Введите %d элемент после равно:\n",i+1);
        scanf("%lf", &pos[i]);}
    double delta = determinant(matrix, n); 
    if (delta == 0){
        printf("Нет единственного решения. \n");
    }
    else{
        for (int b=0; b<n; b++){
            for (int m=0; m<n; m++ ){
                new[m]=matrix[b][m];
                matrix[b][m]=pos[m];
            }
            double l = determinant(matrix, n)/delta;
            printf("x: %.2lf \n", l);
            for (int t=0; t<n; t++){
                matrix[b][t]=new[t];
            }
        }
    }
}