package algorithms.search;

import java.util.List;

import datastructures.Pair;

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
    
    /**
     * Returns a pair containing the start (inclusive) and end (exclusive) indices
     * of a given value.
     *
     * @param  a the array, must be sorted in ascending order
     * @param  key the search key
     * @return a pair containing the start (inclusive) and end (exclusive) indices
     * of a given value
     */
    public static Pair<Integer, Integer> equalRange(Comparable[] a, Comparable key) {
        return equalRange(a, 0, a.length, key);
    }
    
    /**
     * Returns a pair containing the start (inclusive) and end (exclusive) indices
     * of a given value.
     *
     * @param  a the array, must be sorted in ascending order
     * @param  start the inclusive start index
     * @param  end the exclusive end index
     * @param  key the search key
     * @return a pair containing the start (inclusive) and end (exclusive) indices
     * of a given value
     */
    public static Pair<Integer, Integer> equalRange(Comparable[] a, int start, int end, Comparable key) {
        return new Pair<Integer, Integer>(lowerBound(a, start, end, key), upperBound(a, start, end, key));
    }
    
    //*********************** LIST METHODS ********************************//
    
    /**
     * Returns the index of the specified key in the specified list.
     *
     * @param  a the list, must be sorted in ascending order
     * @param  start the inclusive start index
     * @return index of key in list {@code a} if present; {@code -1} otherwise
     */
    public static int indexOf(List<Comparable> a, Comparable key) {
        return indexOf(a, 0, a.size(), key);
    }
    
    /**
     * Returns the index of the specified key in the specified list between indices {@code start} (inclusive)
     * and {@code end} (exclusive).
     *
     * @param  a the list, must be sorted in ascending order
     * @param  start the inclusive start index
     * @param  end the exclusive end index
     * @param  key the search key
     * @return index of key in list {@code a} if present; {@code -1} otherwise
     */
    public static int indexOf(List<Comparable> a, int start, int end, Comparable key) {
        int lo = start;
        int hi = end - 1;
        while (lo <= hi) {
            // Key is in a[lo..hi] or not present.
            int mid = lo + (hi - lo) / 2;
            int cmp = key.compareTo(a.get(mid));
            if      (cmp < 0) hi = mid - 1;
            else if (cmp > 0) lo = mid + 1;
            else return mid;
        }
        return -1;
    }
    
    /**
     * Returns the index of the first element that is not less than the key 
     * in the specified list.
     *
     * @param  a the list, must be sorted in ascending order
     * @param  key the search key
     * @return index of first element not less than key in list {@code a};
     * {@code a.size()} if key is larger than the largest element
     */
    public static int lowerBound(List<Comparable> a, Comparable key) {
        return lowerBound(a, 0, a.size(), key);
    }
    
    /**
     * Returns the index of the first element that is not less than the key 
     * in the specified list between indices {@code start} (inclusive)
     * and {@code end} (exclusive).
     *
     * @param  a the list, must be sorted in ascending order
     * @param  start the inclusive start index
     * @param  end the exclusive end index
     * @param  key the search key
     * @return index of first element not less than key in list {@code a};
     * {@code end} if key is larger than the largest element
     */
    public static int lowerBound(List<Comparable> a, int start, int end, Comparable key) {
        int lo = start;
        int hi = end;
        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            int cmp = key.compareTo(a.get(mid));
            if (cmp <= 0) hi = mid;
            else lo = mid + 1;
        }
        return lo;
    }
    
    /**
     * Returns the index of the first element that is greater than the key 
     * in the specified list.
     *
     * @param  a the list, must be sorted in ascending order
     * @param  key the search key
     * @return index of first element greater than key in list {@code a};
     * {@code a.size()} if key is larger than or equal to the largest element
     */
    public static int upperBound(List<Comparable> a, Comparable key) {
        return upperBound(a, 0, a.size(), key);
    }
    
    /**
     * Returns the index of the first element that is greater than the key 
     * in the specified list between indices {@code start} (inclusive)
     * and {@code end} (exclusive).
     *
     * @param  a the list, must be sorted in ascending order
     * @param  start the inclusive start index
     * @param  end the exclusive end index
     * @param  key the search key
     * @return index of first element greater than key in list {@code a};
     * {@code end} if key is larger than or equal to the largest element
     */
    public static int upperBound(List<Comparable> a, int start, int end, Comparable key) {
        int lo = start;
        int hi = end;
        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            int cmp = key.compareTo(a.get(mid));
            if (cmp >= 0) lo = mid + 1;
            else hi = mid;
        }
        return lo;
    }
    
    /**
     * Returns a pair containing the start (inclusive) and end (exclusive) indices
     * of a given value.
     *
     * @param  a the array, must be sorted in ascending order
     * @param  key the search key
     * @return a pair containing the start (inclusive) and end (exclusive) indices
     * of a given value
     */
    public static Pair<Integer, Integer> equalRange(List<Comparable> a, Comparable key) {
        return equalRange(a, 0, a.size(), key);
    }
    
    /**
     * Returns a pair containing the start (inclusive) and end (exclusive) indices
     * of a given value.
     *
     * @param  a the array, must be sorted in ascending order
     * @param  start the inclusive start index
     * @param  end the exclusive end index
     * @param  key the search key
     * @return a pair containing the start (inclusive) and end (exclusive) indices
     * of a given value
     */
    public static Pair<Integer, Integer> equalRange(List<Comparable> a, int start, int end, Comparable key) {
        return new Pair<Integer, Integer>(lowerBound(a, start, end, key), upperBound(a, start, end, key));
    }
}
