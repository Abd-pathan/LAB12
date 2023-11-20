#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct Pthread_Sum {
    double *a;
    double sum;
    int N;
    int size;
    int tid;
} Pthread;

void *myFunc(void *arg) {
    int mys, mye, myN, i;
    Pthread *pt = (Pthread *)arg;
    myN = pt->N / pt->size;
    mys = pt->tid * myN;
    mye = mys + myN;
    if (pt->tid == (pt->size - 1))
        mye = pt->N;
    double mysum = 0.0;
    for (i = mys; i < mye; i++)
        mysum += pt->a[i];
    pthread_mutex_lock(&mutex);
    pt->sum += mysum;
    pthread_mutex_unlock(&mutex);
    return (NULL);
}

int main(int argc, char **argv) {

    double *a = NULL, sum = 0.0;
    int N, size = 0;
    long i;
    pthread_t *tid;
    Pthread *pt = 0;

    if (argc != 3) {
        printf("Usage: %s <# of elements> <# of threads>\n", argv[0]);
        exit(-1);
    }

    N = atoi(argv[1]);
    size = atoi(argv[2]);

    tid = (pthread_t *)malloc(sizeof(pthread_t) * size);
    a = (double *)malloc(sizeof(double) * N);
    pt = (Pthread *)malloc(sizeof(Pthread) * size);

    for (i = 0; i < N; i++)
        a[i] = (double)(i + 1);

    for (i = 0; i < size; i++) {
        pt[i].a = a;
        pt[i].sum = 0.0;
        pt[i].N = N;
        pt[i].size = size;
        pt[i].tid = i;

        pthread_create(&tid[i], NULL, myFunc, (void *)&pt[i]);
    }

    for (i = 0; i < size; i++) {
        pthread_join(tid[i], NULL);
        sum += pt[i].sum;
    }

    printf("The total is %g, it should be equal to %g\n", sum, ((double)N * (N + 1)) / 2);

    free(tid);
    free(a);
    free(pt);

    return 0;
}