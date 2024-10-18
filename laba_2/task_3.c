#include <stdio.h>
#include <math.h>
int main() {
	int x = 0, y = 0, r1 = 1, r2 = 2;
	printf("x: \n");
	scanf("%d", &x);
	printf("y: \n");
	scanf("%d", &y);
	if (x*x + y*y < r2*r2 && x*x + y*y > r1*r1 && x >= 0){
		printf("Да \n");
}	else
		printf("Не \n");
}

