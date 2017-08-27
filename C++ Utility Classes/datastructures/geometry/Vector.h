#ifndef DATASTRUCTURES_GEOMETRY_VECTOR_H_
#define DATASTRUCTURES_GEOMETRY_VECTOR_H_

#include <bits/stdc++.h>

using namespace std;

/**
 *  The {@code Vector} class represents a <em>d</em>-dimensional Euclidean vector.
 *  Vectors are immutable: their values cannot be changed after they are created.
 *  It includes methods for addition, subtraction,
 *  dot product, scalar product, unit vector, Euclidean norm, and the Euclidean
 *  distance between two vectors.
 *  <p>
 *  For additional documentation,
 *  see <a href="http://algs4.cs.princeton.edu/12oop">Section 1.2</a> of
 *  <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne.
 *
 *  @author Robert Sedgewick
 *  @author Kevin Wayne
 */
class Vector {
private:
    int d;               // dimension of the vector
    double *data;        // array of vector's components

public:
    Vector() {}

    /**
     * Initializes a d-dimensional zero vector.
     *
     * @param d the dimension of the vector
     */
    Vector(int d) {
        this->d = d;
        this->data = new double[d];
    }

    /**
     * Initializes a vector from either an array.
     *
     * @param d the dimension of the vector
     * @param a the array
     */
    Vector(int d, double *a) {
        this->d = d;
        // defensive copy so that client can't alter our copy of data[]
        this->data = new double[d];
        for (int i = 0; i < d; i++)
            this->data[i] = a[i];
    }

    /**
     * Returns the dimension of this vector.
     *
     * @return the dimension of this vector
     */
    int dimension() {
        return d;
    }

    /**
     * Returns the dot product of this vector with the specified vector.
     *
     * @param  that the other vector
     * @return the dot product of this vector and that vector
     * @throws invalid_argument if the dimensions of the two vectors are not equal
     */
    double dot(Vector &that) {
        if (d != that.d) throw invalid_argument("Dimensions don't agree");
        double sum = 0.0;
        for (int i = 0; i < d; i++)
            sum = sum + (data[i] * that.data[i]);
        return sum;
    }

    /**
     *
     * Returns the dot product of this vector with the specified vector.
     *
     * @param  that the other vector
     * @return the dot product of this vector and that vector
     * @throws invalid_argument if the dimensions of the two vectors are not equal
     */
    double operator ^ (Vector &that) {
        return dot(that);
    }

    /**
     * Returns the 2-D cross product of this vector with the specified vector.
     *
     * @param  that the other vector
     * @return the 2-D cross product of this vector and that vector
     * @throws invalid_argument if the two vectors are not 2-dimensional
     */
    double cross2D(Vector &that) {
        if (d != 2 || that.d != 2) throw invalid_argument("Vectors must be 2-dimensional");
        return data[0] * that.data[1] - data[1] * that.data[0];
    }

    /**
     * Returns the 2-D cross product of this vector with the specified vector.
     *
     * @param  that the other vector
     * @return the 2-D cross product of this vector and that vector
     * @throws invalid_argument if the two vectors are not 2-dimensional
     */
    double operator | (Vector &that) {
        return cross2D(that);
    }

    /**
     * Returns the 3-D cross product of this vector with the specified vector.
     *
     * @param  that the other vector
     * @return the 3-D cross product of this vector and that vector
     * @throws invalid_argument if the two vectors are not 3-dimensional
     */
    Vector &cross3D(Vector &that) {
        if (d != 3 || that.d != 3) throw invalid_argument("Vectors must be 3-dimensional");
        Vector *c = new Vector(d);
        c->data[0] = data[1] * that.data[2] - data[2] * that.data[1];
        c->data[1] = data[2] * that.data[0] - data[0] * that.data[2];
        c->data[2] = data[0] * that.data[1] - data[1] * that.data[0];
        return *c;
    }

    /**
     * Returns the 3-D cross product of this vector with the specified vector.
     *
     * @param  that the other vector
     * @return the 3-D cross product of this vector and that vector
     * @throws invalid_argument if the two vectors are not 3-dimensional
     */
    Vector &operator * (Vector &that) {
        return cross3D(that);
    }

    /**
     * Returns the magnitude of this vector.
     * This is also known as the L2 norm or the Euclidean norm.
     *
     * @return the magnitude of this vector
     */
    double magnitude() {
        return sqrt(this->dot(*this));
    }

    /**
     * Returns the Euclidean distance between this vector and the specified vector.
     *
     * @param  that the other vector
     * @return the Euclidean distance between this vector and that vector
     * @throws invalid_argument if the dimensions of the two vectors are not equal
     */
    double distanceTo(Vector &that) {
        if (d != that.d) throw invalid_argument("Dimensions don't agree");
        return minus(that).magnitude();
    }

    /**
     * Returns the sum of this vector and the specified vector.
     *
     * @param  that the vector to add to this vector
     * @return the vector whose value is {@code (this + that)}
     * @throws invalid_argument if the dimensions of the two vectors are not equal
     */
    Vector &plus(Vector &that) {
        if (d != that.d) throw invalid_argument("Dimensions don't agree");
        Vector *c = new Vector(d);
        for (int i = 0; i < d; i++)
            c->data[i] = data[i] + that.data[i];
        return *c;
    }

    /**
     * Returns the sum of this vector and the specified vector.
     *
     * @param  that the vector to add to this vector
     * @return the vector whose value is {@code (this + that)}
     * @throws invalid_argument if the dimensions of the two vectors are not equal
     */
    Vector &operator + (Vector &that) {
        return plus(that);
    }

    /**
     * Returns the difference between this vector and the specified vector.
     *
     * @param  that the vector to subtract from this vector
     * @return the vector whose value is {@code (this - that)}
     * @throws invalid_argument if the dimensions of the two vectors are not equal
     */
    Vector &minus(Vector &that) {
        if (d != that.d) throw invalid_argument("Dimensions don't agree");
        Vector *c = new Vector(d);
        for (int i = 0; i < d; i++)
            c->data[i] = data[i] - that.data[i];
        return *c;
    }

    /**
     * Returns the difference between this vector and the specified vector.
     *
     * @param  that the vector to subtract from this vector
     * @return the vector whose value is {@code (this - that)}
     * @throws invalid_argument if the dimensions of the two vectors are not equal
     */
    Vector &operator - (Vector &that) {
        return minus(that);
    }

    /**
     * Returns the ith cartesian coordinate.
     *
     * @param  i the coordinate index
     * @return the ith cartesian coordinate
     */
    double cartesian(int i) {
        return data[i];
    }

    // mutator operator
    double &operator [](int i) {
        return data[i];
    }

    // accessor operator
    double operator [](int i) const {
        return data[i];
    }

    /**
     * Returns the scalar-vector product of this vector and the specified scalar
     *
     * @param  alpha the scalar
     * @return the vector whose value is {@code (alpha * this)}
     */
    Vector &scale(double alpha) {
        Vector *c = new Vector(d);
        for (int i = 0; i < d; i++)
            c->data[i] = alpha * data[i];
        return *c;
    }

    /**
     * Returns the scalar-vector product of this vector and the specified scalar
     *
     * @param  alpha the scalar
     * @return the vector whose value is {@code (alpha * this)}
     */
    Vector &operator * (double alpha) {
        return scale(alpha);
    }

    /**
     * Returns the scalar-vector product of this vector and the specified the reciprocal of the scalar
     *
     * @param  alpha the scalar
     * @return the vector whose value is {@code (alpha * this)}
     */
    Vector &operator / (double alpha) {
        return scale(1.0 / alpha);
    }

    /**
     * Returns a unit vector in the direction of this vector.
     *
     * @return a unit vector in the direction of this vector
     * @throws runtime_error if this vector is the zero vector
     */
    Vector &direction() {
        if (magnitude() == 0.0) throw runtime_error("Zero-vector has no direction");
        return scale(1.0 / magnitude());
    }

    /**
     * Returns the angle of this 2-D vector to horizontal.
     *
     *
     * @return the angle of this 2-D vector to horizontal
     * @throws IllegalArgumentException if this vector is not 2-dimensional
     */
    double angle() {
        if (d != 2) throw invalid_argument("Vector must be 2-dimensional");
        return atan2(data[1], data[0]);
    }

    /**
     * Returns a projection of this vector on that vector.
     *
     * @param that the vector that this vector is to be projected on
     * @return a projection of this vector on that vector
     * @throws invalid_argument if the dimensions of the two vectors are not equal
     */
    Vector &projectionOn(Vector &that) {
        if (d != that.d) throw invalid_argument("Dimensions don't agree");
        return that.scale(dot(that) / (that.magnitude() * that.magnitude()));
    }
};

struct Point2D {
public:
    double x;
    double y;

    Point2D() {};

    /**
     * Initializes a new point (x, y).
     * @param x the x-coordinate
     * @param y the y-coordinate
     */
    Point2D(double x, double y) {
        if (x == 0.0) this->x = 0.0;  // convert -0.0 to +0.0
        else          this->x = x;

        if (y == 0.0) this->y = 0.0;  // convert -0.0 to +0.0
        else          this->y = y;
    }

    /**
     * Returns the x-coordinate.
     * @return the x-coordinate
     */
    double getX() {
        return x;
    }

    /**
     * Returns the y-coordinate.
     * @return the y-coordinate
     */
    double getY() {
        return y;
    }

    /**
     * Returns the polar radius of this point.
     * @return the polar radius of this point in polar coordiantes: sqrt(x*x + y*y)
     */
    double polarRadius() {
        return sqrt(x*x + y*y);
    }

    /**
     * Returns the angle of this point in polar coordinates.
     * @return the angle (in radians) of this point in polar coordiantes (between –&pi;/2 and &pi;/2)
     */
    double theta() {
        return atan2(y, x);
    }

    /**
     * Returns the angle between this point and that point.
     * @return the angle in radians (between –&pi; and &pi;) between this point and that point (0 if equal)
     */
    double angleTo(Point2D &that) {
        double dx = that.x - x;
        double dy = that.y - y;
        return atan2(dy, dx);
    }

    /**
     * Returns true if a→b→c is a counterclockwise turn.
     * @param a first point
     * @param b second point
     * @param c third point
     * @return { -1, 0, +1 } if a→b→c is a { clockwise, collinear; counterclocwise } turn.
     */
    static int ccw(Point2D &a, Point2D &b, Point2D &c) {
        double area2 = (b.x-a.x)*(c.y-a.y) - (b.y-a.y)*(c.x-a.x);
        if      (area2 < 0) return -1;
        else if (area2 > 0) return +1;
        else                return  0;
    }

    /**
     * Returns twice the signed area of the triangle a-b-c.
     * @param a first point
     * @param b second point
     * @param c third point
     * @return twice the signed area of the triangle a-b-c
     */
    static double area2(Point2D &a, Point2D &b, Point2D &c) {
        return (b.x-a.x)*(c.y-a.y) - (b.y-a.y)*(c.x-a.x);
    }

    /**
     * Returns the Euclidean distance between this point and that point.
     * @param that the other point
     * @return the Euclidean distance between this point and that point
     */
    double distanceTo(Point2D &that) {
        double dx = x - that.x;
        double dy = y - that.y;
        return sqrt(dx*dx + dy*dy);
    }

    /**
     * Returns the square of the Euclidean distance between this point and that point.
     * @param that the other point
     * @return the square of the Euclidean distance between this point and that point
     */
    double distanceSquaredTo(Point2D &that) {
        double dx = x - that.x;
        double dy = y - that.y;
        return dx*dx + dy*dy;
    }

    /**
     * Compares two points by y-coordinate, breaking ties by x-coordinate.
     * Formally, the invoking point (x0, y0) is less than the argument point (x1, y1)
     * if and only if either {@code y0 < y1} or if {@code y0 == y1} and {@code x0 < x1}.
     *
     * @param  that the other point
     * @return the value {@code 0} if this string is equal to the argument
     *         string (precisely when {@code equals()} returns {@code true});
     *         a negative integer if this point is less than the argument
     *         point; and a positive integer if this point is greater than the
     *         argument point
     */
    int compareTo(Point2D &that) {
        if (y < that.y) return -1;
        if (y > that.y) return +1;
        if (x < that.x) return -1;
        if (x > that.x) return +1;
        return 0;
    }

    /**
     * Compares this point to the specified point.
     *
     * @param  other the other point
     * @return {@code true} if this point equals {@code other};
     *         {@code false} otherwise
     */
    bool operator == (const Point2D &other) const {
        return x == other.x && y == other.y;
    }

    /**
     * Compares this point to the specified point.
     *
     * @param  other the other point
     * @return {@code true} if this point does not equal {@code other};
     *         {@code false} otherwise
     */
    bool operator != (const Point2D &other) const {
        return x != other.x || y != other.y;
    }

    // X ORDER

    /**
     * Compares two points by x-coordinate. Less than comparator.
     */
    static bool xOrderLt(Point2D &p, Point2D &q) { return p.x < q.x; }

    /**
     * Compares two points by x-coordinate. Less than or equal to comparator.
     */
    static bool xOrderLe(Point2D &p, Point2D &q) { return p.x < q.x; }

    /**
     * Compares two points by x-coordinate. Greater than comparator.
     */
    static bool xOrderGt(Point2D &p, Point2D &q) { return p.x > q.x; }

    /**
     * Compares two points by x-coordinate. Greater than or equal to comparator.
     */
    static bool xOrderGe(Point2D &p, Point2D &q) { return p.x >= q.x; }

    // Y ORDER

    /**
     * Compares two points by y-coordinate. Less than comparator.
     */
    static bool yOrderLt(Point2D &p, Point2D &q) { return p.y < q.y; }

    /**
     * Compares two points by y-coordinate. Less than or equal to comparator.
     */
    static bool yOrderLe(Point2D &p, Point2D &q) { return p.y <= q.y; }

    /**
     * Compares two points by y-coordinate. Greater than comparator.
     */
    static bool yOrderGt(Point2D &p, Point2D &q) { return p.y > q.y; }

    /**
     * Compares two points by y-coordinate. Greater than or equal to comparator.
     */
    static bool yOrderGe(Point2D &p, Point2D &q) { return p.y >= q.y; }

    // R ORDER

    /**
     * Compares two points by polar radius. Less than comparator.
     */
    static bool rOrderLt(Point2D &p, Point2D &q) { return (p.x*p.x + p.y*p.y) < (q.x*q.x + q.y*q.y); }

    /**
     * Compares two points by polar radius. Less than or equal to comparator.
     */
    static bool rOrderLe(Point2D &p, Point2D &q) { return (p.x*p.x + p.y*p.y) <= (q.x*q.x + q.y*q.y); }

    /**
     * Compares two points by polar radius. Greater than comparator.
     */
    static bool rOrderGt(Point2D &p, Point2D &q) { return (p.x*p.x + p.y*p.y) > (q.x*q.x + q.y*q.y); }

    /**
     * Compares two points by polar radius. Greater than or equal to comparator.
     */
    static bool rOrderGe(Point2D &p, Point2D &q) { return (p.x*p.x + p.y*p.y) >= (q.x*q.x + q.y*q.y); }

    // Polar Order

    /**
     * Compares two points by polar angle (between 0 and 2&pi;) with respect to this point.
     * Less than comparator.
     */
    bool polarOrderLt(Point2D &q1, Point2D &q2) {
        double dx1 = q1.x - x;
        double dy1 = q1.y - y;
        double dx2 = q2.x - x;
        double dy2 = q2.y - y;

        if      (dy1 >= 0 && dy2 < 0) return true;    // q1 above; q2 below
        else if (dy2 >= 0 && dy1 < 0) return false;   // q1 below; q2 above
        else if (dy1 == 0 && dy2 == 0) {              // 3-collinear and horizontal
            if      (dx1 >= 0 && dx2 < 0) return true;
            else if (dx2 >= 0 && dx1 < 0) return false;
            else                          return false;
        }
        else return ccw(*this, q1, q2) > 0;     // both above or below
        // Note: ccw() recomputes dx1, dy1, dx2, and dy2
    }

    /**
     * Compares two points by polar angle (between 0 and 2&pi;) with respect to this point.
     * Less than or equal to comparator.
     */
    bool polarOrderLe(Point2D &q1, Point2D &q2) {
        double dx1 = q1.x - x;
        double dy1 = q1.y - y;
        double dx2 = q2.x - x;
        double dy2 = q2.y - y;

        if      (dy1 >= 0 && dy2 < 0) return true;    // q1 above; q2 below
        else if (dy2 >= 0 && dy1 < 0) return false;   // q1 below; q2 above
        else if (dy1 == 0 && dy2 == 0) {              // 3-collinear and horizontal
            if      (dx1 >= 0 && dx2 < 0) return true;
            else if (dx2 >= 0 && dx1 < 0) return false;
            else                          return true;
        }
        else return ccw(*this, q1, q2) > 0;     // both above or below
        // Note: ccw() recomputes dx1, dy1, dx2, and dy2
    }

    /**
     * Compares two points by polar angle (between 0 and 2&pi;) with respect to this point.
     * Greater than comparator.
     */
    bool polarOrderGt(Point2D &q1, Point2D &q2) {
        double dx1 = q1.x - x;
        double dy1 = q1.y - y;
        double dx2 = q2.x - x;
        double dy2 = q2.y - y;

        if      (dy1 >= 0 && dy2 < 0) return false;   // q1 above; q2 below
        else if (dy2 >= 0 && dy1 < 0) return true;    // q1 below; q2 above
        else if (dy1 == 0 && dy2 == 0) {              // 3-collinear and horizontal
            if      (dx1 >= 0 && dx2 < 0) return false;
            else if (dx2 >= 0 && dx1 < 0) return true;
            else                          return false;
        }
        else return ccw(*this, q1, q2) > 0;     // both above or below
        // Note: ccw() recomputes dx1, dy1, dx2, and dy2
    }

    /**
     * Compares two points by polar angle (between 0 and 2&pi;) with respect to this point.
     * Greater than or equal to comparator.
     */
    bool polarOrderGe(Point2D &q1, Point2D &q2) {
        double dx1 = q1.x - x;
        double dy1 = q1.y - y;
        double dx2 = q2.x - x;
        double dy2 = q2.y - y;

        if      (dy1 >= 0 && dy2 < 0) return false;   // q1 above; q2 below
        else if (dy2 >= 0 && dy1 < 0) return true;    // q1 below; q2 above
        else if (dy1 == 0 && dy2 == 0) {              // 3-collinear and horizontal
            if      (dx1 >= 0 && dx2 < 0) return false;
            else if (dx2 >= 0 && dx1 < 0) return true;
            else                          return true;
        }
        else return ccw(*this, q1, q2) > 0;     // both above or below
        // Note: ccw() recomputes dx1, dy1, dx2, and dy2
    }

    // Atan2 Order

    /**
     * Compares two points by atan2() angle (between –&pi; and &pi;) with respect to this point.
     * Less than comparator.
     */
    bool atan2OrderLt(Point2D &q1, Point2D &q2) { return angleTo(q1) < angleTo(q2); }

    /**
     * Compares two points by atan2() angle (between –&pi; and &pi;) with respect to this point.
     * Less than or equal to comparator.
     */
    bool atan2OrderLe(Point2D &q1, Point2D &q2) { return angleTo(q1) <= angleTo(q2); }

    /**
     * Compares two points by atan2() angle (between –&pi; and &pi;) with respect to this point.
     * Greater than comparator.
     */
    bool atan2OrderGt(Point2D &q1, Point2D &q2) { return angleTo(q1) > angleTo(q2); }

    /**
     * Compares two points by atan2() angle (between –&pi; and &pi;) with respect to this point.
     * Greater than or equal to comparator.
     */
    bool atan2OrderGe(Point2D &q1, Point2D &q2) { return angleTo(q1) >= angleTo(q2); }

    // DistanceTo Order

    /**
     * Compares two points by distance to this point. Less than comparator.
     */
    bool distanceToOrderLt(Point2D &p, Point2D &q) { return distanceSquaredTo(p) < distanceSquaredTo(q); }

    /**
     * Compares two points by distance to this point. Less than or equal to comparator.
     */
    bool distanceToOrderLe(Point2D &p, Point2D &q) { return distanceSquaredTo(p) <= distanceSquaredTo(q); }

    /**
     * Compares two points by distance to this point. Greater than comparator.
     */
    bool distanceToOrderGt(Point2D &p, Point2D &q) { return distanceSquaredTo(p) > distanceSquaredTo(q); }

    /**
     * Compares two points by distance to this point. Greater than or equal to comparator.
     */
    bool distanceToOrderGe(Point2D &p, Point2D &q) { return distanceSquaredTo(p) >= distanceSquaredTo(q); }
};

#endif /* DATASTRUCTURES_GEOMETRY_VECTOR_H_ */
