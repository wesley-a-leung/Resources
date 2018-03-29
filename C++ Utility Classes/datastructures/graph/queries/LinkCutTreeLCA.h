#ifndef DATASTRUCTURES_GRAPH_QUERIES_LINKCUTTREELCA_H_
#define DATASTRUCTURES_GRAPH_QUERIES_LINKCUTTREELCA_H_

#include <bits/stdc++.h>
using namespace std;

struct LinkCutTreeLCA {
private:
    struct Node {
    public:
        int vertex;
        Node *left = nullptr, *right = nullptr, *parent = nullptr;

        Node (int vertex) : vertex(vertex) {};
    };

    int V;
    vector<Node*> nodes;

    bool isRoot(Node *x) {
        return nullptr == x->parent || (x != x->parent->left && x != x->parent->right);
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
    }

    void splay(Node *x) {
        while (!isRoot(x)) {
            Node *p = x->parent;
            Node *g = p->parent;
            if (!isRoot(p)) rotate((x == p->left) == (p == g->left) ? p : x);
            rotate(x);
        }
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

    Node *lca(Node *x, Node *y) {
        if (findRoot(x) != findRoot(y)) return nullptr;
        expose(x);
        return expose(y);
    }

public:
    LinkCutTreeLCA(int V) : V(V), nodes(V) {
        for (int v = 0; v < V; v++) {
            nodes[v] = new Node(v);
        }
    }

    void addNode(int v, int value) {
        nodes.push_back(new Node(v));
        V++;
    }

    bool link(int v, int w) {
        return link(nodes[v], nodes[w]);
    }

    void cutParent(int v) {
        cutParent(nodes[v]);
    }

    int lca(int v, int w) {
        Node *x = lca(nodes[v], nodes[w]);
        return nullptr == x ? -1 : x->vertex;
    }

    int getV() {
        return V;
    }
};

#endif /* DATASTRUCTURES_GRAPH_QUERIES_LINKCUTTREELCA_H_ */
