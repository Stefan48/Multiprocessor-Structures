#include <bits/stdc++.h>
#include "helpers.h"
#include "pthread.h"

struct position
{
    int i, j, k;
};

void *thread_func(void *arg)
{
    position p = *(position*)arg;
    int prod = a[p.i][p.k] * b[p.k][p.j];
    __atomic_fetch_add(&c[p.i][p.j], prod, __ATOMIC_RELAXED);
    return NULL;
}

void execute()
{
    int ret;
    pthread_t ***threads = new pthread_t**[n];
    position ***positions = new position**[n];
    if (threads == NULL || positions == NULL)
    {
        exit(12);
    }
    for (int i = 0; i < n; ++i)
    {
        threads[i] = new pthread_t*[n];
        positions[i] = new position*[n];
        if (threads[i] == NULL || positions[i] == NULL)
        {
            exit(12);
        }
        for (int j = 0; j < n; ++j)
        {
            threads[i][j] = new pthread_t[n];
            positions[i][j] = new position[n];
            if (threads[i][j] == NULL || positions[i][j] == NULL)
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
            delete[] threads[i][j];
            delete[] positions[i][j];
        }
        delete[] threads[i];
        delete[] positions[i];
    }
    delete[] threads;
    delete[] positions;
}
