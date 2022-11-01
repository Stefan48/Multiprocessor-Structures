#include <bits/stdc++.h>
using namespace std;

struct Block
{
    unsigned int out0, out1;
    bool isDirect;
};

unsigned int rotr(unsigned int x, unsigned int bits) {
    return (x >> 1) + (x & 1) * (1 << (bits - 1));
}

vector<Block> addOffset(vector<Block> row, unsigned int offset) {
    for (unsigned int i = 0; i < row.size(); ++i) {
        row[i].out0 += offset;
        row[i].out1 += offset;
    }
    return row;
}

vector< vector<Block> > buildBenesNetwork(unsigned int k) {
    if (k == 1) {
        vector<Block> row;
        Block block;
        block.out0 = 0;
        block.out1 = 1;
        row.push_back(block);
        vector< vector<Block> > network;
        network.push_back(row);
        return network;
    }
    unsigned int blocksPerLevel = 1 << (k - 1);
    unsigned int levels = 2 * k - 1;
    vector< vector<Block> > network;
    vector< vector<Block> > subNetwork = buildBenesNetwork(k - 1);
    for (unsigned int i = 0; i < blocksPerLevel; ++i) {
        vector<Block> row;
        Block first, last;
        last.out0 = 2 * i;
        last.out1 = 2 * i + 1;
        row.push_back(first);
        vector<Block> subRow = i >= blocksPerLevel/2 ? addOffset(subNetwork[i%subNetwork.size()], 1<<(k-1)) : subNetwork[i%subNetwork.size()];
        row.insert(row.end(), subRow.begin(), subRow.end());
        row.push_back(last);
        network.push_back(row);
    }
    for (unsigned int i = 0; i < blocksPerLevel; ++i) {
        unsigned int out0 = rotr(2*i, k);
        unsigned int out1 = rotr(2*i+1, k);
        network[i][0].out0 = out0;
        network[i][0].out1 = out1;
        if (out0 % 2) {
            network[out0/2][levels-2].out1 = 2*i;
        }
        else {
            network[out0/2][levels-2].out0 = 2*i;
        }
        if (out1 % 2) {
            network[out1/2][levels-2].out1 = 2*i+1;
        }
        else {
            network[out1/2][levels-2].out0 = 2*i+1;
        }
    }
    return network;
}

bool isValid(vector< vector<Block> > &network, unsigned int levels, vector<unsigned int> dest) {
    unsigned int N = dest.size();
    for (unsigned int i = 0; i < N; ++i) {
        unsigned int val = i;
        for (unsigned int level = 0; level < levels; ++level) {
            unsigned int block = val / 2;
            if ((val % 2 == 0 && network[block][level].isDirect) || (val % 2 && !network[block][level].isDirect)) {
                val = network[block][level].out0;
            }
            else {
                val = network[block][level].out1;
            }
        }
        if (val != dest[i]) {
            return false;
        }
    }
    return true;
}

bool bkt(vector< vector<Block> > &network, unsigned int block, unsigned int level, unsigned int blocksPerLevel, unsigned int levels, vector<unsigned int> dest) {
    network[block][level].isDirect = true;
    if (block == blocksPerLevel-1 && level == levels-1) {
        if (isValid(network, levels, dest)) {
            return true;
        }
        network[block][level].isDirect = false;
        return isValid(network, levels, dest);
    }
    unsigned int nextBlock, nextLevel;
    if (block == blocksPerLevel - 1) {
        nextBlock = 0;
        nextLevel = level + 1;
    }
    else {
        nextBlock = block + 1;
        nextLevel = level;
    }
    if (bkt(network, nextBlock, nextLevel, blocksPerLevel, levels, dest)) {
        return true;
    }
    network[block][level].isDirect = false;
    return bkt(network, nextBlock, nextLevel, blocksPerLevel, levels, dest);
}

int main()
{
    /* build Benes network */
    unsigned int k, N, blocksPerLevel, levels;
    cin >> k;
    N = 1 << k;
    blocksPerLevel = N / 2;
    levels = 2 * k - 1;
    vector< vector<Block> > network = buildBenesNetwork(k);
    cout << "The Benes network is:\n";
    for (unsigned int i = 0; i < blocksPerLevel; ++i) {
        for (unsigned int j = 0; j < levels; ++j) {
            cout << '(' << network[i][j].out0 << ' ' << network[i][j].out1 << ")\t";
        }
        cout << '\n';
    }
    /* determine connection types */
    vector<unsigned int> dest(N);
    for (unsigned int i = 0; i < N; ++i) {
        cin >> dest[i];
    }
    if (bkt(network, 0, 0, blocksPerLevel, levels, dest)) {
        cout << "The connection types are:\n";
        for (unsigned int i = 0; i < blocksPerLevel; ++i) {
            for (unsigned int j = 0; j < levels; ++j) {
                if (network[i][j].isDirect) {
                    cout << "direct ";
                }
                else cout << "indirect ";
            }
            cout << '\n';
        }
    }
    else {
        cout << "Cannot determine connections\n";
    }
    return 0;
}
