package algorithms;

/**
 *  The {@code InterpolationSearch} class provides a static method for interpolation
 *  searching for an integer in a sorted array of integers.
 *  <p>
 *  The <em>indexOf</em> operations takes time of log log <em>n</em> in the average case 
 *  and <em>n</em> time in the worse case when the array is not uniformly distributed.
 *
 *  @author Wesley Leung
 */
public class InterpolationSearch {

    /**
     * This class should not be instantiated.
     */
    private InterpolationSearch() { }

    /**
     * Returns the index of the specified key in the specified array.
     *
     * @param  a the array of integers, must be sorted in ascending order
     * @param  key the search key
     * @return index of key in array {@code a} if present; {@code -1} otherwise
     */
    public static int indexOf(int[] a, int key) {
        int lo = 0;
        int hi = a.length - 1;
        while (lo <= hi) {
            // Key is in a[lo..hi] or not present.
            int guess = ((key - a[lo]) / (a[hi - lo])) * (hi - lo) + lo;
            if      (key < a[guess]) hi = guess - 1;
            else if (key > a[guess]) lo = guess + 1;
            else return guess;
        }
        return -1;
    }
}
