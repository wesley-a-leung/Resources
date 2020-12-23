#pragma once
#include <bits/stdc++.h>
using namespace std;

// Palindromic Tree Node backed by a map
// Template Arguments:
//   _T: the type of the element in the string/array for the Palindromic Tree
// Constructor Arguments:
//   len: the length of the longest palindromic substring represented
//     by this node
// Fields:
//   T: the data type of each element in the string/array
//   len: the length of the longest palindromic substring this
//     node represents
//   link: the index of the suffix link of this node (the node with the
//     longest palindromic suffix of this node)
//   qlink: the index of the quick link of this node (the node with the
//     longest palindromic suffix which has a different preceding character
//     as the link node)
// Functions:
//   getEdge(a): returns the index of the other node on the
//     incident edge with the element a or 1 if no such edge exists
//   setEdge(a, n): sets the other node on the incident edge
//     with the element a to n
// Time Complexity:
//   constructor: O(1)
//   getEdge, setEdge: O(log E) where E is the number of edges
//     incident to this node
// Memory Complexity: O(E) where E is the number of edges incident to this node
// Tested:
//   https://dmoj.ca/problem/apio14p1
template <class _T> struct PalTreeMapNode {
  using T = _T; int len, link, qlink; map<T, int> to;
  PalTreeMapNode(int len) : len(len), link(1), qlink(1) {}
  int getEdge(const T &a) const {
    auto it = to.find(a); return it == to.end() ? 1 : it->second;
  }
  void setEdge(const T &a, int n) { to[a] = n; }
};

// Palindromic Tree Node  backed by an array
// Template Arguments:
//   _T: the type of the element in the string/array for the Palindromic Tree
//   ALPHABET_SIZE: the size of the alphabet
//   OFFSET: the offset for the start of the alphabet
// Constructor Arguments:
//   len: the length of the longest palindromic substring represented
//     by this node
// Fields:
//   T: the data type of each element in the string/array
//   len: the length of the longest palindromic substring this
//     node represents
//   link: the index of the suffix link of this node (the node with the
//     longest palindromic suffix of this node)
//   qlink: the index of the quick link of this node (the node with the
//     longest palindromic suffix which has a different preceding character
//     as the link node)
// Functions:
//   getEdge(a): returns the index of the other node on the
//     incident edge with the element a or 1 if no such edge exists
//   setEdge(a, n): sets the other node on the incident edge
//     with the element a to n
// Time Complexity:
//   constructor: O(ALPHABET_SIZE)
//   getEdge, setEdge: O(1)
// Memory Complexity: O(ALPHABET_SIZE)
// Tested:
//   https://dmoj.ca/problem/mmcc15p3
//   https://dmoj.ca/problem/apio14p1
template <class _T, const int ALPHABET_SIZE, const _T OFFSET>
struct PalTreeArrayNode {
  using T = _T; int len, link, qlink; array<int, ALPHABET_SIZE> to;
  PalTreeArrayNode(int len) : len(len), link(1), qlink(1) { to.fill(1); }
  int getEdge(const T &a) const { return to[a - OFFSET]; }
  void setEdge(const T &a, int n) { to[a - OFFSET] = n; }
};

// Palindromic Tree with two roots at 0 (with length -1) and 1 (with length 0)
// Template Arguments:
//   Node: a node class
//     Required Fields:
//       T: the data type of each element in the string/array
//       len: the length of the longest palindromic substring this
//         node represents
//       link: the index of the suffix link of this node (the node with the
//         longest palindromic suffix of this node)
//       qlink: the index of the quick link of this node (the node with the
//         longest palindromic suffix which has a different preceding character
//     as the link node)
//     Required Functions:
//       constructor(len): initializes the node with a length of len with
//         link, qlink, and edges pointing to 1 by default
//       getEdge(a): returns the index of the other node on the
//         incident edge with the element a
//       setEdge(a, n): sets the other node on the incident edge
//         with the element a to n
// Constructor Arguments:
//   def: the default value of type T, cannot be in the string/array
// Fields:
//   S: the current string with the default character at the front
//   TR: the vector of all nodes in the tree
//   last: a vector of integers with the last node after each addition
// Functions:
//   add(a): adds the element a to the tree
//   undo(): undoes the last added element
// In practice, has a small constant
// Time Complexity:
//   constructor: time complexity of node constructor
//   add: O(log N) + time complexity of node constructor
//        + time complexity of getEdge in Node
//   undo: time complexity of setEdge in Node
// Memory Complexity: O(N) * memory complexity of node, after N calls to add
// Tested:
//   https://dmoj.ca/problem/mmcc15p3
//   https://dmoj.ca/problem/apio14p1
template <class Node> struct PalindromicTree {
  using T = typename Node::T;
  vector<T> S; vector<Node> TR; vector<int> last, modified;
  PalindromicTree(const T &def)
      : S(1, def), TR(vector<Node>{Node(-1), Node(0)}), last(1, 1) {
    TR[1].link = TR[1].qlink = 0;
  }
  int getLink(int x, int i) {
    while (S[i - 1 - TR[x].len] != S[i])
      x = S[i - 1 - TR[TR[x].link].len] == S[i] ? TR[x].link : TR[x].qlink;
    return x;
  }
  void add(const T &a) {
    int i = S.size(); S.push_back(a); int p = getLink(last.back(), i);
    modified.push_back(-1); if (TR[p].getEdge(a) == 1) {
      int u = TR.size(); TR.emplace_back(TR[p].len + 2);
      TR[u].link = TR[getLink(TR[p].link, i)].getEdge(a);
      T b = S[i - TR[TR[u].link].len], c = S[i - TR[TR[TR[u].link].link].len];
      TR[u].qlink = b == c ? TR[TR[u].link].qlink : TR[u].link;
      TR[p].setEdge(a, u); modified.back() = p;
    }
    last.push_back(TR[p].getEdge(a));
  }
  void undo() {
    if (modified.back() != -1) {
      TR[modified.back()].setEdge(S.back(), 1); TR.pop_back();
    }
    S.pop_back(); last.pop_back(); modified.pop_back();
  }
};
