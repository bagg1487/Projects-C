#include <stdio.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int turn = 0;

void* child_thread(void* arg) {
    char* message = (char*)arg;
    
    for (int i = 1; i < 6; i++) {
        while (1) {
            pthread_mutex_lock(&mutex);
            if (turn == 1) break;
            pthread_mutex_unlock(&mutex);
        }

        printf("Дочерний поток %s: %d\n", message, i);
        turn = 0;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t thread[4];
    char* messages[] = {"num_1", "num_2", "num_3", "num_4"};
    
    for (int i = 0; i < 4; i++) {
        pthread_create(&thread[i], NULL, child_thread, messages[i]);
    }

    for (int i = 1; i < 6; i++) {
        while (1) {
            pthread_mutex_lock(&mutex);
            if (turn == 0) break;
            pthread_mutex_unlock(&mutex);
        }

        printf("Главный поток: %d\n", i);
        turn = 1;
        pthread_mutex_unlock(&mutex);
    }

    for (int i = 0; i < 4; i++) {
        pthread_join(thread[i], NULL);
    }
    
    pthread_mutex_destroy(&mutex);
    
}
