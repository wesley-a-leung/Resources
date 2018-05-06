#ifndef DATASTRUCTURES_GRAPH_QUERIES_ROOTEDLINKCUTTREE_H_
#define DATASTRUCTURES_GRAPH_QUERIES_ROOTEDLINKCUTTREE_H_

#include <bits/stdc++.h>
using namespace std;

struct RootedLinkCutTree {
private:
    static const int vdef = 0, qdef = 0;

    struct Node {
    public:
        int vertex, val, subtreeVal, size;
        Node *left = nullptr, *right = nullptr, *parent = nullptr;

        Node (int vertex, int val) : vertex(vertex), val(val), subtreeVal(val), size(1) {};
    };

    int V;
    vector<Node*> nodes;

    int apply(int a, int b) {
        return a + b;
    }

    int merge(int l, int r) {
        return l + r;
    }

    bool isRoot(Node *x) {
        return nullptr == x->parent || (x != x->parent->left && x != x->parent->right);
    }

    int getSize(Node *x) {
        return nullptr == x ? 0 : x->size;
    }

    int getSubtreeVal(Node *x) {
        return nullptr == x ? vdef : x->subtreeVal;
    }

    void update(Node *x) {
        x->subtreeVal = merge(merge(getSubtreeVal(x->left), x->val), getSubtreeVal(x->right));
        x->size = 1 + getSize(x->left) + getSize(x->right);
    }

    void connect(Node *child, Node *parent, bool hasChild, bool isLeft) {
        if (nullptr != child) child->parent = parent;
        if (hasChild) {
            if (isLeft) parent->left = child;
            else parent->right = child;
        }
    }

    void rotate(Node *x) {
        Node *p = x->parent;
        Node *g = p->parent;
        bool isRootP = isRoot(p);
        bool isLeft = (x == p->left);
        connect(isLeft ? x->right : x->left, p, true, isLeft);
        connect(p, x, true, !isLeft);
        connect(x, g, !isRootP, isRootP ? false : p == g->left);
        update(p);
    }

    void splay(Node *x) {
        while (!isRoot(x)) {
            Node *p = x->parent;
            Node *g = p->parent;
            if (!isRoot(p)) rotate((x == p->left) == (p == g->left) ? p : x);
            rotate(x);
        }
        update(x);
    }

    Node *expose(Node *x) {
        Node *last = nullptr;
        for (Node *y = x; nullptr != y; y = y->parent) {
            splay(y);
            y->left = last;
            last = y;
        }
        splay(x);
        return last;
    }

    Node *findRoot(Node *x) {
        expose(x);
        while (nullptr != x->right) x = x->right;
        splay(x);
        return x;
    }

    Node *lca(Node *x, Node *y) {
        if (findRoot(x) != findRoot(y)) return nullptr;
        expose(x);
        return expose(y);
    }

    bool link(Node *x, Node *y) {
        if (findRoot(x) == findRoot(y)) return false;
        expose(x);
        if (nullptr != x->right) throw runtime_error("x is not a root node");
        x->parent = y;
        return true;
    }

    void cutParent(Node *x) {
        expose(x);
        if (nullptr == x->right) throw runtime_error("x is a root node");
        x->right->parent = nullptr;
        x->right = nullptr;
    }

    bool modify(Node *x, int delta) {
        splay(x);
        x->val = apply(x->val, delta);
        update(x);
        return true;
    }

    int queryPathFromRoot(Node *x) {
        expose(x);
        return getSubtreeVal(x);
    }

    int queryPath(Node *from, Node *to) {
        if (findRoot(from) != findRoot(to)) return 0;
        Node *LCA = lca(from, to);
        return queryPathFromRoot(from) + queryPathFromRoot(to) - 2 * queryPathFromRoot(LCA) + LCA->val;
    }

public:
    RootedLinkCutTree(int V) : V(V), nodes(V) {
        for (int v = 0; v < V; v++) {
            nodes[v] = new Node(v, vdef);
        }
    }

    RootedLinkCutTree(int V, int *values) : V(V), nodes(V) {
        for (int v = 0; v < V; v++) {
            nodes[v] = new Node(v, values[v]);
        }
    }

    void addNode(int value) {
        nodes.push_back(new Node(V++, value));
    }

    bool link(int v, int w) {
        return link(nodes[v], nodes[w]);
    }

    void cutParent(int v) {
        cutParent(nodes[v]);
    }

    bool modify(int v, int delta) {
        return modify(nodes[v], delta);
    }

    int queryPathFromRoot(int v) {
        return queryPathFromRoot(nodes[v]);
    }

    int queryPath(int v, int w) {
        return queryPath(nodes[v], nodes[w]);
    }

    int lca(int v, int w) {
        Node *x = lca(nodes[v], nodes[w]);
        return nullptr == x ? -1 : x->vertex;
    }

    bool conneted(int v, int w) {
        return findRoot(nodes[v]) == findRoot(nodes[w]);
    }

    int getV() {
        return V;
    }
};

#endif /* DATASTRUCTURES_GRAPH_QUERIES_ROOTEDLINKCUTTREE_H_ */
