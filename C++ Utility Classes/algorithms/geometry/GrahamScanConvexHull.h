#ifndef ALGORITHMS_GEOMETRY_GRAHAMSCANCONVEXHULL_H_
#define ALGORITHMS_GEOMETRY_GRAHAMSCANCONVEXHULL_H_

#include <bits/stdc++.h>
#include "datastructures/geometry/Point2D.h"

using namespace std;
using namespace std::placeholders;

/**
 *  The {@code GrahamScan} data type provides methods for computing the
 *  convex hull of a set of <em>n</em> points in the plane.
 *  <p>
 *  The implementation uses the Graham-Scan convex hull algorithm.
 *  It runs in O(<em>n</em> log <em>n</em>) time in the worst case
 *  and uses O(<em>n</em>) extra memory.
 *  <p>
 *  For additional documentation, see <a href="http://algs4.cs.princeton.edu/99scientific">Section 9.9</a> of
 *  <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne.
 *
 *  @author Robert Sedgewick
 *  @author Kevin Wayne
 */
class GrahamScanConvexHull {
private:
    vector<Point2D> *hullVector;

public:
    /**
     * Computes the convex hull of the specified array of points.
     *
     * @param  n the length of the array
     * @param  points the array of points
     * @throws invalid_argument if {@code points} is {@code null}
     */
    GrahamScanConvexHull(int n, Point2D *points) {
        if (points == nullptr) throw invalid_argument("argument is null");
        stack<Point2D> hull;
        hullVector = new vector<Point2D>;

        // defensive copy
        Point2D a[n];
        for (int i = 0; i < n; i++) {
            a[i] = points[i];
         }

        // preprocess so that a[0] has lowest y-coordinate; break ties by x-coordinate
        // a[0] is an extreme point of the convex hull
        // (alternatively, could do easily in linear time)
        sort(a, a + n, Point2D::yOrderLt);


        // sort by polar angle with respect to base point a[0],
        // breaking ties by distance to a[0]
        sort(a, a + n, bind(&Point2D::polarOrderLt, a[0], _1, _2));

        hull.push(a[0]);       // a[0] is first extreme point

        // find index k1 of first point not equal to a[0]
        int k1;
        for (k1 = 1; k1 < n; k1++)
            if (a[0] != a[k1]) break;
        if (k1 == n) return;        // all points equal

        // find index k2 of first point not collinear with a[0] and a[k1]
        int k2;
        for (k2 = k1+1; k2 < n; k2++)
            if (Point2D::ccw(a[0], a[k1], a[k2]) != 0) break;
        hull.push(a[k2-1]);    // a[k2-1] is second extreme point

        // Graham scan; note that a[n-1] is extreme point different from a[0]
        for (int i = k2; i < n; i++) {
            Point2D top = hull.top();
            hull.pop();
            while (Point2D::ccw(hull.top(), top, a[i]) <= 0) {
                top = hull.top();
                hull.pop();
            }
            hull.push(top);
            hull.push(a[i]);
        }
        while (!hull.empty()) {
            hullVector->push_back(hull.top());
            hull.pop();
        }
    }

    /**
     * Returns the extreme points on the convex hull in counterclockwise order.
     *
     * @return the extreme points on the convex hull in counterclockwise order
     */
    vector<Point2D> &hull() {
        return *hullVector;
    }
};

#endif /* ALGORITHMS_GEOMETRY_GRAHAMSCANCONVEXHULL_H_ */
