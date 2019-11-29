#pragma once
#include <bits/stdc++.h>
#include "Point.h"
#include "Rectangle.h"
using namespace std;

struct KdTree {
    bool VERTICAL = false, HORIZONTAL = true;
    T XMIN, YMIN, XMAX, YMAX;
    struct Node {
        Point p; Rectangle r;
        Node *leftUp = nullptr, *rightDown = nullptr;
        Node(const Point &p, const Rectangle &r) : p(p), r(r) {}
    };
    int cnt; Node *root;
    bool cmpX(const Point &a, const Point &b) { return lt(x(a), x(b)); }
    bool cmpY(const Point &a, const Point &b) { return lt(y(a), y(b)); }
    template <class It> Node *construct(Node *n, It points, int lo, int hi, bool partition, T xmin, T ymin, T xmax, T ymax) {
        if (lo > hi) return nullptr;
        int mid = lo + (hi - lo) / 2;
        if (partition == VERTICAL) nth_element(points + lo, points + mid, points + hi + 1, cmpX);
        else nth_element(points + lo, points + mid, points + hi + 1, cmpY);
        Point p = *(points + mid);
        n = new Node(p, Rectangle(xmin, ymin, xmax, ymax));
        if (partition == VERTICAL) {
            n->leftUp = construct(n->leftUp, points, lo, mid - 1, !partition, xmin, ymin, x(n->p), ymax);
            n->rightDown = construct(n->rightDown, points, mid + 1, hi, !partition, x(n->p), ymin, xmax, ymax);
        } else {
            n->leftUp = construct(n->leftUp, points, lo, mid - 1, !partition, xmin, ymin, xmax, y(n->p));
            n->rightDown = construct(n->rightDown, points, mid + 1, hi, !partition, xmin, y(n->p), xmax, ymax);
        }
        return n;
    }
    Node *insert(Node *n, const Point &p, bool partition, T xmin, T ymin, T xmax, T ymax) {
        if (nullptr == n) {
            cnt++;
            return new Node(p, Rectangle(xmin, ymin, xmax, ymax));
        }
        if (n->p == p) return n;
        if (partition == VERTICAL) {
            if (cmpX(p, n->p)) n->leftUp = insert(n->leftUp, p, !partition, xmin, ymin, x(n->p), ymax);
            else n->rightDown = insert(n->rightDown, p, !partition, x(n->p), ymin, xmax, ymax);
        } else {
            if (cmpY(p, n->p)) n->leftUp = insert(n->leftUp, p, !partition, xmin, ymin, xmax, y(n->p));
            else n->rightDown = insert(n->rightDown, p, !partition, xmin, y(n->p), xmax, ymax);
        }
        return n;
    }
    bool contains(Node *n, const Point &p, bool partition) {
        if (nullptr == n) return false;
        if (n->p == p) return true;
        if (partition == VERTICAL) {
            if (cmpX(p, n->p)) return contains(n->leftUp, p, !partition);
            else return contains(n->rightDown, p, !partition);
        } else {
            if (cmpY(p, n->p)) return contains(n->leftUp, p, !partition);
            else return contains(n->rightDown, p, !partition);
        }
    }
    void range(Node *n, queue<Point> &q, const Rectangle &rect) {
        if (nullptr == n || !rect.intersects(n->r)) return;
        if (rect.contains(n->p)) q.push(n->p);
        range(n->leftUp, q, rect); range(n->rightDown, q, rect);
    }
    Point *findNearest(Node *n, const Point &p, Point *nearest) {
        if (nullptr == n || (nullptr != nearest && distSq(*nearest, p) < distSq(n->r, p))) return nearest;
        if (nullptr == nearest || distSq(n->p, p) < distSq(*nearest, p)) nearest = &(n->p);
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
    void insert(const Point &p) { root = insert(root, p, VERTICAL, XMIN, YMIN, XMAX, YMAX); }
    bool contains(const Point &p) { return contains(root, p, VERTICAL); }
    queue<Point> range(const Rectangle &rect) { queue<Point> q; range(root, q, rect); return q; }
    Point *findNearest(const Point &p) {
        if (empty()) return nullptr;
        return findNearest(root, p, nullptr);
    }
};
