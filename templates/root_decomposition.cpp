#include <bits/stdc++.h>
using namespace std;

struct SquareRootDecomposition {
    vector<int> arr;
    vector<int> blocks;
    SquareRootDecomposition(const vector<int>& input) {
        arr = input;
        int n = arr.size();
        int blockSize = sqrt(n);
        for (int i = 0; i < n; i += blockSize) {
            int sum = 0;
            for (int j = i; j < min(i + blockSize, n); ++j) {
                sum += arr[j];
            }
            blocks.push_back(sum);
        }
    }
    void update(int index, int value) {
        int blockSize = sqrt(arr.size());
        blocks[index / blockSize] += value - arr[index];
        arr[index] = value;
    }
    int query(int left, int right) {
        int blockSize = sqrt(arr.size());
        int sum = 0;
        int startBlock = left / blockSize;
        int endBlock = right / blockSize;
        if (startBlock == endBlock) {
            for (int i = left; i <= right; ++i) {
                sum += arr[i];
            }
        } else {
            for (int i = left; i < (startBlock + 1) * blockSize; ++i) {
                sum += arr[i];
            }
            for (int i = startBlock + 1; i < endBlock; ++i) {
                sum += blocks[i];
            }
            for (int i = endBlock * blockSize; i <= right; ++i) {
                sum += arr[i];
            }
        }
        return sum;
    }
};

struct LayeredRootDecomposition {
    vector<vector<int>> layers;
    int B;
    LayeredRootDecomposition(const vector<int>& input, int depth) {
        layers.push_back(input);
        int N = input.size();
        if (depth <= 1) B = 1;
        else B = (int)ceil(pow(N, 1.0 / (depth - 1)));
        for (int d = 1; d < depth; ++d) {
            const vector<int>& prev = layers.back();
            int n = prev.size();
            int blockSize = B;
            vector<int> newLayer;
            for (int i = 0; i < n; i += blockSize) {
                int sum = 0;
                for (int j = i; j < min(i + blockSize, n); ++j) {
                    sum += prev[j];
                }
                newLayer.push_back(sum);
            }
            layers.push_back(newLayer);
        }
    }

    int query(int left, int right) {
        int sum = 0;
        int L = layers.size();
        int N = layers[0].size();
        vector<int> power(L, 1);
        for (int d = 1; d < L; ++d) power[d] = power[d-1] * B;
        while (left <= right) {
            bool used = false;
            for (int d = L-1; d >= 0; --d) {
                int sz = power[d];
                int blockIdx = left / sz;
                int actual = min(sz, N - blockIdx * sz);
                if (left % sz == 0 && left + actual - 1 <= right) {
                    sum += layers[d][blockIdx];
                    left += actual;
                    used = true;
                    break;
                }
            }
            if (!used) {
                sum += layers[0][left];
                ++left;
            }
        }
        return sum;
    }

    int update(int index, int value) {
        int delta = value - layers[0][index];
        layers[0][index] = value;
        int L = layers.size();
        if (L <= 1) return 0;
        int idx = index;
        for (int d = 1; d < L; ++d) {
            idx = idx / B;
            layers[d][idx] += delta;
        }
        return 0;
    }
};