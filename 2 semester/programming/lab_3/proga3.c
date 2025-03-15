#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char *argv[]){

    if (argc != 2) {
        perror("Using incorrect number of arguments");
        exit(1);
    }

    FILE *file;
    file = fopen(argv[1], "w");

    if (file == NULL){
        perror("Error opening file");
        exit(1);
    }

    for(double x = 0.0; x < 3.1; x += 0.1){
        double y = sin(x);
        double z = tan(x);
        fprintf(file, "x=%.1f\tsin(x)=%.2f\ttg(x)=%e\n", x, y, z);
    }

    if (fclose(file) != 0) {
        perror("Error closing file");
        exit(1);
    }
}
//gcc proga3.c -lm
//./a.out proga3.txt
//cat proga3.txt
