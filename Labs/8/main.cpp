/**
I checked out
https://en.wikipedia.org/wiki/MESI_protocol
for more details (notice slightly different notations).
**/

#include <bits/stdc++.h>
using namespace std;

int main()
{
    ifstream in("input.txt");
    ofstream out("output.txt");

    int numProcessors;
    int numOperations;
    in >> numProcessors >> numOperations;
    char *states = new char[numProcessors];
    memset(states, 'I', numProcessors);
    int proc;
    char op;
    string busOp;

    out << "t0\tinitial\t";
    for (int p = 0; p < numProcessors; ++p) {
        out << "I\t";
    }
    out << "-       -\n";

    for (int i = 0; i < numOperations; ++i) {
        in >> proc >> op;
        proc--;
        string dataSource;
        if (op == 'R') {
            busOp = "BusRd";
            if (states[proc] == 'I') {
                vector<int> procsWithData;
                for (int p = 0; p < numProcessors; ++p) {
                    if (p != proc && states[p] != 'I') {
                        procsWithData.push_back(p);
                        states[p] = 'S';
                        dataSource += "/" + to_string(p + 1);
                    }
                }
                if (!procsWithData.empty()) {
                    states[proc] = 'S';
                    dataSource[0] = 'C';
                }
                else {
                    states[proc] = 'E';
                    dataSource = "Mem";
                }
            }
            else {
                busOp = "-";
                dataSource = "-";
            }
        }
        else if (op == 'W') {
            dataSource = "-";
            if (states[proc] == 'I') {
                busOp = "BusRdX";
                states[proc] = 'M';
                for (int p = 0; p < numProcessors; ++p) {
                    if (p != proc) {
                        states[p] = 'I';
                    }
                }
            }
            else if (states[proc] == 'S') {
                busOp = "BusUpgr";
                states[proc] = 'M';
                for (int p = 0; p < numProcessors; ++p) {
                    if (p != proc) {
                        states[p] = 'I';
                    }
                }
            }
            else {
                busOp = "-";
            }
        }
        out << 't' << (i + 1) << "\tP" << (proc + 1) << (op == 'R' ? "Rd" : "Wr") << '\t';
        for (int p = 0; p < numProcessors; ++p) {
            out << states[p] << '\t';
        }
        out << busOp;
        for (int j = 0; j < (int)(8-busOp.length()); ++j) {
            out << ' ';
        }
        out << dataSource << '\n';
    }

    in.close();
    out.close();
    delete[] states;
    return 0;
}
