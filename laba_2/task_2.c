#include <stdio.h>
int main() {
	float temp;
	char xx;
	printf("Какая температура?\n");
	scanf("%f %c", &temp, &xx);
	if (xx == 'c'|| xx == 'C'){
		printf("Ткмпература C: %.2f\n", (temp*1.8)+32);
	} else if (xx == 'f'|| xx == 'F'){		
		printf("Температура F: %.2f\n", (temp-32)/1.8);
	} else 
		printf("ошибка\n");

}
