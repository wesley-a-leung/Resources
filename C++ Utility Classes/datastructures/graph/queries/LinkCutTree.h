#ifndef DATASTRUCTURES_GRAPH_QUERIES_LINKCUTTREE_H_
#define DATASTRUCTURES_GRAPH_QUERIES_LINKCUTTREE_H_

#include <bits/stdc++.h>
using namespace std;

struct LinkCutTree {
private:
    static const int ddef = 0, vdef = 0, qdef = 0;

    struct Node {
    public:
        int vertex, val, subtreeVal, delta, size;
        bool revert;
        Node *left = nullptr, *right = nullptr, *parent = nullptr;

        Node (int vertex, int val) : vertex(vertex), val(val), subtreeVal(val), delta(LinkCutTree::ddef), size(1), revert(false) {};
    };

    int V;
    vector<Node*> nodes;

    int apply(int a, int b) {
        return a + b;
    }

    int merge(int l, int r) {
        return l + r;
    }

    int getSegmentVal(int delta, int len) {
        return delta * len;
    }

    int applyDelta(int val, int delta) {
        if (ddef == delta) return val;
        return apply(val, delta);
    }

    int joinDeltas(int delta1, int delta2) {
        if (ddef == delta1) return delta2;
        if (ddef == delta2) return delta1;
        return apply(delta1, delta2);
    }

    bool isRoot(Node *x) {
        return nullptr == x->parent || (x != x->parent->left && x != x->parent->right);
    }

    int getSize(Node *x) {
        return nullptr == x ? 0 : x->size;
    }

    int getSubtreeVal(Node *x) {
        return nullptr == x ? vdef : applyDelta(x->subtreeVal, getSegmentVal(x->delta, x->size));
    }

    void propagate(Node *x) {
        if (x->revert) {
            x->revert = false;
            Node *t = x->left;
            x->left = x->right;
            x->right = t;
            if (nullptr != x->left) x->left->revert = !x->left->revert;
            if (nullptr != x->right) x->right->revert = !x->right->revert;
        }
        x->val = applyDelta(x->val, x->delta);
        x->subtreeVal = applyDelta(x->subtreeVal, getSegmentVal(x->delta, x->size));
        if (nullptr != x->left) x->left->delta = joinDeltas(x->left->delta, x->delta);
        if (nullptr != x->right) x->right->delta = joinDeltas(x->right->delta, x->delta);
        x->delta = ddef;
    }

    void update(Node *x) {
        x->subtreeVal = merge(merge(getSubtreeVal(x->left), applyDelta(x->val, x->delta)), getSubtreeVal(x->right));
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
            if (!isRoot(p)) propagate(g);
            propagate(p);
            propagate(x);
            if (!isRoot(p)) rotate((x == p->left) == (p == g->left) ? p : x);
            rotate(x);
        }
        propagate(x);
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

    void makeRoot(Node *x) {
        expose(x);
        x->revert = !x->revert;
    }

    bool connected(Node *x, Node *y) {
        if (x == y) return true;
        expose(x);
        expose(y);
        return nullptr != x->parent;
    }

    bool link(Node *x, Node *y) {
        if (connected(x, y)) return false;
        makeRoot(x);
        x->parent = y;
        return true;
    }

    bool cut(Node *x, Node *y) {
        makeRoot(x);
        expose(y);
        if (x != y->right || nullptr != x->left) return false;
        y->right->parent = nullptr;
        y->right = nullptr;
        return true;
    }

    bool modify(Node *from, Node *to, int delta) {
        if (!connected(from, to)) return false;
        makeRoot(from);
        expose(to);
        to->delta = joinDeltas(to->delta, delta);
        return true;
    }

    int query(Node *from, Node *to) {
        if (!connected(from, to)) return qdef;
        makeRoot(from);
        expose(to);
        return getSubtreeVal(to);
    }

public:
    LinkCutTree(int V) : V(V), nodes(V) {
        for (int v = 0; v < V; v++) {
            nodes[v] = new Node(v, vdef);
        }
    }

    LinkCutTree(int V, int *values) : V(V), nodes(V) {
        for (int v = 0; v < V; v++) {
            nodes[v] = new Node(v, values[v]);
        }
    }

    void addNode(int v, int value) {
        nodes.push_back(new Node(v, value));
        V++;
    }

    bool link(int v, int w) {
        return link(nodes[v], nodes[w]);
    }

    bool cut(int v, int w) {
        return cut(nodes[v], nodes[w]);
    }

    bool modify(int v, int w, int delta) {
        return modify(nodes[v], nodes[w], delta);
    }

    int query(int v, int w) {
        return query(nodes[v], nodes[w]);
    }

    bool connected(int v, int w) {
        return connected(nodes[v], nodes[w]);
    }

    int getV() {
        return V;
    }
};

#endif /* DATASTRUCTURES_GRAPH_QUERIES_LINKCUTTREE_H_ */
