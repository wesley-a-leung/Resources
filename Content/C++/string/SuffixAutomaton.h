#pragma once
#include <bits/stdc++.h>
using namespace std;

// Suffix Automaton Node backed by a map
// Template Arguments:
//   _T: the type of the element in the string/array for the Suffix Automaton
// Constructor Arguments:
//   len: the length of the longest substring represented by this node
// Fields:
//   T: the data type of each element in the string/array
//   len: the length of the longest substring this node represents
//   link: the index of the suffix link of this node (the node with the
//     longest suffix of this node)
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
//   https://www.spoj.com/problems/LCS/
//   https://open.kattis.com/problems/stringmultimatching
template <class _T> struct SAMMapNode {
  using T = _T; int len, link; map<T, int> to;
  SAMMapNode(int len) : len(len), link(-1) {}
  bool hasEdge(const T &a) const { return to.count(a); }
  int getEdge(const T &a) const { return to.at(a); }
  void setEdge(const T &a, int n) { to[a] = n; }
};

// Suffix Automaton Node backed by an array
// Template Arguments:
//   _T: the type of the element in the string/array for the Suffix Automaton
//   ALPHABET_SIZE: the size of the alphabet
//   OFFSET: the offset for the start of the alphabet
// Constructor Arguments:
//   len: the length of the longest substring represented by this node
// Fields:
//   T: the data type of each element in the string/array
//   len: the length of the longest substring this node represents
//   link: the index of the suffix link of this node (the node with the
//     longest suffix of this node)
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
//   https://www.spoj.com/problems/LCS/
//   https://dmoj.ca/problem/coci14c5p6
//   https://dmoj.ca/problem/coci11c5p6
template <class _T, const int ALPHABET_SIZE, const _T OFFSET>
struct SAMArrayNode {
  using T = _T; int len, link; array<int, ALPHABET_SIZE> to;
  SAMArrayNode(int len) : len(len), link(-1) { to.fill(-1); }
  bool hasEdge(const T &a) const { return to[a - OFFSET] != -1; }
  int getEdge(const T &a) const { return to[a - OFFSET]; }
  void setEdge(const T &a, int n) { to[a - OFFSET] = n; }
};

// Suffix Automaton with the root node at 0
// Each distinct path from the root is a substring of the set of words inserted
// Template Arguments:
//   Node: a node class
//     Required Fields:
//       T: the data type of each element in the string/array
//       len: the length of the longest substring this node represents
//       link: the index of the suffix link of this node (the node with the
//         longest suffix of this node)
//     Required Functions:
//       constructor(len): initializes the node with a length of len with no
//         links or edges
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
//   add(a): adds the element a to the automaton
//   terminate(): terminates the current word, returning to the root (which
//     allows for another word to be added with add(a))
// In practice, has a moderate constant
// Time Complexity:
//   constructor: time complexity of node constructor
//   add: O(1) amortized + time complexity of node constructor
//        + time complexity of getEdge in Node
//   terminate: O(1)
// Memory Complexity: O(N) * memory complexity of node, after N calls to add
// Tested:
//   https://www.spoj.com/problems/LCS/
//   https://dmoj.ca/problem/coci14c5p6
//   https://open.kattis.com/problems/stringmultimatching
//   https://dmoj.ca/problem/coci11c5p6
template <class Node> struct SuffixAutomaton {
  using T = typename Node::T; vector<Node> TR; int last;
  SuffixAutomaton() : TR(1, Node(0)), last(0) {}
  void add(const T &a) {
    int u = -1; if (!TR[last].hasEdge(a)) {
      u = TR.size(); TR.emplace_back(TR[last].len + 1);
      for (; last != -1 && !TR[last].hasEdge(a); last = TR[last].link)
        TR[last].setEdge(a, u);
      if (last == -1) { TR[last = u].link = 0; return; }
    }
    int p = TR[last].getEdge(a); if (TR[p].len == TR[last].len + 1) {
      (u == -1 ? last : TR[last = u].link) = p; return;
    }
    int q = TR.size(); TR.push_back(TR[p]); TR[q].len = TR[last].len + 1;
    TR[p].link = q; if (u != -1) TR[u].link = q;
    while (last != -1 && TR[last].hasEdge(a) && TR[last].getEdge(a) == p) {
      TR[last].setEdge(a, q); last = TR[last].link;
    }
    last = u == -1 ? q : u;
  }
  void terminate() { last = 0; }
};
