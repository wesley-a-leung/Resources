#ifndef DATASTRUCTURES_GEOMETRY_POINT2D_H_
#define DATASTRUCTURES_GEOMETRY_POINT2D_H_

#include <bits/stdc++.h>
using namespace std;

/**
 *  The {@code Point} class is an immutable data type to encapsulate a
 *  two-dimensional point with real-value coordinates.
 *  <p>
 *  Note: in order to deal with the difference behavior of double and
 *  Double with respect to -0.0 and +0.0, the Point2D constructor converts
 *  any coordinates that are -0.0 to +0.0.
 *  <p>
 *  For additional documentation,
 *  see <a href="http://algs4.cs.princeton.edu/12oop">Section 1.2</a> of
 *  <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne.
 *
 *  @author Robert Sedgewick
 *  @author Kevin Wayne
 */
typedef double T;
constexpr static T EPS = 1e-9;

struct Point2D {
public:
    T x;
    T y;

    Point2D() {}

    /**
     * Initializes a new point (x, y).
     * @param x the x-coordinate
     * @param y the y-coordinate
     */
    Point2D(T x, T y) {
        this->x = x;
        this->y = y;
    }

    /**
     * Returns the x-coordinate.
     * @return the x-coordinate
     */
    T getX() {
        return x;
    }

    /**
     * Returns the y-coordinate.
     * @return the y-coordinate
     */
    T getY() {
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
     * @return the angle (in radians) of this point in polar coordiantes (between -&pi;/2 and &pi;/2)
     */
    double theta() {
        return atan2(y, x);
    }

    /**
     * Returns the angle between this point and that point.
     * @return the angle in radians (between -&pi; and &pi;) between this point and that point (0 if equal)
     */
    double angleTo(Point2D &that) {
        T dx = that.x - x;
        T dy = that.y - y;
        return atan2(dy, dx);
    }

    /**
     * Returns true if a->b->c is a counterclockwise turn.
     * @param a first point
     * @param b second point
     * @param c third point
     * @return { -1, 0, +1 } if a->b->c is a { clockwise, collinear; counterclockwise } turn.
     */
    static int ccw(Point2D &a, Point2D &b, Point2D &c) {
        T area2 = (b.x-a.x)*(c.y-a.y) - (b.y-a.y)*(c.x-a.x);
        if      (area2 < -EPS) return -1;
        else if (area2 > EPS) return +1;
        else                  return  0;
    }

    /**
     * Returns twice the signed area of the triangle a-b-c.
     * @param a first point
     * @param b second point
     * @param c third point
     * @return twice the signed area of the triangle a-b-c
     */
    static T area2(Point2D &a, Point2D &b, Point2D &c) {
        return (b.x-a.x)*(c.y-a.y) - (b.y-a.y)*(c.x-a.x);
    }

    /**
     * Returns a point that is this point rotated theta radians around that point.
     *
     * @param that the pivot point
     * @param theta the angle in radians
     * @return a point that is this point rotated theta radians around that point
     */
    Point2D &rotate(Point2D &that, double theta) {
        T x = that.x + (this->x - that.x) * cos(theta) - (this->y - that.y) * sin(theta);
        T y = that.y + (this->x - that.x) * sin(theta) + (this->y - that.y) * cos(theta);
        return *(new Point2D(x, y));
    }

    /**
     * Returns the Euclidean distance between this point and that point.
     * @param that the other point
     * @return the Euclidean distance between this point and that point
     */
    double distanceTo(Point2D &that) {
        T dx = x - that.x;
        T dy = y - that.y;
        return sqrt(dx*dx + dy*dy);
    }

    /**
     * Returns the square of the Euclidean distance between this point and that point.
     * @param that the other point
     * @return the square of the Euclidean distance between this point and that point
     */
    T distanceSquaredTo(Point2D &that) {
        T dx = x - that.x;
        T dy = y - that.y;
        return dx*dx + dy*dy;
    }

    /**
     * Is this point on the line segment pq?
     * @param p one point of the line segment
     * @param q the other point of the line segment
     * @returns whether this point is on the line segment pq
     */
    bool onSegment(Point2D &p, Point2D &q) {
        return x <= max(p.x, q.x) + EPS && x >= min(p.x, q.x) - EPS && y <= max(p.y, q.y) + EPS && y >= min(p.y, q.y) - EPS;
    }

    /**
     * Does line segment p1q1 intersect line segment p2q2?
     *
     * @param p1 one point of the first line segment
     * @param q1 the other point of the first line segment
     * @param p2 one point of the second line segment
     * @param q2 the other point of the second line segment
     * @return whether line segment p1q1 intersects line segment p2q2
     */
    static bool intersects(Point2D &p1, Point2D &q1, Point2D &p2, Point2D &q2) {
        // Find the four orientations needed for general and special cases
        int o1 = Point2D::ccw(p1, q1, p2);
        int o2 = Point2D::ccw(p1, q1, q2);
        int o3 = Point2D::ccw(p2, q2, p1);
        int o4 = Point2D::ccw(p2, q2, q1);
        // General case
        if (o1 != o2 && o3 != o4) return true;
        // Special Cases
        // p1, q1 and p2 are colinear and p2 lies on segment p1q1
        if (o1 == 0 && p2.onSegment(p1, q1)) return true;
        // p1, q1 and p2 are colinear and q2 lies on segment p1q1
        if (o2 == 0 && q2.onSegment(p1, q1)) return true;
        // p2, q2 and p1 are colinear and p1 lies on segment p2q2
        if (o3 == 0 && p1.onSegment(p2, q2)) return true;
         // p2, q2 and q1 are colinear and q1 lies on segment p2q2
        if (o4 == 0 && q1.onSegment(p2, q2)) return true;
        return false; // Doesn't fall in any of the above cases
    }

    /**
     * Calculates the point of intersection of 2 lines.
     *
     * @param p1 one point on the first line
     * @param q1 the other point on the first line
     * @param p2 one point on the second line
     * @param q2 the other point on the second line
     * @return the point of intersection of the 2 lines
     * @throws RuntimeException if there is no point of intersection
     */
    static Point2D &intersection(Point2D &p1, Point2D &q1, Point2D &p2, Point2D &q2) {
        T A1 = q1.y - p1.y;
        T B1 = p1.x - q1.x;
        T C1 = A1 * p1.x + B1 * p1.y;
        T A2 = q2.y - p2.y;
        T B2 = p2.x - q2.x;
        T C2 = A2 * p2.x + B2 * p2.y;
        T det = A1 * B2 - A2 * B1;
        if (abs(det) <= EPS) throw runtime_error("The lines do not intersect");
        Point2D *r = new Point2D((B2 * C1 - B1 * C2) / det, (A1 * C2 - A2 * C1) / det);
        return *r;
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
        if (y < that.y - EPS) return -1;
        if (y > that.y + EPS) return +1;
        if (x < that.x - EPS) return -1;
        if (x > that.x + EPS) return +1;
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
        return abs(x - other.x) <= EPS && abs(y - other.y) <= EPS;
    }

    /**
     * Compares this point to the specified point.
     *
     * @param  other the other point
     * @return {@code true} if this point does not equal {@code other};
     *         {@code false} otherwise
     */
    bool operator != (const Point2D &other) const {
        return abs(x - other.x) > EPS || abs(y - other.y) > EPS;
    }

    // X ORDER

    /**
     * Compares two points by x-coordinate. Less than comparator.
     */
    static bool xOrderLt(Point2D &p, Point2D &q) { return q.x - p.x < EPS; }

    /**
     * Compares two points by x-coordinate. Less than or equal to comparator.
     */
    static bool xOrderLe(Point2D &p, Point2D &q) { return q.x - p.x < -EPS; }

    /**
     * Compares two points by x-coordinate. Greater than comparator.
     */
    static bool xOrderGt(Point2D &p, Point2D &q) { return p.x - q.x < EPS; }

    /**
     * Compares two points by x-coordinate. Greater than or equal to comparator.
     */
    static bool xOrderGe(Point2D &p, Point2D &q) { return p.x - q.x < -EPS; }

    // Y ORDER

    /**
     * Compares two points by y-coordinate. Less than comparator.
     */
    static bool yOrderLt(Point2D &p, Point2D &q) { return q.y - p.y < EPS; }

    /**
     * Compares two points by y-coordinate. Less than or equal to comparator.
     */
    static bool yOrderLe(Point2D &p, Point2D &q) { return q.y - p.y < -EPS; }

    /**
     * Compares two points by y-coordinate. Greater than comparator.
     */
    static bool yOrderGt(Point2D &p, Point2D &q) { return p.y - q.y < EPS; }

    /**
     * Compares two points by y-coordinate. Greater than or equal to comparator.
     */
    static bool yOrderGe(Point2D &p, Point2D &q) { return p.y - q.y < -EPS; }

    // R ORDER

    /**
     * Compares two points by polar radius. Less than comparator.
     */
    static bool rOrderLt(Point2D &p, Point2D &q) { return (q.x*q.x + q.y*q.y) - (p.x*p.x + p.y*p.y) < EPS; }

    /**
     * Compares two points by polar radius. Less than or equal to comparator.
     */
    static bool rOrderLe(Point2D &p, Point2D &q) { return (q.x*q.x + q.y*q.y) - (p.x*p.x + p.y*p.y) < -EPS; }

    /**
     * Compares two points by polar radius. Greater than comparator.
     */
    static bool rOrderGt(Point2D &p, Point2D &q) { return (p.x*p.x + p.y*p.y) - (q.x*q.x + q.y*q.y) < EPS; }

    /**
     * Compares two points by polar radius. Greater than or equal to comparator.
     */
    static bool rOrderGe(Point2D &p, Point2D &q) { return (p.x*p.x + p.y*p.y) - (q.x*q.x + q.y*q.y) < -EPS; }

    // POLAR ORDER

    /**
     * Compares two points by polar angle (between 0 and 2&pi;) with respect to this point.
     * Less than comparator.
     */
    bool polarOrderLt(Point2D &q1, Point2D &q2) {
        T dx1 = q1.x - x;
        T dy1 = q1.y - y;
        T dx2 = q2.x - x;
        T dy2 = q2.y - y;

        if      (dy1 >= -EPS && dy2 < -EPS) return true;    // q1 above; q2 below
        else if (dy2 >= -EPS && dy1 < -EPS) return false;   // q1 below; q2 above
        else if (abs(dy1) <= EPS && abs(dy2) <= EPS) {      // 3-collinear and horizontal
            if      (dx1 >= -EPS && dx2 < -EPS) return true;
            else if (dx2 >= -EPS && dx1 < EPS)  return false;
            else                                return false;
        }
        else return ccw(*this, q1, q2) > 0;     // both above or below
        // Note: ccw() recomputes dx1, dy1, dx2, and dy2
    }

    /**
     * Compares two points by polar angle (between 0 and 2&pi;) with respect to this point.
     * Less than or equal to comparator.
     */
    bool polarOrderLe(Point2D &q1, Point2D &q2) {
        T dx1 = q1.x - x;
        T dy1 = q1.y - y;
        T dx2 = q2.x - x;
        T dy2 = q2.y - y;

        if      (dy1 >= -EPS && dy2 < -EPS) return true;    // q1 above; q2 below
        else if (dy2 >= -EPS && dy1 < -EPS) return false;   // q1 below; q2 above
        else if (abs(dy1) <= EPS && abs(dy2) <= EPS) {      // 3-collinear and horizontal
            if      (dx1 >= -EPS && dx2 < -EPS) return true;
            else if (dx2 >= -EPS && dx1 < EPS)  return false;
            else                                return true;
        }
        else return ccw(*this, q1, q2) > 0;     // both above or below
        // Note: ccw() recomputes dx1, dy1, dx2, and dy2
    }

    /**
     * Compares two points by polar angle (between 0 and 2&pi;) with respect to this point.
     * Greater than comparator.
     */
    bool polarOrderGt(Point2D &q1, Point2D &q2) {
        T dx1 = q1.x - x;
        T dy1 = q1.y - y;
        T dx2 = q2.x - x;
        T dy2 = q2.y - y;

        if      (dy1 >= -EPS && dy2 < -EPS) return false;   // q1 above; q2 below
        else if (dy2 >= -EPS && dy1 < -EPS) return true;    // q1 below; q2 above
        else if (abs(dy1) <= EPS && abs(dy2) <= EPS) {      // 3-collinear and horizontal
            if      (dx1 >= -EPS && dx2 < -EPS) return false;
            else if (dx2 >= -EPS && dx1 < EPS)  return true;
            else                                return false;
        }
        else return ccw(*this, q1, q2) > 0;     // both above or below
        // Note: ccw() recomputes dx1, dy1, dx2, and dy2
    }

    /**
     * Compares two points by polar angle (between 0 and 2&pi;) with respect to this point.
     * Greater than or equal to comparator.
     */
    bool polarOrderGe(Point2D &q1, Point2D &q2) {
        T dx1 = q1.x - x;
        T dy1 = q1.y - y;
        T dx2 = q2.x - x;
        T dy2 = q2.y - y;

        if      (dy1 >= -EPS && dy2 < -EPS) return false;   // q1 above; q2 below
        else if (dy2 >= -EPS && dy1 < -EPS) return true;    // q1 below; q2 above
        else if (abs(dy1) <= EPS && abs(dy2) <= EPS) {      // 3-collinear and horizontal
            if      (dx1 >= -EPS && dx2 < -EPS) return false;
            else if (dx2 >= -EPS && dx1 < EPS)  return true;
            else                                return true;
        }
        else return ccw(*this, q1, q2) > 0;     // both above or below
        // Note: ccw() recomputes dx1, dy1, dx2, and dy2
    }

    // ATAN2 ORDER

    /**
     * Compares two points by atan2() angle (between -&pi; and &pi;) with respect to this point.
     * Less than comparator.
     */
    bool atan2OrderLt(Point2D &q1, Point2D &q2) { return angleTo(q1) - angleTo(q2) < EPS; }

    /**
     * Compares two points by atan2() angle (between -&pi; and &pi;) with respect to this point.
     * Less than or equal to comparator.
     */
    bool atan2OrderLe(Point2D &q1, Point2D &q2) { return angleTo(q1) - angleTo(q2) < -EPS; }

    /**
     * Compares two points by atan2() angle (between -&pi; and &pi;) with respect to this point.
     * Greater than comparator.
     */
    bool atan2OrderGt(Point2D &q1, Point2D &q2) { return angleTo(q1) - angleTo(q2) < EPS; }

    /**
     * Compares two points by atan2() angle (between -&pi; and &pi;) with respect to this point.
     * Greater than or equal to comparator.
     */
    bool atan2OrderGe(Point2D &q1, Point2D &q2) { return angleTo(q1) - angleTo(q2) < -EPS; }

    // DISTANCETO ORDER

    /**
     * Compares two points by distance to this point. Less than comparator.
     */
    bool distanceToOrderLt(Point2D &p, Point2D &q) { return distanceSquaredTo(p) - distanceSquaredTo(q) < EPS; }

    /**
     * Compares two points by distance to this point. Less than or equal to comparator.
     */
    bool distanceToOrderLe(Point2D &p, Point2D &q) { return distanceSquaredTo(p) - distanceSquaredTo(q) < -EPS; }

    /**
     * Compares two points by distance to this point. Greater than comparator.
     */
    bool distanceToOrderGt(Point2D &p, Point2D &q) { return distanceSquaredTo(p) - distanceSquaredTo(q) < EPS; }

    /**
     * Compares two points by distance to this point. Greater than or equal to comparator.
     */
    bool distanceToOrderGe(Point2D &p, Point2D &q) { return distanceSquaredTo(p) - distanceSquaredTo(q) < -EPS; }
};

struct Point2D_hash {
    size_t operator ()(const Point2D &p) const {
        return 31 * hash<T> {}(p.x) + hash<T> {}(p.y);
    }
};

#endif /* DATASTRUCTURES_GEOMETRY_POINT2D_H_ */
