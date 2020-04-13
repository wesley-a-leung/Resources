#pragma once
#include <bits/stdc++.h>
using namespace std;

// Two players alternate taking coins from either end of a row. If both players play optimally, 
// what is the value of player 1's total minus player 2's total?
// The standard dp recurrence is dp[l][r] = l > r ? 0 : max(A[l] - dp[l + 1][r], A[r] - dp[l][r - 1])
// This implementation is generalized to support any number of rows, possibly with a fixed endpoint on the right
// The value of a single player can be recovered by solving the system of equations: P1 + P2 = A, P1 - P2 = B
// Time Complexity:
//   single row: O(N)
//   multiple rows: O(N log N)
// Memory Complexity: O(N)
template <class T> struct Termity {
    int N; vector<vector<T>> A; vector<bool> isFixed; vector<T> B;
    void addRow(bool fixed) { A.emplace_back(); isFixed.push_back(fixed); }
    void addValue(int row, T val) { N++; A[row].emplace_back(val); }
    void clear() { N = 0; A.clear(); isFixed.clear(); B.clear(); }
    T solve() {
        T ret = 0; int mult = N % 2 == 0 ? 1 : -1; B.reserve(N);
        for (int i = 0; i < int(A.size()); i++) {
            int front = int(B.size());
            for (auto &&a : A[i]) { // fusion principle
                B.push_back(a);
                while (int(B.size()) - front >= 3 && B[B.size() - 3] <= B[B.size() - 2] && B[B.size() - 2] >= B[B.size() - 1]) {
                    B[B.size() - 3] -= B[B.size() - 2] - B[B.size() - 1]; B.pop_back(); B.pop_back();
                }
            }
            if (isFixed[i]) { // fruitless move principle
                while (int(B.size()) - front >= 2 && B[B.size() - 2] <= B[B.size() - 1]) {
                    ret += mult * (B[B.size() - 2] - B[B.size() - 1]); B.pop_back(); B.pop_back();
                }
            }
        }
        if (int(A.size()) == 1) { // greedy principle
            for (int l = 0, r = int(B.size()) - 1, m = 1; l <= r; m *= -1) {
                if (B[l] >= B[r]) ret += m * B[l++];
                else ret += m * B[r--];
            }
        } else {
            sort(B.begin(), B.end(), greater<T>());
            for (int i = 0, m = 1; i < int(B.size()); i++, m *= -1) ret += m * B[i];
        }
        return ret;
    }
};
