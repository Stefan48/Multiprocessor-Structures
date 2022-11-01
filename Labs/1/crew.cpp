#include <bits/stdc++.h>
#include "helpers.h"
#include "pthread.h"

struct position
{
    int i, j, k;
};

int ***v;
pthread_barrier_t barrier;

void *thread_func(void *arg)
{
    position p = *(position*)arg;
    v[p.i][p.j][p.k] = a[p.i][p.k] * b[p.k][p.j];
    pthread_barrier_wait(&barrier);
    float stop = log2f((float)n);
    int power = 1;
    for (int m = 0; (float)m < stop; ++m)
    {
        if (p.k % (2 * power) == 0 && p.k + power < n)
        {
            v[p.i][p.j][p.k] += v[p.i][p.j][p.k + power];
        }
        power <<= 1;
        pthread_barrier_wait(&barrier);
    }
    if (p.k == 0)
    {
        c[p.i][p.j] = v[p.i][p.j][0];
    }
    return NULL;
}

void execute()
{
    int ret;
    v = new int**[n];
    pthread_t ***threads = new pthread_t**[n];
    position ***positions = new position**[n];
    if (v == NULL || threads == NULL || positions == NULL)
    {
        exit(12);
    }
    ret = pthread_barrier_init(&barrier, NULL, n * n * n);
    if (ret)
    {
        exit(1);
    }
    for (int i = 0; i < n; ++i)
    {
        v[i] = new int*[n];
        threads[i] = new pthread_t*[n];
        positions[i] = new position*[n];
        if (v[i] == NULL || threads[i] == NULL || positions[i] == NULL)
        {
            exit(12);
        }
        for (int j = 0; j < n; ++j)
        {
            v[i][j] = new int[n]();
            threads[i][j] = new pthread_t[n];
            positions[i][j] = new position[n];
            if (v[i][j] == NULL || threads[i][j] == NULL || positions[i][j] == NULL)
            {
                exit(12);
            }
        }
    }
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            for (int k = 0; k < n; ++k)
            {
                positions[i][j][k].i = i;
                positions[i][j][k].j = j;
                positions[i][j][k].k = k;
                ret = pthread_create(&threads[i][j][k], NULL, thread_func, (void*)&positions[i][j][k]);
                if (ret)
                {
                    // creating threads might fail with error code EAGAIN
                    // if there aren't enough available resources
                    exit(1);
                }
            }
        }
    }
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            for (int k = 0; k < n; ++k)
            {
                ret = pthread_join(threads[i][j][k], NULL);
                if (ret)
                {
                    exit(1);
                }
            }
        }
    }
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            delete[] v[i][j];
            delete[] threads[i][j];
            delete[] positions[i][j];
        }
        delete[] v[i];
        delete[] threads[i];
        delete[] positions[i];
    }
    delete[] v;
    delete[] threads;
    delete[] positions;
    ret = pthread_barrier_destroy(&barrier);
    if (ret)
    {
        exit(1);
    }
}
