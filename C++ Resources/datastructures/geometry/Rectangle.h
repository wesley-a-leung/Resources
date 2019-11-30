#pragma once
#include <bits/stdc++.h>
#include "Point.h"
using namespace std;

struct Rectangle {
    T xmin, ymin, xmax, ymax;
    Rectangle(T xmin = 0, T ymin = 0, T xmax = 0, T ymax = 0) : xmin(xmin), ymin(ymin), xmax(xmax), ymax(ymax) {
        assert(xmin <= xmax);
        assert(ymin <= ymax);
    }
    T width() const { return xmax - xmin; }
    T height() const { return ymax - ymin; }
    bool intersects(const Rectangle &that) const {
        return le(that.xmin, xmax) && le(that.ymin, ymax) && le(xmin, that.xmax) && le(ymin, that.ymax);
    }
    Rectangle intersection(const Rectangle &that) const {
        return Rectangle(max(xmin, that.xmin), max(ymin, that.ymin), min(xmax, that.xmax), min(ymax, that.ymax));
    }
    bool contains(const Rectangle &that) const { // does this rectangle contain that rectangle
        return le(xmin, that.xmin) && le(ymin, that.ymin) && le(that.xmax, xmax) && le(that.ymax, ymax);
    }
    bool contains(const Point &p) const {
        return le(xmin, p.x) && le(ymin, p.y) && le(p.x, xmax) && le(p.y, ymax);
    }
    T distSq(const Point &p) const {
        T dx = 0, dy = 0;
        if (p.x < xmin) dx = p.x - xmin;
        else if (p.x > xmax) dx = p.x - xmax;
        if (p.y < ymin) dy = p.y - ymin;
        else if (p.y > ymax) dy = p.y - ymax;
        return dx * dx + dy * dy;
    }
    F dist(const Point &p) const { return sqrt(distSq(p)); }
    bool operator == (const Rectangle &that) const {
        return eq(xmin, that.xmin) && eq(ymin, that.ymin) && eq(xmax, that.xmax) && eq(ymax, that.ymax);
    }
    bool operator != (const Rectangle &that) const { return !(*this == that); }
};
