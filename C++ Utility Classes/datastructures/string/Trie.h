#ifndef DATASTRUCTURES_STRING_TRIE_H_
#define DATASTRUCTURES_STRING_TRIE_H_

#include <bits/stdc++.h>
using namespace std;

struct Trie {
private:
    struct Node {
        unordered_map<char, Node*> child;
        int cnt = 0, prefixCnt = 0;
    };

    void insert(Node *root, string &s) {
        Node *cur = root;
        for (char c : s) {
            if (cur->child.count(c)) cur = cur->child[c];
            else cur = cur->child[c] = new Node();
            cur->prefixCnt++;
        }
        cur->cnt++;
    }

    int count(Node *root, string &s) {
        Node *cur = root;
        for (char c : s) {
            if (cur->child.count(c)) cur = cur->child[c];
            else return 0;
        }
        return cur->cnt;
    }

    int prefixCount(Node *root, string &s) {
        Node *cur = root;
        for (char c : s) {
            if (cur->child.count(c)) cur = cur->child[c];
            else return 0;
        }
        return cur->prefixCnt;
    }

    Node *root = new Node();

public:
    Trie() {}

    void insert(string &s) {
        insert(root, s);
    }

    bool contains(string &s) {
        return count(root, s) >= 1;
    }

    bool hasPrefix(string &s) {
        return prefixCount(root, s) >= 1;
    }

    int count(string &s) {
        return count(root, s);
    }

    int prefixCount(string &s) {
        return prefixCount(root, s);
    }
};

#endif /* DATASTRUCTURES_STRING_TRIE_H_ */
