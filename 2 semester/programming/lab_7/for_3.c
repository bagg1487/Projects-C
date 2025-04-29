#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void clean(void *arg){
    char* message= (char*)arg;
    printf("Поток %s завершен\n",message);
}

void *function(void *arg){
    char* message= (char*)arg;
    
    pthread_cleanup_push(clean,message);
    
    for (int i = 1; i < 4; i++){
        printf("Поток %d  %s%d\n", i, message, i);
        sleep(1);
    }
    pthread_cleanup_pop(0);
    return NULL;
}

int main(){
    pthread_t thread[4];
    char* message[]={"num_1.","num_2.","num_3.","num_4."};

    for (int i = 0; i < 4; i++)
        pthread_create(&thread[i], NULL, function, message[i]);

    sleep(2);

    for (int i=0;i<4;i++)
        pthread_cancel(thread[i]);
    for (int i=0;i<4;i++)
        pthread_join(thread[i],NULL);
  
}
