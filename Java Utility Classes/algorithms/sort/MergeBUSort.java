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
 */
public class MergeBUSort {

    // This class should not be instantiated.
    private MergeBUSort() {
    }

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
        for (int len = 1; len < n * 2; len *= 2) {
            if (flag) {
                for (int lo = 0; lo < n; lo += len + len) {
                    int mid = lo + len - 1;
                    int hi = Math.min(lo + len + len - 1, n - 1);
                    merge(a, aux, lo, mid, hi);
                }
            } else {
                for (int lo = 0; lo < n; lo += len + len) {
                    int mid = lo + len - 1;
                    int hi = Math.min(lo + len + len - 1, n - 1);
                    merge(aux, a, lo, mid, hi);
                }
            }
            flag = !flag;
        }
        // assert isSorted(a);
    }

    /***********************************************************************
     * Helper sorting functions.
     ***************************************************************************/

    // is v < w ?
    private static boolean less(Comparable v, Comparable w) {
        return v.compareTo(w) < 0;
    }

    /***************************************************************************
     * Check if array is sorted - useful for debugging.
     ***************************************************************************/
    private static boolean isSorted(Comparable[] a) {
        for (int i = 1; i < a.length; i++)
            if (less(a[i], a[i - 1])) return false;
        return true;
    }
    
    public static void main(String[] args) {
        Integer[] arr = {15, 2, 4, -1, 28, 2, 6, 4, 9, 9, 8, 7};
        sort(arr);
        for (Integer x: arr) {
            System.out.print(x + " ");
        }
    }
}