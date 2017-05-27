package algorithms.geometry;

import java.util.ArrayList;
import java.util.List;

import datastructures.geometry.Point2D;

/**
 *  {@code KNearestPoints} computes <em>K</em> nearest points by Euclidean distance
 *  to a starting point in <em>N</em> time. 
 *  <p>
 *  The algorithm sorts the points using quicksort, but skips unnecessary sub arrays.
 *  
 *  @author Wesley Leung
 */

public class KNearestPoints {
	/**
     * This class should not be instantiated.
     */
	private KNearestPoints() {}
	
    /**
     * Static method that computes the K closest points in the specified array of points.
     *
     * @param points array of points
     * @param that starting point
     * @param k number of closest points
     * @return List of K array of K closest points
     */
	public static List<Point2D> nearest(Point2D[] points, final Point2D that, int k) {
	    List<Point2D> ans = new ArrayList<>();
	    Point2D[] copy = new Point2D[points.length]; // to maintain order of points
	    for (int i = 0; i < copy.length; i++) {
	        copy[i] = points[i];
	    }
	    quickSelect(copy, that, k);
	    for (int i = 0; i < k; i++) {
	        ans.add(copy[i]);
	    }
	    return ans;
	}
	
    /**
     * Helper method that does a partial quicksort to find the K closest points.
     *
     * @param copy copy of array of points
     * @param that starting point
     * @param k number of closest points
     * @return List of K array of K closest points
     */
	private static void quickSelect(Point2D[] copy, Point2D that, int k) {
	    int start = 0, end = copy.length - 1;
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
	private static int partition(Point2D[] copy, Point2D that, int start, int end) {
	    Point2D pivot = copy[start];
	    int i = start, j = end + 1;
	    while (true) {
	        while (i < end && copy[++i].distanceTo(that) - pivot.distanceTo(that) < 0);
	        while (j > start && copy[--j].distanceTo(that) - pivot.distanceTo(that) > 0);
	        if (i >= j) {
	            break;
	        }
	        swap(copy, i, j);
	    }
	    swap(copy, start, j);
	    return j;
	}

	private static void swap(Point2D[] copy, int i, int j) {
	    Point2D tmp = copy[i];
	    copy[i] = copy[j];
	    copy[j] = tmp;
	}
}
