#pragma once
#include <bits/stdc++.h>
using namespace std;

struct Trie {
    struct Node { unordered_map<char, Node*> child; int cnt = 0, prefixCnt = 0; };
    void insert(Node *root, const string &s) {
        Node *cur = root;
        for (char c : s) {
            if (cur->child.count(c)) cur = cur->child[c];
            else cur = cur->child[c] = new Node();
            cur->prefixCnt++;
        }
        cur->cnt++;
    }
    int count(Node *root, const string &s) {
        Node *cur = root;
        for (char c : s) {
            if (cur->child.count(c)) cur = cur->child[c];
            else return 0;
        }
        return cur->cnt;
    }
    int prefixCount(Node *root, const string &s) {
        Node *cur = root;
        for (char c : s) {
            if (cur->child.count(c)) cur = cur->child[c];
            else return 0;
        }
        return cur->prefixCnt;
    }
    Node *root = new Node();
    Trie() {}
    void insert(const string &s) { insert(root, s); }
    bool contains(const string &s) { return count(root, s) >= 1; }
    bool hasPrefix(const string &s) { return prefixCount(root, s) >= 1; }
    int count(const string &s) { return count(root, s); }
    int prefixCount(const string &s) { return prefixCount(root, s); }
};
