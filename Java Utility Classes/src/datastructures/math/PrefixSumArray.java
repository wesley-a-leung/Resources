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
		prefixSum = new int[array.length];
		prefixSum[0] = array[0];
		for (int i = 1; i < prefixSum.length; i++) {
			prefixSum[i] = prefixSum[i - 1] + array[i];
		}
	}
	
	/**
     * Returns the cumulative sum from index 0 to <em>end</em>
     * @param end the 0-based end index (inclusive)
     * @return the cumulative sum
     */
	public int query(int end) {
		if (end < 0 || end >= prefixSum.length) throw new ArrayIndexOutOfBoundsException();
		return prefixSum[end];
	}
	
	/**
     * Returns the cumulative sum from index <em>start</em> to <em>end</em>
     * @param start the 0-based start index (inclusive)
     * @param end the 0-based end index (inclusive)
     * @return the cumulative sum
     */
	public int query(int start, int end) {
		if (start < 0 || end < 0 || start >= prefixSum.length || end >= prefixSum.length) throw new ArrayIndexOutOfBoundsException();
		if (end < start) throw new IllegalArgumentException();
		return query(end) - query(start - 1);
	}
}
