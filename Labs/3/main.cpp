#include <bits/stdc++.h>
using namespace std;

unsigned int shuffle(unsigned int i, unsigned int N) {
    return (2 * i + 2 * i / N) % N;
}

bool sameBit(unsigned int src, unsigned int dest, unsigned int pos) {
    return ((src & (1 << pos)) == (dest & (1 << pos)));
}

int main() {
    /* build Omega network */
    unsigned int k, N, value;
    cin >> k;
    N = 1 << k;
    vector< vector<unsigned int> > entries(N);
    for (unsigned int i = 0; i < N; ++i) {
        value = i;
        for (unsigned int j = 0; j < k; ++j) {
            value = shuffle(value, N);
            entries[i].push_back(value);
        }
    }
    /* print Omega network */
    cout << "The Omega network:\n";
    for (unsigned int i = 0; i < N; ++i) {
        cout << i << ' ';
        for (unsigned int j = 0; j < k; ++j) {
            cout << entries[i][j] << ' ';
        }
        cout << '\n';
    }
    /* solve queries */
    unsigned int m, src, dest;
    cin >> m;
    for (unsigned int q = 0; q < m; ++q) {
        cin >> src >> dest;
        value = src;
        cout << value;
        for (unsigned int i = 0; i < k; ++i) {
            value = shuffle(value, N);
            if (sameBit(src, dest, k-i-1)) {
                cout << " -> " << value << " (block " << value/2 << " - direct)";
            }
            else {
                if (value % 2) {
                    value--;
                }
                else {
                    value++;
                }
                cout << " -> " << value << " (block " << value/2 << " - indirect)";
            }
        }
        cout << '\n';
    }
    return 0;
}
