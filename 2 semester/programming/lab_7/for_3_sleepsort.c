#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

void *sleepsort(void *arg){
    int thr = *(int*)arg;
    sleep(thr);
    printf("Поток со временем %d завершился \n", thr);
    return NULL;
}

int main(){
    int arr[]={1, 4, 3, 2, 9, 10, 8};
    int n = sizeof(arr) / sizeof(int);
    pthread_t thread[n];
    for (int i = 0; i < n; i++){
        pthread_create(&thread[i], NULL, sleepsort, &arr[i]);
    }
    for (int i = 0; i < n; i++){
        pthread_join(thread[i], NULL);
    }
}
