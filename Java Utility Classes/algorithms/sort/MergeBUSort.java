package algorithms.sort;
/**
 * The {@code MergeBUSort} class provides static methods for sorting an array
 * using bottom-up mergesort.
 * <p>
 * For additional documentation, see
 * <a href="http://algs4.cs.princeton.edu/21elementary">Section 2.1</a> of
 * <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne.
 *
 * @author Robert Sedgewick
 * @author Kevin Wayne
 * @author Wesley Leung
 */
public class MergeBUSort {
    private static final int CUTOFF = 7;  // cutoff to insertion sort

    // This class should not be instantiated.
    private MergeBUSort() {}

    // stably merge a[lo..mid] with a[mid+1..hi] using aux[lo..hi]
    private static void merge(Comparable[] a, Comparable[] aux, int lo, int mid, int hi) {
        // merge back to a[]
        int i = lo, j = mid + 1;
        for (int k = lo; k <= hi; k++) {
            if (i > mid) a[k] = aux[j++];
            else if (j > hi) a[k] = aux[i++];
            else if (less(aux[j], aux[i])) a[k] = aux[j++];
            else a[k] = aux[i++];
        }
    }

    /**
     * Rearranges the array in ascending order, using the natural order.
     * 
     * @param a the array to be sorted
     */
    public static void sort(Comparable[] a) {
        int n = a.length;
        Comparable[] aux = a.clone();
        boolean flag = true;
        int len = 1;
        for (; len <= CUTOFF && len < n; len *= 2) {
            for (int lo = 0; lo < n; lo += len + len) {
                int hi = Math.min(lo + len + len - 1, n - 1);
                insertionSort(aux, lo, hi);
            }
        }
        for (; len < n; len *= 2) {
            if (flag) {
                for (int lo = 0; lo < n; lo += len + len) {
                    int mid = lo + len - 1;
                    int hi = Math.min(lo + len + len - 1, n - 1);
                    if (mid + 1 < n && !less(aux[mid + 1], aux[mid])) System.arraycopy(aux, lo, a, lo, hi - lo + 1);
                    else merge(a, aux, lo, mid, hi);
                }
            } else {
                for (int lo = 0; lo < n; lo += len + len) {
                    int mid = lo + len - 1;
                    int hi = Math.min(lo + len + len - 1, n - 1);
                    if (mid + 1 < n && !less(a[mid + 1], a[mid])) System.arraycopy(a, lo, aux, lo, hi - lo + 1);
                    else merge(aux, a, lo, mid, hi);
                }
            }
            flag = !flag;
        }
        if (flag) System.arraycopy(aux, 0, a, 0, a.length);
        // assert isSorted(a);
    }
    
    // sort from a[lo] to a[hi] using insertion sort
    private static void insertionSort(Comparable[] a, int lo, int hi) {
        for (int i = lo; i <= hi; i++) {
            for (int j = i; j > lo && less(a[j], a[j-1]); j--) {
                exch(a, j, j-1);
            }
        }
    }

    /***********************************************************************
     * Helper sorting functions.
     ***************************************************************************/

    // is v < w ?
    private static boolean less(Comparable v, Comparable w) {
        return v.compareTo(w) < 0;
    }
    
    // exchange a[i] and a[j]
    private static void exch(Object[] a, int i, int j) {
        Object swap = a[i];
        a[i] = a[j];
        a[j] = swap;
    }

    /***************************************************************************
     * Check if array is sorted - useful for debugging.
     ***************************************************************************/
    private static boolean isSorted(Comparable[] a) {
        for (int i = 1; i < a.length; i++) {
            if (less(a[i], a[i - 1])) return false;
        }
        return true;
    }
    
    public static void main(String[] args) {
        Integer[] arr = {15, 2, 4, -1, 28, 2, 6, 4, 7, 9, 9, -4, 12, -13, 0, 18, 0};
        sort(arr);
        for (Integer x: arr) {
            System.out.print(x + " ");
        }
    }
}