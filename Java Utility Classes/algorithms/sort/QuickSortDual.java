package algorithms.sort;

/**
 * QuickSort with dual pivot.
 */
public class QuickSortDual {
	
	private static final int INSERTION_SORT_CUTOFF = 8;

    // quicksort the array a[] using dual-pivot quicksort
    public static void sort(Comparable[] a) {
        //Collections.shuffle(Arrays.asList(a));
        sort(a, 0, a.length - 1);
    }

    // quicksort the subarray a[lo .. hi] using dual-pivot quicksort
    private static void sort(Comparable[] a, int lo, int hi) { 
        if (hi <= lo) return;
        
     // cutoff to insertion sort
        if (hi - lo + 1 <= INSERTION_SORT_CUTOFF) {
            insertionSort(a, lo, hi);
            return;
        }

        // make sure a[lo] <= a[hi]
        if (less(a[hi], a[lo])) exch(a, lo, hi);

        int lt = lo + 1, gt = hi - 1;
        int i = lo + 1;
        while (i <= gt) {
            if       (less(a[i], a[lo])) exch(a, lt++, i++);
            else if  (less(a[hi], a[i])) exch(a, i, gt--);
            else                         i++;
        }
        exch(a, lo, --lt);
        exch(a, hi, ++gt);

        // recursively sort three subarrays
        sort(a, lo, lt-1);
        if (less(a[lt], a[gt])) sort(a, lt+1, gt-1);
        sort(a, gt+1, hi);
    }
    
    // sort from a[lo] to a[hi] using insertion sort
    private static void insertionSort(Comparable[] a, int lo, int hi) {
        for (int i = lo; i <= hi; i++)
            for (int j = i; j > lo && less(a[j], a[j-1]); j--)
                exch(a, j, j-1);
    }

   /***************************************************************************
    *  Helper sorting functions.
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
}