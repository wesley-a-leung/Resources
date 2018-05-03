#ifndef DATASTRUCTURES_GEOMETRY_KDTREE_H_
#define DATASTRUCTURES_GEOMETRY_KDTREE_H_

#include <bits/stdc++.h>
#include "datastructures/geometry/Point2D.h"
#include "datastructures/geometry/Rectangle.h"
using namespace std;

typedef double T;
constexpr static T EPS = 1e-9;

struct KdTree {
private:
    bool VERTICAL = false;
    bool HORIZONTAL = true;
    T XMIN, YMIN, XMAX, YMAX;

    struct Node {
        Point2D *p;
        Node *leftUp, *rightDown;
        Rectangle *r;

        Node(Point2D *p, Rectangle *r) {
            this->p = p;
            this->r = r;
            this->leftUp = nullptr;
            this->rightDown = nullptr;
        }
    };

    int cnt;
    Node *root;

    Node *construct(Node *n, Point2D *points, int lo, int hi, bool partition, T xmin, T ymin, T xmax, T ymax) {
        if (lo > hi) return nullptr;
        if (partition == VERTICAL) sort(points + lo, points + hi + 1, Point2D::xOrderLt);
        else sort(points + lo, points + hi + 1, Point2D::yOrderLt);
        int mid = lo + (hi - lo) / 2;
        Point2D *p = &points[mid];
        n = new Node(p, new Rectangle(xmin, ymin, xmax, ymax));
        if (partition == VERTICAL) {
            n->leftUp = construct(n->leftUp, points, lo, mid - 1, !partition, xmin, ymin, n->p->x, ymax);
            n->rightDown = construct(n->rightDown, points, mid + 1, hi, !partition, n->p->x, ymin, xmax, ymax);
        } else {
            n->leftUp = construct(n->leftUp, points, lo, mid - 1, !partition, xmin, ymin, xmax, n->p->y);
            n->rightDown = construct(n->rightDown, points, mid + 1, hi, !partition, xmin, n->p->y, xmax, ymax);
        }
        return n;
    }

    Node *insert(Node *n, Point2D *p, bool partition, T xmin, T ymin, T xmax, T ymax) {
        if (nullptr == n) {
            cnt++;
            return new Node(p, new Rectangle(xmin, ymin, xmax, ymax));
        }
        if (*(n->p) == *p) return n;
        if (partition == VERTICAL) {
            if (Point2D::xOrderLt(*p, *(n->p))) n->leftUp = insert(n->leftUp, p, !partition, xmin, ymin, n->p->x, ymax);
            else n->rightDown = insert(n->rightDown, p, !partition, n->p->x, ymin, xmax, ymax);
        } else {
            if (Point2D::yOrderLt(*p, *(n->p))) n->leftUp = insert(n->leftUp, p, !partition, xmin, ymin, xmax, n->p->y);
            else n->rightDown = insert(n->rightDown, p, !partition, xmin, n->p->y, xmax, ymax);
        }
        return n;
    }

    bool contains(Node *n, Point2D *p, bool partition) {
        if (nullptr == n) return false;
        if (*(n->p) == *p) return true;
        if (partition == VERTICAL) {
            if (Point2D::xOrderLt(*p, *(n->p))) return contains(n->leftUp, p, !partition);
            else return contains(n->rightDown, p, !partition);
        } else {
            if (Point2D::yOrderLt(*p, *(n->p))) return contains(n->leftUp, p, !partition);
            else return contains(n->rightDown, p, !partition);
        }
    }

    void range(Node *n, queue<Point2D*> &q, Rectangle *rect) {
        if (nullptr == n || !rect->intersects(n->r)) return;
        if (rect->contains(n->p)) q.push(n->p);
        range(n->leftUp, q, rect);
        range(n->rightDown, q, rect);
    }

    Point2D *findNearest(Node *n, Point2D *p, Point2D *nearest) {
        if (nullptr == n || (nullptr != nearest && nearest->distanceSquaredTo(*p) < n->r->distanceSquaredTo(p))) return nearest;
        if (nullptr == nearest || n->p->distanceSquaredTo(*p) < nearest->distanceSquaredTo(*p)) nearest = n->p;
        if (nullptr != n->leftUp && n->leftUp->r->contains(p)) {
            nearest = findNearest(n->leftUp, p, nearest);
            nearest = findNearest(n->rightDown, p, nearest);
        } else {
            nearest = findNearest(n->rightDown, p, nearest);
            nearest = findNearest(n->leftUp, p, nearest);
        }
        return nearest;
    }

public:
    KdTree(T xmin, T ymin, T xmax, T ymax) {
        cnt = 0;
        XMIN = xmin;
        YMIN = ymin;
        XMAX = xmax;
        YMAX = ymax;
        root = nullptr;
    }

    KdTree(T xmin, T ymin, T xmax, T ymax, Point2D *points, int N) {
        cnt = N;
        XMIN = xmin;
        YMIN = ymin;
        XMAX = xmax;
        YMAX = ymax;
        root = construct(root, points, 0, N - 1, VERTICAL, XMIN, YMIN, XMAX, YMAX);
    }

    bool isEmpty() {
        return cnt == 0;
    }

    int size() {
        return cnt;
    }

    void insert(Point2D *p) {
        assert(nullptr != p);
        root = insert(root, p, VERTICAL, XMIN, YMIN, XMAX, YMAX);
    }

    bool contains(Point2D *p) {
        assert(nullptr != p);
        return contains(root, p, VERTICAL);
    }

    queue<Point2D*> range(Rectangle *rect) {
        assert(nullptr != rect);
        queue<Point2D*> q;
        range(root, q, rect);
        return q;
    }

    Point2D *findNearest(Point2D *p) {
        assert(nullptr != p);
        if (isEmpty()) return nullptr;
        return findNearest(root, p, nullptr);
    }
};

#endif /* DATASTRUCTURES_GEOMETRY_KDTREE_H_ */
