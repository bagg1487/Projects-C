#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

int THREADS, N, **A, **B, **C;

void* multiplication(void* arg) {
    int thread_id = *(int*)arg;
    int stroki = N / THREADS;
    int start = thread_id * stroki;
    int end = (thread_id == THREADS - 1) ? N : start + stroki; 
    
    for (int i = start; i < end; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = 0;
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return NULL; 
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <matrix_size> <threads>\n", argv[0]);
        return 1;
    }

    N = atoi(argv[1]);
    THREADS = atoi(argv[2]);
    if (N <= 0 || THREADS <= 0) {
        fprintf(stderr, "Error: N and THREADS must be positive\n");
        return 1;
    }

    A = (int**)malloc(N * sizeof(int*));
    B = (int**)malloc(N * sizeof(int*));
    C = (int**)malloc(N * sizeof(int*));
    if (!A || !B || !C) {
        perror("malloc failed");
        return 1;
    }

    for (int i = 0; i < N; i++) {
        A[i] = (int*)malloc(N * sizeof(int));
        B[i] = (int*)malloc(N * sizeof(int));
        C[i] = (int*)malloc(N * sizeof(int));
        if (!A[i] || !B[i] || !C[i]) {
            perror("malloc failed");
            return 1;
        }
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = 1;
            B[i][j] = 1;
        }
    }

    pthread_t threads[THREADS];
    int thread_ids[THREADS];
    struct timeval start, end;

    gettimeofday(&start, NULL);

    for (int i = 0; i < THREADS; i++) {
        thread_ids[i] = i;
        if (pthread_create(&threads[i], NULL, multiplication, &thread_ids[i]) != 0) {
            perror("pthread_create failed");
            return 1;
        }
    }

    for (int i = 0; i < THREADS; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("pthread_join failed");
            return 1;
        }
    }

    gettimeofday(&end, NULL);
    double time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;

    for (int i = 0; i < N; i++) {
        free(A[i]);
        free(B[i]);
        free(C[i]);
    }
    free(A);
    free(B);
    free(C);

    printf("%f\n", time);
}
