package algorithms.search;

import datastructures.Pair;

/******************************************************************************
 *  Compilation:  javac BinarySearch.java
 *  Execution:    java BinarySearch whitelist.txt < input.txt
 *  Dependencies: In.java StdIn.java StdOut.java
 *  Data files:   http://algs4.cs.princeton.edu/11model/tinyW.txt
 *                http://algs4.cs.princeton.edu/11model/tinyT.txt
 *                http://algs4.cs.princeton.edu/11model/largeW.txt
 *                http://algs4.cs.princeton.edu/11model/largeT.txt
 *
 *  % java BinarySearch tinyW.txt < tinyT.txt
 *  50
 *  99
 *  13
 *
 *  % java BinarySearch largeW.txt < largeT.txt | more
 *  499569
 *  984875
 *  295754
 *  207807
 *  140925
 *  161828
 *  [367,966 total values]
 *  
 ******************************************************************************/

/**
 *  The {@code BinarySearch} class provides a static method for binary
 *  searching for an element in a sorted array.
 *  <p>
 *  The <em>indexOf</em> operations takes logarithmic time in the worst case.
 *  <p>
 *  For additional documentation, see <a href="http://algs4.cs.princeton.edu/11model">Section 1.1</a> of
 *  <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne.
 *
 *  @author Robert Sedgewick
 *  @author Kevin Wayne
 */
public class BinarySearch {

    /**
     * This class should not be instantiated.
     */
    private BinarySearch() { }

    /**
     * Returns the index of the specified key in the specified array.
     *
     * @param  a the array, must be sorted in ascending order
     * @param  start the inclusive start index
     * @return index of key in array {@code a} if present; {@code -1} otherwise
     */
    public static int indexOf(Comparable[] a, Comparable key) {
    	return indexOf(a, 0, a.length, key);
    }
    
    /**
     * Returns the index of the specified key in the specified array between indices {@code start} (inclusive)
     * and {@code end} (exclusive).
     *
     * @param  a the array, must be sorted in ascending order
     * @param  start the inclusive start index
     * @param  end the exclusive end index
     * @param  key the search key
     * @return index of key in array {@code a} if present; {@code -1} otherwise
     */
    public static int indexOf(Comparable[] a, int start, int end, Comparable key) {
        int lo = start;
        int hi = end - 1;
        while (lo <= hi) {
            // Key is in a[lo..hi] or not present.
            int mid = lo + (hi - lo) / 2;
            int cmp = key.compareTo(a[mid]);
            if      (cmp < 0) hi = mid - 1;
            else if (cmp > 0) lo = mid + 1;
            else return mid;
        }
        return -1;
    }
    
    /**
     * Returns the index of the first element that is not less than the key 
     * in the specified array.
     *
     * @param  a the array, must be sorted in ascending order
     * @param  key the search key
     * @return index of first element not less than key in array {@code a};
     * {@code a.length} if key is larger than the largest element
     */
    public static int lowerBound(Comparable[] a, Comparable key) {
    	return lowerBound(a, 0, a.length, key);
    }
    
    /**
     * Returns the index of the first element that is not less than the key 
     * in the specified array between indices {@code start} (inclusive)
     * and {@code end} (exclusive).
     *
     * @param  a the array, must be sorted in ascending order
     * @param  start the inclusive start index
     * @param  end the exclusive end index
     * @param  key the search key
     * @return index of first element not less than key in array {@code a};
     * {@code end} if key is larger than the largest element
     */
    public static int lowerBound(Comparable[] a, int start, int end, Comparable key) {
    	int lo = start;
    	int hi = end;
    	while (lo < hi) {
    		int mid = lo + (hi - lo) / 2;
    		int cmp = key.compareTo(a[mid]);
    		if (cmp <= 0) hi = mid;
    		else lo = mid + 1;
    	}
    	return lo;
    }
    
    /**
     * Returns the index of the first element that is greater than the key 
     * in the specified array.
     *
     * @param  a the array, must be sorted in ascending order
     * @param  key the search key
     * @return index of first element greater than key in array {@code a};
     * {@code a.length} if key is larger than or equal to the largest element
     */
    public static int upperBound(Comparable[] a, Comparable key) {
    	return upperBound(a, 0, a.length, key);
    }
    
    /**
     * Returns the index of the first element that is greater than the key 
     * in the specified array between indices {@code start} (inclusive)
     * and {@code end} (exclusive).
     *
     * @param  a the array, must be sorted in ascending order
     * @param  start the inclusive start index
     * @param  end the exclusive end index
     * @param  key the search key
     * @return index of first element greater than key in array {@code a};
     * {@code end} if key is larger than or equal to the largest element
     */
    public static int upperBound(Comparable[] a, int start, int end, Comparable key) {
    	int lo = start;
    	int hi = end;
    	while (lo < hi) {
    		int mid = lo + (hi - lo) / 2;
    		int cmp = key.compareTo(a[mid]);
    		if (cmp >= 0) lo = mid + 1;
    		else hi = mid;
    	}
    	return lo;
    }
    
    public static Pair<Integer, Integer> equalRange(Comparable[] a, Comparable key) {
    	return equalRange(a, 0, a.length, key);
    }
    
    public static Pair<Integer, Integer> equalRange(Comparable[] a, int start, int end, Comparable key) {
    	return new Pair<Integer, Integer>(lowerBound(a, start, end, key), upperBound(a, start, end, key));
    }

}
