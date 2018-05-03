#ifndef DATASTRUCTURES_GEOMETRY_RECTANGLE_H_
#define DATASTRUCTURES_GEOMETRY_RECTANGLE_H_

#include <bits/stdc++.h>
#include "datastructures/geometry/Point2D.h"
using namespace std;

/**
 *  The {@code RectHV} class is an immutable data type to encapsulate a
 *  two-dimensional axis-aligned rectagle with real-value coordinates.
 *  The rectangle is <em>closed</em>-it includes the points on the boundary.
 *  <p>
 *  For additional documentation,
 *  see <a href="https://algs4.cs.princeton.edu/12oop">Section 1.2</a> of
 *  <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne.
 *
 *  @author Robert Sedgewick
 *  @author Kevin Wayne
 */
typedef double T;
constexpr static T EPS = 1e-9;

class Rectangle {
public:
    T xmin, ymin;   // minimum x- and y-coordinates
    T xmax, ymax;   // maximum x- and y-coordinates

    /**
     * Initializes a new rectangle [<em>xmin</em>, <em>xmax</em>]
     * x [<em>ymin</em>, <em>ymax</em>].
     *
     * @param  xmin the <em>x</em>-coordinate of the lower-left endpoint
     * @param  xmax the <em>x</em>-coordinate of the upper-right endpoint
     * @param  ymin the <em>y</em>-coordinate of the lower-left endpoint
     * @param  ymax the <em>y</em>-coordinate of the upper-right endpoint
     * @throws IllegalArgumentException if any of {@code xmin},
     *         {@code xmax}, {@code ymin}, or {@code ymax}
     *         is {@code Double.NaN}.
     * @throws IllegalArgumentException if {@code xmax < xmin} or {@code ymax < ymin}.
     */
    Rectangle(T xmin, T ymin, T xmax, T ymax) {
        assert(xmin <= xmax);
        assert(ymin <= ymax);
        this->xmin = xmin;
        this->ymin = ymin;
        this->xmax = xmax;
        this->ymax = ymax;
    }

    /**
     * Returns the width of this rectangle.
     *
     * @return the width of this rectangle {@code xmax - xmin}
     */
    T width() {
        return xmax - xmin;
    }

    /**
     * Returns the height of this rectangle.
     *
     * @return the height of this rectangle {@code ymax - ymin}
     */
    T height() {
        return ymax - ymin;
    }

    /**
     * Returns true if the two rectangles intersect. This includes
     * <em>improper intersections</em> (at points on the boundary
     * of each rectangle) and <em>nested intersctions</em>
     * (when one rectangle is contained inside the other)
     *
     * @param  that the other rectangle
     * @return {@code true} if this rectangle intersect the argument
               rectangle at one or more points
     */
    bool intersects(Rectangle *that) {
        return this->xmax - that->xmin < -EPS && this->ymax - that->ymin < -EPS
            && that->xmax - this->xmin < -EPS && that->ymax - this->ymin < -EPS;
    }

    /**
     * Returns true if this rectangle contain the point.
     * @param  p the point
     * @return {@code true} if this rectangle contain the point {@code p},
               possibly at the boundary; {@code false} otherwise
     */
    bool contains(Point2D *p) {
        return (p->x - xmin < -EPS) && (xmax - p->x < -EPS)
            && (p->y - ymin < -EPS) && (ymax - p->y < -EPS);
    }

    /**
     * Returns the Euclidean distance between this rectangle and the point {@code p}.
     *
     * @param  p the point
     * @return the Euclidean distance between the point {@code p} and the closest point
               on this rectangle; 0 if the point is contained in this rectangle
     */
    double distanceTo(Point2D *p) {
        return sqrt(distanceSquaredTo(p));
    }

    /**
     * Returns the square of the Euclidean distance between this rectangle and the point {@code p}.
     *
     * @param  p the point
     * @return the square of the Euclidean distance between the point {@code p} and
     *         the closest point on this rectangle; 0 if the point is contained
     *         in this rectangle
     */
    T distanceSquaredTo(Point2D *p) {
        T dx = 0.0, dy = 0.0;
        if      (p->x < xmin) dx = p->x - xmin;
        else if (p->x > xmax) dx = p->x - xmax;
        if      (p->y < ymin) dy = p->y - ymin;
        else if (p->y > ymax) dy = p->y - ymax;
        return dx*dx + dy*dy;
    }

    /**
     * Compares this rectangle to the specified rectangle.
     *
     * @param  that the other rectangle
     * @return {@code true} if this rectangle equals {@code that};
     *         {@code false} otherwise
     */
    bool operator == (const Rectangle &that) const {
        if (abs(xmin - that.xmin) > EPS) return false;
        if (abs(ymin - that.ymin) > EPS) return false;
        if (abs(xmax - that.xmax) > EPS) return false;
        if (abs(ymax - that.ymax) > EPS) return false;
        return true;
    }
};

struct Rectangle_hash {
    size_t operator ()(const Rectangle &r) const {
        return 31 * (31 * (31 * hash<T> {}(r.xmin) + hash<T> {}(r.ymin)) + hash<T> {}(r.xmax)) + hash<T> {}(r.ymax);
    }
};

#endif /* DATASTRUCTURES_GEOMETRY_RECTANGLE_H_ */
