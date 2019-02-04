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
        return xmax - that.xmin >= -EPS && ymax - that.ymin >= -EPS && that.xmax - xmin >= -EPS && that.ymax - ymin >= -EPS;
    }
    Rectangle intersection(const Rectangle &that) const {
        return Rectangle(max(xmin, that.xmin), max(ymin, that.ymin), min(xmax, that.xmax), min(ymax, that.ymax));
    }
    bool contains(const Point &p) const {
        return (p.x - xmin >= -EPS) && (xmax - p.x >= -EPS) && (p.y - ymin >= -EPS) && (ymax - p.y >= -EPS);
    }
    T distanceSquaredTo(const Point &p) const {
        T dx = 0, dy = 0;
        if (p.x < xmin) dx = p.x - xmin;
        else if (p.x > xmax) dx = p.x - xmax;
        if (p.y < ymin) dy = p.y - ymin;
        else if (p.y > ymax) dy = p.y - ymax;
        return dx * dx + dy * dy;
    }
    F distanceTo(const Point &p) const { return sqrt(distanceSquaredTo(p)); }
    bool operator == (const Rectangle &that) const {
        if (abs(xmin - that.xmin) > EPS) return false;
        if (abs(ymin - that.ymin) > EPS) return false;
        if (abs(xmax - that.xmax) > EPS) return false;
        if (abs(ymax - that.ymax) > EPS) return false;
        return true;
    }
    bool operator != (const Rectangle &that) const {
        if (abs(xmin - that.xmin) > EPS) return true;
        if (abs(ymin - that.ymin) > EPS) return true;
        if (abs(xmax - that.xmax) > EPS) return true;
        if (abs(ymax - that.ymax) > EPS) return true;
        return false;
    }
};

struct Rectangle_hash {
    size_t operator ()(const Rectangle &r) const {
        return 31 * (31 * (31 * hash<T> {}(r.xmin) + hash<T> {}(r.ymin)) + hash<T> {}(r.xmax)) + hash<T> {}(r.ymax);
    }
};
