package algorithms.search;

/**
 *  The {@code QuickSelect} class provides static methods for selecting the
 *  Kth smallest element in an array
 *  <p>
 *  Best Case: <em>N</em>
 *  <p>
 *  Average Case: <em>N</em>
 *  <p>
 *  Worse Case: <em>N^2</em>
 *  <p>
 *  Space: 1
 *
 *  @author Wesley Leung
 */
public class QuickSelect {

    // This class should not be instantiated.
    private QuickSelect() {}
    
    /**
     * Returns the {@code K}th smallest element
     * 
     * @param a the array
     * @param k the index to find
     * @return the {@code K}th smallest element
     */
    public static Comparable select(Comparable[] a, int k) {
        return select(a, 0, a.length - 1, k);
    }

    /**
     * Returns the {@code K}th smallest element
     * 
     * @param a the array
     * @param start the start index
     * @param end the end index
     * @param k the index to find
     * @return the {@code K}th smallest element
     */
    public static Comparable select(Comparable[] a, int start, int end, int k) {
        if (start == end) {
            return a[start];
        }

        for (;;) {
            int pivotIndex = randomPivot(start, end);
            pivotIndex = partition(a, start, end, pivotIndex);

            if (k == pivotIndex) {
                return a[k];
            } else if (k < pivotIndex) {
                end = pivotIndex - 1;
            } else {
                start = pivotIndex + 1;
            }
        }
    }
    
    private static int partition(Comparable[] a, int start, int end, int pivotIndex) {
        Comparable pivotValue = a[pivotIndex];
        swap(a, pivotIndex, end);
        int storeIndex = start;
        for (int i = start; i < end; i++) {
            if (a[i].compareTo(pivotValue) < 0) {
                swap(a, storeIndex, i);
                storeIndex++;
            }
        }
        swap(a, end, storeIndex);
        return storeIndex;
    }

    private static void swap(Comparable[] a, int i, int j) {
        Comparable temp = a[i];
        a[i] = a[j];
        a[j] = temp;
    }
    
    private static int randomPivot(int start, int end) {
        return start + (int) Math.floor(Math.random() * (end - start + 1));
    }
}
