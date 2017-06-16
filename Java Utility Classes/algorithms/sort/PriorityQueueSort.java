package algorithms.sort;

import datastructures.MinPQ;

/**
 * The {@code PriorityQueueSort} class provides a static methods for sorting
 * an array using a Minimum Priority Queue.
 * 
 * @author Wesley Leung
 *
 */
public class PriorityQueueSort {
    
    // This class should not be instantiated.
    private PriorityQueueSort() { }

    /**
     * Rearranges the array in ascending order, using the natural order.
     * @param a the array to be sorted
     */
    public static void sort(Comparable[] a) {
        MinPQ<Comparable> pq = new MinPQ<Comparable>();
        for (Comparable x: a) {
            pq.insert(x);
        }
        for (int i = 0; i < a.length; i++) {
            a[i] = pq.delMin();
        }
    }
}