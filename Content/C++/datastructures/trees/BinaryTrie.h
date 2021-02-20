#pragma once
#include <bits/stdc++.h>
using namespace std;

// Binary Trie
// Template Arguments:
//   T: the type of the values being inserted and queried
// Constructor Arguments:
//   MX: the maximum value of v for any function call
// Functions:
//   empty(): returns whether the trie is empty or not
//   insert(v): inserts the value v into the trie if it doesn't already exist,
//     returns whether it was inserted or not
//   contains(v): returns whether the trie contains the value v
//   erase(v): erases the value v into the trie if it exists,
//     returns whether it was erased or not
//   minXor(v): returns the minimum value of v xor any value in the trie
//   maxXor(v): returns the maximum value of v xor any value in the trie
// Time Complexity:
//   constructor: O(1)
//   insert, contains, erase, minXor, maxXor: O(log MX)
// Memory Complexity: O(Q log MX) for Q insertions
// Tested:
//   Fuzz Tested
//   https://judge.yosupo.jp/problem/set_xor_min
template <class T> struct BinaryTrie {
  struct Node : public array<int, 2> {
    Node() : array<int, 2>() { fill(-1); }
  };
  vector<Node> TR; vector<int> deleted; int root, lg;
  int makeNode() {
    if (deleted.empty()) { TR.emplace_back(); return int(TR.size()) - 1; }
    int i = deleted.back(); deleted.pop_back(); TR[i] = Node(); return i;
  }
  BinaryTrie(T MX)
      : root(-1), lg(MX == 0 ? 0 : __lg(MX)) {}
  bool empty() { return root == -1; }
  bool insert(T v) {
    bool ret = false; if (root == -1) root = makeNode();
    for (int cur = root, i = lg; i >= 0; i--) {
      bool b = (v >> i) & 1;
      if (TR[cur][b] == -1) { int n = makeNode(); ret = true; TR[cur][b] = n; }
      cur = TR[cur][b];
    }
    return ret;
  }
  bool contains(T v) {
    if (root == -1) return false;
    for (int cur = root, i = lg; i >= 0; i--) {
      bool b = (v >> i) & 1; if (TR[cur][b] == -1) return false;
      cur = TR[cur][b];
    }
    return true;
  }
  bool erase(T v) {
    bool ret = false;
    function<int(int, int)> rec = [&] (int cur, int i) {
      if (i == -1) { ret = true; deleted.push_back(cur); return -1; }
      bool b = (v >> i) & 1; if (TR[cur][b] == -1) return cur;
      TR[cur][b] = rec(TR[cur][b], i - 1);
      if (TR[cur][0] == -1 && TR[cur][1] == -1) {
        ret = true; deleted.push_back(cur); return -1;
      } else return cur;
    };
    if (root != -1) root = rec(root, lg);
    return ret;
  }
  T minXor(T v) {
    T ret = 0; for (int cur = root, i = lg; i >= 0; i--) {
      bool b = (v >> i) & 1; if (TR[cur][b] == -1) {
        ret ^= T(1) << i; cur = TR[cur][b ^ 1];
      } else cur = TR[cur][b];
    }
    return ret;
  }
  T maxXor(T v) {
    T ret = 0; for (int cur = root, i = lg; i >= 0; i--) {
      bool b = (v >> i) & 1; if (TR[cur][b ^ 1] == -1) cur = TR[cur][b];
      else { ret ^= T(1) << i; cur = TR[cur][b ^ 1]; }
    }
    return ret;
  }
};
