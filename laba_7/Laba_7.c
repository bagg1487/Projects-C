
//Записать слова в предложении в обратном порядке.
#include <stdio.h>
#include <string.h>
int main() {
    char input_str[256],*words[256];
    int word_count = 0;
    fgets(input_str, sizeof(input_str), stdin);
    char *word = strtok(input_str, " \n");
    for (word_count;word!=0;word_count++){
        words[word_count] = word;
        word = strtok(0, " \n"); 
    }
    for (int i = word_count - 1; i >= 0; i--) {
        printf("%s ", words[i]);
    } printf("\n");
}