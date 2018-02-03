#ifndef ALGORITHMS_GEOMETRY_CLOSESTPAIR_H_
#define ALGORITHMS_GEOMETRY_CLOSESTPAIR_H_

#include <bits/stdc++.h>
#include "datastructures/geometry/Point2D.h"
using namespace std;

/**
 *  The {@code ClosestPair} data type computes a closest pair of points
 *  in a set of <em>n</em> points in the plane and provides accessor methods
 *  for getting the closest pair of points and the distance between them.
 *  The distance between two points is their Euclidean distance.
 *  <p>
 *  This implementation uses a divide-and-conquer algorithm.
 *  It runs in O(<em>n</em> log <em>n</em>) time in the worst case and uses
 *  O(<em>n</em>) extra space.
 *  <p>
 *  See also {@link FarthestPair}.
 *  <p>
 *  For additional documentation, see <a href="http://algs4.cs.princeton.edu/99hull">Section 9.9</a> of
 *  <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne.
 *
 *  @author Robert Sedgewick
 *  @author Kevin Wayne
 */
class ClosestPair {
private:
    // closest pair of points and their Euclidean distance
    Point2D best1, best2;
    double bestDistance = numeric_limits<double>::infinity();

    // stably merge a[lo .. mid] with a[mid+1 ..hi] using aux[lo .. hi]
    // precondition: a[lo .. mid] and a[mid+1 .. hi] are sorted subarrays
    void merge(Point2D *a, Point2D *aux, int lo, int mid, int hi) {
        // copy to aux[]
        for (int k = lo; k <= hi; k++) {
            aux[k] = a[k];
        }

        // merge back to a[]
        int i = lo, j = mid+1;
        for (int k = lo; k <= hi; k++) {
            if      (i > mid)              a[k] = aux[j++];
            else if (j > hi)               a[k] = aux[i++];
            else if (aux[j].compareTo(aux[i]) < 0) a[k] = aux[j++];
            else                           a[k] = aux[i++];
        }
    }

    // find closest pair of points in pointsByX[lo..hi]
    // precondition:  pointsByX[lo..hi] and pointsByY[lo..hi] are the same sequence of points
    // precondition:  pointsByX[lo..hi] sorted by x-coordinate
    // postcondition: pointsByY[lo..hi] sorted by y-coordinate
    double closest(Point2D *pointsByX, Point2D *pointsByY, Point2D *aux, int lo, int hi) {
        if (hi <= lo) return numeric_limits<double>::infinity();

        int mid = lo + (hi - lo) / 2;
        Point2D median = pointsByX[mid];

        // compute closest pair with both endpoints in left subarray or both in right subarray
        double delta1 = closest(pointsByX, pointsByY, aux, lo, mid);
        double delta2 = closest(pointsByX, pointsByY, aux, mid+1, hi);
        double delta = min(delta1, delta2);

        // merge back so that pointsByY[lo..hi] are sorted by y-coordinate
        merge(pointsByY, aux, lo, mid, hi);

        // aux[0..m-1] = sequence of points closer than delta, sorted by y-coordinate
        int m = 0;
        for (int i = lo; i <= hi; i++) {
            if (abs(pointsByY[i].x - median.x) < delta)
                aux[m++] = pointsByY[i];
        }

        // compare each point to its neighbors with y-coordinate closer than delta
        for (int i = 0; i < m; i++) {
            // a geometric packing argument shows that this loop iterates at most 7 times
            for (int j = i+1; (j < m) && (aux[j].y - aux[i].y < delta); j++) {
                double distance = aux[i].distanceTo(aux[j]);
                if (distance < delta) {
                    delta = distance;
                    if (distance < bestDistance) {
                        bestDistance = delta;
                        best1 = aux[i];
                        best2 = aux[j];
                    }
                }
            }
        }
        return delta;
    }

public:
    /**
     * Computes the closest pair of points in the specified array of points.
     *
     * @param  n the length of the array
     * @param  points the array of points
     * @throws NullPointerException if {@code points} is {@code null} or if any
     *         entry in {@code points[]} is {@code null}
     */
    ClosestPair(int n, Point2D *points) {
        if (points == nullptr) throw invalid_argument("argument is null");
        if (n <= 1) return;

        // sort by x-coordinate (breaking ties by y-coordinate)
        Point2D pointsByX[n];
        for (int i = 0; i < n; i++) pointsByX[i] = points[i];
        sort(pointsByX, pointsByX + n, Point2D::xOrderLt);

        // check for coincident points
        for (int i = 0; i < n-1; i++) {
            if (pointsByX[i] == pointsByX[i+1]) {
                bestDistance = 0.0;
                best1 = pointsByX[i];
                best2 = pointsByX[i+1];
                return;
            }
        }

        // sort by y-coordinate (but not yet sorted)
        Point2D pointsByY[n];
        for (int i = 0; i < n; i++) pointsByY[i] = pointsByX[i];

        // auxiliary array
        Point2D aux[n];

        closest(pointsByX, pointsByY, aux, 0, n-1);
    }

    /**
     * Returns one of the points in the closest pair of points.
     *
     * @return one of the two points in the closest pair of points;
     *         {@code null} if no such point (because there are fewer than 2 points)
     */
    Point2D either() {
        return best1;
    }

    /**
     * Returns the other point in the closest pair of points.
     *
     * @return the other point in the closest pair of points
     *         {@code null} if no such point (because there are fewer than 2 points)
     */
    Point2D other() {
        return best2;
    }

    /**
     * Returns the Euclidean distance between the closest pair of points.
     *
     * @return the Euclidean distance between the closest pair of points
     *         {@code numeric_limits<double>::infinity()} if no such pair of points
     *         exist (because there are fewer than 2 points)
     */
    double getDistance() {
        return bestDistance;
    }
};

#endif /* ALGORITHMS_GEOMETRY_CLOSESTPAIR_H_ */
