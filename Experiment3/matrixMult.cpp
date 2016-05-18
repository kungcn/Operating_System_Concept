#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#define M 3
#define K 2
#define N 3
#define NUM_THREADS M*N
int A[M][K] = {{1, 4}, {2, 5}, {3, 6}};
int B[K][N] = {{8, 7, 6}, {5, 4, 3}};
int C[M][N];
struct v {
    int i;
    int j;
};
void* getOneElem(void* data) {
    int i,j,k;
    struct v * d = (struct v*)data;
    i = d->i;
    j = d->j;
    /*计算元素Cij的值*/
    for (k = 0; k < K; k++) {
        C[i][j] = A[i][k] * B[k][j] + C[i][j];
    }
}

int main(int argc, char *argv[]) {
    
    int i,j;
    pthread_t tid[NUM_THREADS];
    pthread_attr_t attr;
    
    /*get the default attributes */
    pthread_attr_init(&attr);
    /* create pthreads */
    for ( i = 0; i < M; i++ ) {
        for ( j = 0; j < N; j++ ) {
            struct v *data=(struct v*) malloc(sizeof(struct v));
            data->i = i;
            data->j = j;
            pthread_create( &tid[i*N+j], &attr, getOneElem, (void *)data);
        }
    }
    /* now join on each thread */
    for ( i = 0; i < NUM_THREADS; i++ )
        pthread_join( tid[i], NULL);
    for (i = 0; i < M; i++) {
        printf("|");
        for (j = 0; j < N; j++) {
            printf("%d ", C[i][j]);
        }
        printf("|\n");
    }
    return 0;
}
