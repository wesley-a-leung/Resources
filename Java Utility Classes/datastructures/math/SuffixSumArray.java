package datastructures.math;

/**
 *  @author Wesley Leung
 */
public class SuffixSumArray {
	private int[] suffixSum;
	
	/**
     * Initializes a suffix sum array for the given {@code array} array.
     * @param array the original array of integers
     */	
	public SuffixSumArray(int[] array) {
		suffixSum = new int[array.length];
		suffixSum[array.length - 1] = array[array.length - 1];
		for (int i = suffixSum.length - 2; i >= 0; i--) {
			suffixSum[i] = suffixSum[i + 1] + array[i];
		}
	}
	
	/**
     * Returns the cumulative sum from index 0 to <em>end</em>
     * @param end the 0-based start index (inclusive)
     * @return the cumulative sum
     */
	public int query(int start) {
		if (start < 0 || start >= suffixSum.length) throw new ArrayIndexOutOfBoundsException();
		return suffixSum[start];
	}
	
	/**
     * Returns the cumulative sum from index <em>start</em> to <em>end</em>
     * @param start the 0-based start index (inclusive)
     * @param end the 0-based end index (inclusive)
     * @return the cumulative sum
     */
	public int query(int start, int end) {
		if (start < 0 || end < 0 || start >= suffixSum.length || end >= suffixSum.length) throw new ArrayIndexOutOfBoundsException();
		if (end < start) throw new IllegalArgumentException();
		return query(start) - query(end + 1);
	}
}
