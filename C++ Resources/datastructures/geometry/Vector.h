#pragma once
#include <bits/stdc++.h>
using namespace std;

using T = double; using F = double; const F EPS = 1e-9;

struct Vector {
    int d; vector<T> data;
    Vector() {}
    Vector(int d) : d(d), data(d, 0) {}
    template <class It> Vector(It st, It en) : d(en - st), data(st, en) {}
    int dimension() const { return d; }
    T dot(const Vector &that) const {
        if (d != that.d) throw invalid_argument("Dimensions don't agree");
        T sum = 0;
        for (int i = 0; i < d; i++) sum = sum + (data[i] * that.data[i]);
        return sum;
    }
    T operator * (const Vector &that) const { return dot(that); }
    T cross2D(const Vector &that) const {
        if (d != 2 || that.d != 2) throw invalid_argument("Vectors must be 2-dimensional");
        return data[0] * that.data[1] - data[1] * that.data[0];
    }
    T operator | (const Vector &that) const { return cross2D(that); }
    Vector cross3D(const Vector &that) const {
        if (d != 3 || that.d != 3) throw invalid_argument("Vectors must be 3-dimensional");
        Vector c(d);
        c.data[0] = data[1] * that.data[2] - data[2] * that.data[1];
        c.data[1] = data[2] * that.data[0] - data[0] * that.data[2];
        c.data[2] = data[0] * that.data[1] - data[1] * that.data[0];
        return c;
    }
    Vector operator ^ (const Vector &that) const { return cross3D(that); }
    F magnitude() const { return sqrt(dot(*this)); }
    F distanceTo(const Vector &that) const { // treating this and that vector as points
        if (d != that.d) throw invalid_argument("Dimensions don't agree");
        return minus(that).magnitude();
    }
    // treating this vector as a point, p as a point on the line, and dir as the direction vector of the line
    F distanceTo(const Vector &p, const Vector &dir) const {
        if (d != p.d || d != dir.d) throw invalid_argument("Dimensions don't agree");
        Vector sub = p.minus(*this), n = dir.direction(); return sub.minus(n.scale(sub.dot(n))).magnitude();
    }
    Vector plus(const Vector &that) const {
        if (d != that.d) throw invalid_argument("Dimensions don't agree");
        Vector c(d);
        for (int i = 0; i < d; i++) c.data[i] = data[i] + that.data[i];
        return c;
    }
    Vector operator + (const Vector &that) const { return plus(that); }
    Vector minus(const Vector &that) const {
        if (d != that.d) throw invalid_argument("Dimensions don't agree");
        Vector c(d);
        for (int i = 0; i < d; i++) c.data[i] = data[i] - that.data[i];
        return c;
    }
    Vector operator - (const Vector &that) const { return minus(that); }
    const T &cartesian(const int i) const { return data[i]; }
    T &cartesian(const int i) { return data[i]; }
    const T &operator [](const int i) const { return data[i]; }
    T &operator [](const int i) { return data[i]; }
    Vector scale(F alpha) const {
        Vector c(d);
        for (int i = 0; i < d; i++) c.data[i] = data[i] * alpha;
        return c;
    }
    Vector operator * (F alpha) const { return scale(alpha); }
    Vector operator / (F alpha) const { return scale(1.0 / alpha); }
    Vector direction() const {
        if (magnitude() <= EPS) throw runtime_error("Zero-vector has no direction");
        return scale(1.0 / magnitude());
    }
    F directionCosine(int i) const { return data[i] / magnitude(); }
    Vector projectionOn(const Vector &that) const {
        if (d != that.d) throw invalid_argument("Dimensions don't agree");
        return that.scale(dot(that) / that.dot(that));
    }
    Vector perpendicularTo(const Vector &that) const {
        if (d != that.d) throw invalid_argument("Dimensions don't agree");
        return minus(projectionOn(that));
    }
    Vector reflectionOnLine(const Vector &that) const {
        if (d == 2 && that.d == 2) return projectionOn(that).scale(2).minus(*this);
        else throw invalid_argument("Vectors must be 2-dimensional");
    }
    Vector reflectionOnHyperplane(const Vector &that) const {
        if (d == 3 && that.d == 3) return minus(projectionOn(that).scale(2));
        else throw invalid_argument("Vectors must be 3-dimensional");
    }
    Vector rotate(const Vector &that, F theta) const {
        if (d == 2 && that.d == 2) {
            Vector r(2);
            r.data[0] = that.data[0] + (data[0] - that.data[0]) * cos(theta) - (data[1] - that.data[1]) * sin(theta);
            r.data[1] = that.data[1] + (data[0] - that.data[0]) * sin(theta) + (data[1] - that.data[1]) * cos(theta);
            return r;
        } else if (d == 3 && that.d == 3) {
            return scale(cos(theta)).plus(that.direction().cross3D(*this).scale(sin(theta))).plus(that.direction().
                scale(that.direction().dot(*this)).scale(1.0 - cos(theta)));
        } else if (d == that.d) throw invalid_argument("Vectors must be 2-dimensional or 3-dimensional");
        else throw invalid_argument("Dimensions don't agree");
    }
    bool operator < (const Vector &that) const {
        for (int i = 0; i < d; i++) if (abs(data[i] - that.data[i]) > EPS) return data[i] < that.data[i];
        return false;
    }
    bool operator <= (const Vector &that) const { return !(that < *this); }
    bool operator > (const Vector &that) const { return that < *this; }
    bool operator >= (const Vector &that) const { return !(*this < that); }
    bool operator == (const Vector &that) const { return !(*this < that) && !(that < *this); }
    bool operator != (const Vector &that) const { return !(*this == that); }
};
