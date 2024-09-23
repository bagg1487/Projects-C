#include <stdio.h>
int main() {
	int year;
	printf("Введите год: \n");
	scanf("%d", &year);
	if (year%400==0) {
		printf("YES\n");
	} else if (year%100==0) {
		printf("NO\n");
	} else if (year%4==0) {
		printf("YES\n");
	} else 
		printf("NO\n");		
 		
}
