#include <stdio.h>
#include <math.h>
double recurse_pi(int n) {
    double E = pow(10,-4);
    int count;
    double part = 4.0 / (2 * n + 1);  
    if (n % 2 != 0) part = -part;
    printf("%f\n",part);
    while (fabs(part) <= E) {
        count = n + 1; 
        printf("Число членов ряда: %d\n",count);
        return part;
    }
    return part + recurse_pi(n + 1);
}
int main() {
    double pi = recurse_pi(0);
    printf("Вычисленное значение: %.10f\n", pi);
    printf("Значение из библиотеки: %.10f\n", M_PI);
    printf("Разница между значениями: %.10f\n", fabs(pi - M_PI));
}
