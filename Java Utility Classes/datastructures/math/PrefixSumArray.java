package datastructures.math;

/**
 *  @author Wesley Leung
 */
public class PrefixSumArray {
    private int[] prefixSum;
    
    /**
     * Initializes a prefix sum array for the given {@code array} array.
     * @param array the original array of integers
     */    
    public PrefixSumArray(int[] array) {
        prefixSum = new int[array.length + 1];
        for (int i = 1; i < prefixSum.length; i++) {
            prefixSum[i] = prefixSum[i - 1] + array[i - 1];
        }
    }
    
    /**
     * Returns the cumulative sum from index 1 to <em>end</em>
     * @param end the 1-based end index (inclusive)
     * @return the cumulative sum
     */
    public int query(int end) {
        return prefixSum[end];
    }
    
    /**
     * Returns the cumulative sum from index <em>start</em> to <em>end</em>
     * @param start the 1-based start index (inclusive)
     * @param end the 1-based end index (inclusive)
     * @return the cumulative sum
     */
    public int query(int start, int end) {
        if (end < start) throw new IllegalArgumentException();
        return query(end) - query(start - 1);
    }
}
