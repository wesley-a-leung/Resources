#pragma once
#include <bits/stdc++.h>
#include "Point.h"
#include "Rectangle.h"
using namespace std;

struct KdTree {
    bool VERTICAL = false, HORIZONTAL = true; T XMIN, YMIN, XMAX, YMAX;
    struct Node {
        pt p; Rectangle r; Node *leftUp = nullptr, *rightDown = nullptr;
        Node(ref p, const Rectangle &r) : p(p), r(r) {}
    };
    int cnt; Node *root;
    template <class It> Node *construct(Node *n, It points, int lo, int hi, bool partition, T xmin, T ymin, T xmax, T ymax) {
        if (lo > hi) return nullptr;
        int mid = lo + (hi - lo) / 2;
        if (partition == VERTICAL) nth_element(points + lo, points + mid, points + hi + 1, xOrdLt);
        else nth_element(points + lo, points + mid, points + hi + 1, yOrdLt);
        pt p = *(points + mid);
        n = new Node(p, Rectangle(xmin, ymin, xmax, ymax));
        if (partition == VERTICAL) {
            n->leftUp = construct(n->leftUp, points, lo, mid - 1, !partition, xmin, ymin, n->p.x, ymax);
            n->rightDown = construct(n->rightDown, points, mid + 1, hi, !partition, n->p.x, ymin, xmax, ymax);
        } else {
            n->leftUp = construct(n->leftUp, points, lo, mid - 1, !partition, xmin, ymin, xmax, n->p.y);
            n->rightDown = construct(n->rightDown, points, mid + 1, hi, !partition, xmin, n->p.y, xmax, ymax);
        }
        return n;
    }
    Node *insert(Node *n, ref p, bool partition, T xmin, T ymin, T xmax, T ymax) {
        if (nullptr == n) {
            cnt++;
            return new Node(p, Rectangle(xmin, ymin, xmax, ymax));
        }
        if (n->p == p) return n;
        if (partition == VERTICAL) {
            if (xOrdLt(p, n->p)) n->leftUp = insert(n->leftUp, p, !partition, xmin, ymin, n->p.x, ymax);
            else n->rightDown = insert(n->rightDown, p, !partition, n->p.x, ymin, xmax, ymax);
        } else {
            if (yOrdLt(p, n->p)) n->leftUp = insert(n->leftUp, p, !partition, xmin, ymin, xmax, n->p.y);
            else n->rightDown = insert(n->rightDown, p, !partition, xmin, n->p.y, xmax, ymax);
        }
        return n;
    }
    bool contains(Node *n, ref p, bool partition) {
        if (nullptr == n) return false;
        if (n->p == p) return true;
        if (partition == VERTICAL) {
            if (xOrdLt(p, n->p)) return contains(n->leftUp, p, !partition);
            else return contains(n->rightDown, p, !partition);
        } else {
            if (yOrdLt(p, n->p)) return contains(n->leftUp, p, !partition);
            else return contains(n->rightDown, p, !partition);
        }
    }
    void range(Node *n, queue<pt> &q, const Rectangle &rect) {
        if (nullptr == n || !rect.intersects(n->r)) return;
        if (rect.contains(n->p)) q.push(n->p);
        range(n->leftUp, q, rect); range(n->rightDown, q, rect);
    }
    pt *findNearest(Node *n, ref p, pt *nearest) {
        if (nullptr == n || (nullptr != nearest && lt(distSq(*nearest, p), n->r.distSq(p)))) return nearest;
        if (nullptr == nearest || lt(distSq(n->p, p), distSq(*nearest, p))) nearest = &(n->p);
        if (nullptr != n->leftUp && n->leftUp->r.contains(p)) {
            nearest = findNearest(n->leftUp, p, nearest);
            nearest = findNearest(n->rightDown, p, nearest);
        } else {
            nearest = findNearest(n->rightDown, p, nearest);
            nearest = findNearest(n->leftUp, p, nearest);
        }
        return nearest;
    }
    KdTree(T xmin, T ymin, T xmax, T ymax) : XMIN(xmin), YMIN(ymin), XMAX(xmax), YMAX(ymax), cnt(0), root(nullptr) {}
    template <class It> KdTree(T xmin, T ymin, T xmax, T ymax, It st, It en) :
            XMIN(xmin), YMIN(ymin), XMAX(xmax), YMAX(ymax), cnt(en - st) {
        root = construct(root, st, 0, cnt - 1, VERTICAL, XMIN, YMIN, XMAX, YMAX);
    }
    bool empty() { return cnt == 0; }
    int size() { return cnt; }
    void insert(ref p) { root = insert(root, p, VERTICAL, XMIN, YMIN, XMAX, YMAX); }
    bool contains(ref p) { return contains(root, p, VERTICAL); }
    queue<pt> range(const Rectangle &rect) { queue<pt> q; range(root, q, rect); return q; }
    pt *findNearest(ref p) {
        if (empty()) return nullptr;
        return findNearest(root, p, nullptr);
    }
};
