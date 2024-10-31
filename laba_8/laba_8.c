//реализовать алгоритм Крамера для решения СЛАУ NxN, где N может быть от 2 до 3. N in range (2,3)
#include <stdio.h>
#include <math.h>
#define N 2
#define M 3
//определитель для матрицы 2x2
double det2(double m[N][N]){ 
    return m[0][0]*m[1][1]-m[0][1]*m[1][0];}
//определитель для матрицы по x 2x2
double detx2(double m[N][N],double o[N]){
    double m_2[N][N];
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) 
            m_2[i][j]=m[i][j];}
    for(int i=0;i<N;i++) m_2[i][0]=o[i];
    return m_2[0][0]*m_2[1][1]-m_2[0][1]*m_2[1][0];}
//определитель для матрицы по y 2x2
double dety2(double m[N][N],double o[N]){
    double m_2[N][N];
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) 
            m_2[i][j]=m[i][j];}
    for(int i=0;i<N;i++) m_2[i][1]=o[i];
   return m_2[0][0]*m_2[1][1]-m_2[0][1]*m_2[1][0];}
//определитель для матрицы 3x3 
double det3(double m[M][M]){
    return m[0][0]*m[1][1]*m[2][2]+m[0][1]*m[2][0]*m[1][2]+m[0][2]*m[1][0]*m[2][1]-m[2][0]*m[1][1]*m[0][2]-m[0][0]*m[2][1]*m[1][2]-m[1][0]*m[0][1]*m[2][2];}
//определитель для матрицы по x 3x3
double detx3(double m[M][M],double o[M]){
    double m_2[M][M];
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < M; j++) 
            m_2[i][j]=m[i][j];}
    for(int i=0;i<M;i++) m_2[i][0]=o[i];
    return m_2[0][0]*m_2[1][1]*m_2[2][2]+m_2[0][1]*m_2[2][0]*m_2[1][2]+m_2[0][2]*m_2[1][0]*m_2[2][1]-m_2[2][0]*m_2[1][1]*m_2[0][2]-m_2[0][0]*m_2[2][1]*m_2[1][2]-m_2[1][0]*m_2[0][1]*m_2[2][2];}
//определитель для матрицы по y 3x3
double dety3(double m[M][M],double o[M]){
    double m_2[M][M];
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < M; j++) 
            m_2[i][j]=m[i][j];}
    for(int i=0;i<M;i++) m_2[i][1]=o[i];
    return m_2[0][0]*m_2[1][1]*m_2[2][2]+m_2[0][1]*m_2[2][0]*m_2[1][2]+m_2[0][2]*m_2[1][0]*m_2[2][1]-m_2[2][0]*m_2[1][1]*m_2[0][2]-m_2[0][0]*m_2[2][1]*m_2[1][2]-m_2[1][0]*m_2[0][1]*m_2[2][2];}
//определитель для матрицы по z 3x3
double detz3(double m[M][M],double o[M]){
    double m_2[M][M];
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < M; j++) 
            m_2[i][j]=m[i][j];}
    for(int i=0;i<M;i++) m_2[i][2]=o[i];
    return m_2[0][0]*m_2[1][1]*m_2[2][2]+m_2[0][1]*m_2[2][0]*m_2[1][2]+m_2[0][2]*m_2[1][0]*m_2[2][1]-m_2[2][0]*m_2[1][1]*m_2[0][2]-m_2[0][0]*m_2[2][1]*m_2[1][2]-m_2[1][0]*m_2[0][1]*m_2[2][2];}

int main(){
    int n;
    printf("Введите размер матрицы: \n");
    scanf("%d",&n);

    if (n==N){
    double matrix[n][n],mat_rav[n];
    printf("Введите элементы матрицы:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) 
            scanf("%lf", &matrix[i][j]);}     

    printf("Введите вторую часть уравнения:\n");
    for (int j = 0; j < n; j++) scanf("%lf", &mat_rav[j]);

    double det = det2(matrix);
    double detx = detx2(matrix,mat_rav);
    double dety = dety2(matrix,mat_rav);
    printf("x=%.3lf y=%.3lf \n",detx/det,dety/det);}

    else if (n==3){
        double matrix[n][n],mat_rav[n];
        printf("Введите элементы матрицы:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) 
            scanf("%lf", &matrix[i][j]);}     

    printf("Введите вторую часть уравнения:\n");
    for (int j = 0; j < n; j++) scanf("%lf", &mat_rav[j]);
    double det = det3(matrix);
    double detx = detx3(matrix,mat_rav);
    double dety = dety3(matrix,mat_rav);
    double detz = detz3(matrix,mat_rav);
    printf("x=%.3lf y=%.3lf z=%.3lf\n",detx/det,dety/det,detz/det);}

    else printf("error");}