#ifndef ALGORITHMS_GEOMETRY_HALFPLANEINTERSECTION_H_
#define ALGORITHMS_GEOMETRY_HALFPLANEINTERSECTION_H_

#include <bits/stdc++.h>
#include "datastructures/geometry/Point2D.h"
#include "datastructures/geometry/Vector.h"
using namespace std;

class HalfPlaneIntersection {
private:
    struct Line {
    private:
        double EPS = 1e-15;
    public:
        Vector a, b;
        double rad;

        Line() {}

        Line(Vector a, Vector b) {
            this->a = a;
            this->b = b;
            this->rad = atan2(b[1], b[0]);
        }

        bool onLeft(Vector &v) {
            return (b | (v - a)) > EPS;
        }

        bool operator < (Line &l) {
            return rad < l.rad;
        }
    };

    int N;
    vector<Line> all, L, q;
    vector<Vector> v;
    double EPS = 1e-15;
    int count;

    Vector getLineIntersection(Line &la, Line &lb) {
        double t = (lb.b | (la.a - lb.a)) / (la.b | lb.b);
        return la.a + la.b * t;
    }

    bool halfPlaneIntersection(int n) {
        sort(L.begin(), L.end());
        int front = 0;
        int back = 0;
        q[0] = L[0];
        printf("%.6f %.6f %.6f %.6f\n", L[0].b[0], L[0].b[1], q[0].b[0], q[0].b[1]);
        for (int i = 1; i < n; i++) {
            while (front < back && !L[i].onLeft(v[back - 1])) back--;
            while (front < back && !L[i].onLeft(v[front])) front++;
            if (abs(q[back].b | L[i].b) < EPS) {
                if (q[back].onLeft(L[i].a)) q[back] = L[i];
            } else {
                q[++back] = L[i];
            }
            if (front < back) v[back - 1] = getLineIntersection(q[back - 1], q[back]);
        }
        while (front < back && !q[front].onLeft(v[back - 1])) back--;
        return (back - front) > 1;
    }

    bool check(int n) {
        L.clear();
        for (int i = 0; i < n * 2; i++) L.push_back(all[i]);
        return halfPlaneIntersection(n * 2);
    }

public:
    HalfPlaneIntersection(int N, Point2D *low, Point2D *high) : N(N), all(0), L(0), q(N * 2), v(N * 2) {
        for (int i = 0; i < N; i++) {
            Vector a(2), b(2), c(2), d(2);
            a[0] = 0.0;
            a[1] = high[i].y / high[i].x;
            b[0] = -1.0 / high[i].x;
            b[1] = 1.0;
            all.push_back(Line(a, b));
            c[0] = 0.0;
            c[1] = low[i].y / low[i].x;
            d[0] = 1.0 / low[i].x;
            d[1] = -1.0;
            all.push_back(Line(c, d));
        }
        int l = 1;
        int r = N;
        count = 1;
        while (l <= r) {
            int mid = l + (r - l) / 2;
            if (check(mid)) {
                count = mid;
                l = mid + 1;
            } else {
                r = mid - 1;
            }
        }
    }

    int countIntersections() {
        return count;
    }
};

#endif /* ALGORITHMS_GEOMETRY_HALFPLANEINTERSECTION_H_ */
