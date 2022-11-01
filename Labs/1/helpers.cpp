#include <bits/stdc++.h>
#include "helpers.h"
using namespace std;

int n;
int **a, **b, **c;

void init()
{
    cin >> n;
    a = new int*[n];
    b = new int*[n];
    c = new int*[n];
    if (a == NULL || b == NULL || c == NULL)
    {
        exit(12);
    }
    for (int i = 0; i < n; ++i)
    {
        a[i] = new int[n]();
        b[i] = new int[n]();
        c[i] = new int[n]();
        if (a[i] == NULL || b[i] == NULL || c[i] == NULL)
        {
            exit(12);
        }
        for (int j = 0; j < n; ++j)
        {
            cin >> a[i][j];
        }
    }
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            cin >> b[i][j];
        }
    }
}

void print()
{
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            cout << c[i][j] << ' ';
        }
        cout << '\n';
    }
}

void clear()
{
    for (int i = 0; i < n; ++i)
    {
        delete[] a[i];
        delete[] b[i];
        delete[] c[i]; 
    }
    delete[] a;
    delete[] b;
    delete[] c;
}
