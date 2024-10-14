#include <stdio.h>
int f(unsigned int a){
	unsigned int b =~ a;
	int count = 0;
	while ((b&1)==0){
		b = b>>1;
		count++;
	} return count;
}
int main(){
unsigned int a;
scanf("%u", &a);
int res = f(a);
printf("%d \n",res);
}

