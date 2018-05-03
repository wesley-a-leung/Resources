#ifndef ALGORITHMS_GEOMETRY_FARTHESTPAIR_H_
#define ALGORITHMS_GEOMETRY_FARTHESTPAIR_H_

#include <bits/stdc++.h>
#include "datastructures/geometry/Point2D.h"
#include "algorithms/geometry/GrahamScanConvexHull.h"
using namespace std;

/**
 *  The {@code FarthestPair} data type computes the farthest pair of points
 *  in a set of <em>n</em> points in the plane and provides accessor methods
 *  for getting the farthest pair of points and the distance between them.
 *  The distance between two points is their Euclidean distance.
 *  <p>
 *  This implementation computes the convex hull of the set of points and
 *  uses the rotating calipers method to find all antipodal point pairs
 *  and the farthest pair.
 *  It runs in O(<em>n</em> log <em>n</em>) time in the worst case and uses
 *  O(<em>N</em>) extra space.
 *  See also {@link ClosestPair} and {@link GrahamScan}.
 *  <p>
 *  For additional documentation, see <a href="http://algs4.cs.princeton.edu/99hull">Section 9.9</a> of
 *  <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne.
 *
 *  @author Robert Sedgewick
 *  @author Kevin Wayne
 */
typedef double T;
constexpr static T EPS = 1e-9;

class FarthestPair {
private:
    // farthest pair of points and distance
    Point2D best1, best2;
    double bestDistanceSquared = -numeric_limits<double>::infinity();

public:
    /**
     * Computes the farthest pair of points in the specified array of points.
     *
     * @param  n the length of the array
     * @param  points the array of points
     * @throws NullPointerException if {@code points} is {@code null} or if any
     *         entry in {@code points[]} is {@code null}
     */
    FarthestPair(int n, Point2D *points) {
        if (points == nullptr) throw invalid_argument("argument is null");
        GrahamScanConvexHull graham(n, points);

        // single point
        if (n <= 1) return;

        // number of points on the hull
        int m = (int) graham.hull().size();

        // the hull, in counterclockwise order hull[1] to hull[m]
        Point2D hull[m+1];
        m = 1;
        for (Point2D p : graham.hull()) hull[m++] = p;
        m--;

        // all points are equal
        if (m == 1) return;

        // points are collinear
        if (m == 2) {
            best1 = hull[1];
            best2 = hull[2];
            bestDistanceSquared = best1.distanceSquaredTo(best2);
            return;
        }

        // k = farthest vertex from edge from hull[1] to hull[m]
        int k = 2;
        while (Point2D::area2(hull[m], hull[1], hull[k+1]) > Point2D::area2(hull[m], hull[1], hull[k]) + EPS) k++;

        int j = k;
        for (int i = 1; i <= k && j <= m; i++) {
            if (hull[i].distanceSquaredTo(hull[j]) > bestDistanceSquared) {
                best1 = hull[i];
                best2 = hull[j];
                bestDistanceSquared = hull[i].distanceSquaredTo(hull[j]);
            }
            while ((j < m) && Point2D::area2(hull[i], hull[i+1], hull[j+1]) > Point2D::area2(hull[i], hull[i+1], hull[j]) + EPS) {
                j++;
                double distanceSquared = hull[i].distanceSquaredTo(hull[j]);
                if (distanceSquared > bestDistanceSquared) {
                    best1 = hull[i];
                    best2 = hull[j];
                    bestDistanceSquared = hull[i].distanceSquaredTo(hull[j]);
                }
            }
        }
    }

    /**
     * Returns one of the points in the farthest pair of points.
     *
     * @return one of the two points in the farthest pair of points;
     *         {@code null} if no such point (because there are fewer than 2 points)
     */
    Point2D either() {
        return best1;
    }

    /**
     * Returns the other point in the farthest pair of points.
     *
     * @return the other point in the farthest pair of points
     *         {@code null} if no such point (because there are fewer than 2 points)
     */
    Point2D other() {
        return best2;
    }

    /**
     * Returns the Euclidean distance between the farthest pair of points.
     * This quantity is also known as the <em>diameter</em> of the set of points.
     *
     * @return the Euclidean distance between the farthest pair of points
     *         {@code -numeric_limits<double>::infinity()} if no such pair of points
     *         exist (because there are fewer than 2 points)
     */
    double getDistance() {
        return sqrt(bestDistanceSquared);
    }
};

#endif /* ALGORITHMS_GEOMETRY_FARTHESTPAIR_H_ */
