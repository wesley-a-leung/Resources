#pragma once
#include <bits/stdc++.h>
#include "Point.h"
using namespace std;

// Kd Tree for randomized 2d points
// Constructor Arguments:
//   xmin: the minimum x value
//   ymin: the minimum y value
//   xmax: the maximum x value
//   ymax: the maximum y value
//   P: the points
// Functions:
//   empty(): returns true if the tree is empty, false otherwise
//   size(): the number of points in the tree
//   insert(p): inserts p into the tree
//   contains(p): returns true if the tree contains p, false otherwise
//   range(rect): returns a vector of the points in the rectangle rect
//   nearest(p, nearest): sets nearest to the nearest point in the tree to p,
//     returns true if a point exists, false otherwise (tree is empty)
// In practice, has a moderate constant
// Time Complexity:
//   constructor: O(N) for N randomized points
//   empty, size: O(1)
//   insert, contains: O(log N) for N randomized points
//   range: O(sqrt N + K) for N randomized points and K points in the rectangle
//   nearest: O(log N) for N randomized points
// Memory Complexity: O(N)
// Tested:
//   https://open.kattis.com/problems/closestpair1
struct KdTree {
  struct Rectangle {
    T xmin, ymin, xmax, ymax;
    Rectangle(T xmin = 0, T ymin = 0, T xmax = 0, T ymax = 0)
        : xmin(xmin), ymin(ymin), xmax(xmax), ymax(ymax) {
      assert(xmin <= xmax); assert(ymin <= ymax);
    }
    bool intersects(const Rectangle &that) const {
      return !lt(xmax, that.xmin) && !lt(ymax, that.ymin)
          && !lt(that.xmax, xmin) && !lt(that.ymax, ymin);
    }
    bool contains(pt p) const {
      return !lt(p.x, xmin) && !lt(p.y, ymin)
          && !lt(xmax, p.x) && !lt(ymax, p.y);
    }
    T distSq(pt p) const {
      T dx = 0, dy = 0;
      if (p.x < xmin) dx = p.x - xmin;
      else if (p.x > xmax) dx = p.x - xmax;
      if (p.y < ymin) dy = p.y - ymin;
      else if (p.y > ymax) dy = p.y - ymax;
      return dx * dx + dy * dy;
    }
  };
  struct Node {
    pt p; Rectangle r; Node *lu, *rd;
    Node(pt p, const Rectangle &r) : p(p), r(r), lu(nullptr), rd(nullptr) {}
  };
  deque<Node> TR;
  static bool xOrdLt(pt p, pt q) { return lt(p.x, q.x); }
  static bool yOrdLt(pt p, pt q) { return lt(p.y, q.y); }
  Node *makeNode(pt p, const Rectangle &r) {
    TR.emplace_back(p, r); return &TR.back();
  }
  T XMIN, YMIN, XMAX, YMAX; int cnt; Node *root;
  template <class It>
  Node *build(Node *n, It st, int lo, int hi, bool partition,
              T xmin, T ymin, T xmax, T ymax) {
    if (lo > hi) return nullptr;
    int mid = lo + (hi - lo) / 2; if (partition)
      nth_element(st + lo, st + mid, st + hi + 1, xOrdLt);
    else nth_element(st + lo, st + mid, st + hi + 1, yOrdLt);
    pt p = *(st + mid); n = makeNode(p, Rectangle(xmin, ymin, xmax, ymax));
    if (partition) {
      n->lu = build(n->lu, st, lo, mid - 1, !partition,
                    xmin, ymin, n->p.x, ymax);
      n->rd = build(n->rd, st, mid + 1, hi, !partition,
                    n->p.x, ymin, xmax, ymax);
    } else {
      n->lu = build(n->lu, st, lo, mid - 1, !partition,
                    xmin, ymin, xmax, n->p.y);
      n->rd = build(n->rd, st, mid + 1, hi, !partition,
                    xmin, n->p.y, xmax, ymax);
    }
    return n;
  }
  Node *insert(Node *n, pt p, bool partition,
               T xmin, T ymin, T xmax, T ymax) {
    if (!n) { cnt++; return makeNode(p, Rectangle(xmin, ymin, xmax, ymax)); }
    if (n->p == p) return n;
    if (partition) {
      if (xOrdLt(p, n->p)) n->lu = insert(n->lu, p, !partition,
                                          xmin, ymin, n->p.x, ymax);
      else n->rd = insert(n->rd, p, !partition,
                          n->p.x, ymin, xmax, ymax);
    } else {
      if (yOrdLt(p, n->p)) n->lu = insert(n->lu, p, !partition,
                                          xmin, ymin, xmax, n->p.y);
      else n->rd = insert(n->rd, p, !partition,
                          xmin, n->p.y, xmax, ymax);
    }
    return n;
  }
  bool contains(Node *n, pt p, bool partition) {
    if (!n) return false;
    if (n->p == p) return true;
    if (partition) {
      if (xOrdLt(p, n->p)) return contains(n->lu, p, !partition);
      else return contains(n->rd, p, !partition);
    } else {
      if (yOrdLt(p, n->p)) return contains(n->lu, p, !partition);
      else return contains(n->rd, p, !partition);
    }
  }
  void range(Node *n, vector<pt> &ret, const Rectangle &rect) {
    if (!n || !rect.intersects(n->r)) return;
    if (rect.contains(n->p)) ret.push_back(n->p);
    range(n->lu, ret, rect); range(n->rd, ret, rect);
  }
  bool findNearest(Node *n, pt p, bool hasNearest, pt &nearest) {
    if (!n || (hasNearest && lt(distSq(nearest, p), n->r.distSq(p))))
      return hasNearest;
    if (!hasNearest || lt(distSq(n->p, p), distSq(nearest, p))) {
      hasNearest = true; nearest = n->p;
    }
    if (n->lu && n->lu->r.contains(p)) {
      hasNearest = findNearest(n->lu, p, hasNearest, nearest);
      hasNearest = findNearest(n->rd, p, hasNearest, nearest);
    } else {
      hasNearest = findNearest(n->rd, p, hasNearest, nearest);
      hasNearest = findNearest(n->lu, p, hasNearest, nearest);
    }
    return hasNearest;
  }
  KdTree(T xmin, T ymin, T xmax, T ymax)
      : XMIN(xmin), YMIN(ymin), XMAX(xmax), YMAX(ymax),
        cnt(0), root(nullptr) {}
    KdTree(T xmin, T ymin, T xmax, T ymax, vector<pt> P)
      : XMIN(xmin), YMIN(ymin), XMAX(xmax), YMAX(ymax), cnt(P.size()) {
    root = build(root, P.begin(), 0, cnt - 1, true, XMIN, YMIN, XMAX, YMAX);
  }
  bool empty() { return cnt == 0; }
  int size() { return cnt; }
  void insert(pt p) { root = insert(root, p, true, XMIN, YMIN, XMAX, YMAX); }
  bool contains(pt p) { return contains(root, p, true); }
  vector<pt> range(const Rectangle &rect) {
    vector<pt> ret; range(root, ret, rect); return ret;
  }
  bool findNearest(pt p, pt &nearest) {
    return findNearest(root, p, false, nearest);
  }
};
