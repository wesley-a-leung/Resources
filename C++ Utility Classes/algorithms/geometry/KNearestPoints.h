#ifndef ALGORITHMS_GEOMETRY_KNEARESTPOINTS_H_
#define ALGORITHMS_GEOMETRY_KNEARESTPOINTS_H_

#include <bits/stdc++.h>
#include "datastructures/geometry/Point2D.h"
using namespace std;

/**
 *  {@code KNearestPoints} computes <em>K</em> nearest points by Euclidean distance
 *  to a starting point in <em>N</em> time.
 *  <p>
 *  The algorithm sorts the points using quicksort, but skips unnecessary sub arrays.
 *
 *  @author Wesley Leung
 */

class KNearestPoints {
private:
    static int partition(Point2D *copy, Point2D that, int start, int end) {
        Point2D pivot = copy[start];
        int i = start, j = end + 1;
        while (true) {
            while (i < end && copy[++i].distanceTo(that) - pivot.distanceTo(that) < 0);
            while (j > start && copy[--j].distanceTo(that) - pivot.distanceTo(that) > 0);
            if (i >= j) {
                break;
            }
            swap(copy[i], copy[j]);
        }
        swap(copy[start], copy[j]);
        return j;
    }

    /**
     * Helper method that does a partial quicksort to find the K closest points.
     *
     * @param n the length of the array
     * @param copy copy of array of points
     * @param that starting point
     * @param k number of closest points
     * @return List of K array of K closest points
     */
    static void quickSelect(int n, Point2D *copy, Point2D that, int k) {
        int start = 0, end = n - 1;
        while (start < end) {
            int p = partition(copy, that, start, end);
            if (p == k) {
                return;
            } else if (p < k) {
                start = p + 1;
            } else {
                end = p - 1;
            }
        }
    }

public:
    /**
     * Static method that computes the K closest points in the specified array of points.
     *
     * @param n the length of the array
     * @param points array of points
     * @param that starting point
     * @param k number of closest points
     * @return List of K array of K closest points
     */
    static vector<Point2D> nearest(int n, Point2D *points, Point2D that, int k) {
        vector<Point2D> ans;
        Point2D copy[n]; // to maintain order of points
        for (int i = 0; i < n; i++) {
            copy[i] = points[i];
        }
        quickSelect(n, copy, that, k);
        for (int i = 0; i < k; i++) {
            ans.push_back(copy[i]);
        }
        return ans;
    }
};

#endif /* ALGORITHMS_GEOMETRY_KNEARESTPOINTS_H_ */
