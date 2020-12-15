#pragma once
#include <bits/stdc++.h>
using namespace std;

// Trie Node backed by a map
// Template Arguments:
//   _T: the type of the element in the string/array for the Trie
// Fields:
//   T: the data type of each element in the string/array
//   link: a link to the parent in the trie
// Functions:
//   hasEdge(a): returns whether there exists an edge to an adjacent node
//     with the element a
//   getEdge(a): returns the index of the other node on the
//     incident edge with the element a
//   setEdge(a, n): sets the other node on the incident edge
//     with the element a to n
// Time Complexity:
//   constructor: O(1)
//   hasEdge, getEdge, setEdge: O(log E) where E is the number of edges
//     incident to this node
// Memory Complexity: O(E) where E is the number of edges incident to this node
// Tested:
//   https://csacademy.com/contest/round-77/task/expected-lcp/
template <class _T> struct TrieMapNode {
  using T = _T; int link; map<T, int> to;
  TrieMapNode() : link(-1) {}
  bool hasEdge(const T &a) const { return to.count(a); }
  int getEdge(const T &a) const { return to.at(a); }
  void setEdge(const T &a, int n) { to[a] = n; }
};

// Trie Node backed by an array
// Template Arguments:
//   _T: the type of the element in the string/array for the Trie
//   ALPHABET_SIZE: the size of the alphabet
//   OFFSET: the offset for the start of the alphabet
// Fields:
//   T: the data type of each element in the string/array
//   link: a link to the parent in the trie
// Functions:
//   hasEdge(a): returns whether there exists an edge to an adjacent node
//     with the element a
//   getEdge(a): returns the index of the other node on the
//     incident edge with the element a
//   setEdge(a, n): sets the other node on the incident edge
//     with the element a to n
// Time Complexity:
//   constructor: O(ALPHABET_SIZE)
//   hasEdge, getEdge, setEdge: O(1)
// Memory Complexity: O(ALPHABET_SIZE)
// Tested:
//   https://csacademy.com/contest/round-77/task/expected-lcp/
template <class _T, const int ALPHABET_SIZE, const int OFFSET>
struct TrieArrayNode {
  using T = _T; int link; array<int, ALPHABET_SIZE> to;
  TrieArrayNode() : link(-1) { to.fill(-1); }
  bool hasEdge(const T &a) const { return to[a - OFFSET] != -1; }
  int getEdge(const T &a) const { return to[a - OFFSET]; }
  void setEdge(const T &a, int n) { to[a - OFFSET] = n; }
};

// Trie with the root node at 0
// Template Arguments:
//   Node: a node class
//     Required Fields:
//       T: the data type of each element in the string/array
//       link: a link to the parent in the trie
//     Required Functions:
//       constructor(): initializes the node with a with no links or edges
//       hasEdge(a): returns whether there exists an edge to an adjacent node
//         with the element a
//       getEdge(a): returns the index of the other node on the
//         incident edge with the element a
//       setEdge(a, n): sets the other node on the incident edge
//         with the element a to n
// Fields:
//   TR: the vector of all nodes in the automaton
//   last: the last node representing the current string
// Functions:
//   add(a): adds the element a to the trie
//   terminate(): terminates the current word, returning to the root (which
//     allows for another word to be added with add(a))
// Time Complexity:
//   constructor: time complexity of node constructor
//   add: O(1) + time complexity of node constructor
//        + time complexity of getEdge in Node
//   terminate: O(1)
// Memory Complexity: O(N) * memory complexity of node, after N calls to add
// Tested:
//   https://csacademy.com/contest/round-77/task/expected-lcp/
template <class Node> struct Trie {
  using T = typename Node::T; vector<Node> TR; int last;
  Trie() : TR(1, Node()), last(0) {}
  void add(const T &a) {
    if (TR[last].hasEdge(a)) last = TR[last].getEdge(a);
    else {
      int u = TR.size(); TR.emplace_back();
      TR[TR[u].link = last].setEdge(a, u); last = u;
    }
  }
  void terminate() { last = 0; }
};
