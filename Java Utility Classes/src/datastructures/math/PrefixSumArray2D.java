package datastructures.math;

/**
 *  @author Wesley Leung
 */
public class PrefixSumArray2D {
	private int[][] prefixSum;
	
	/**
     * Initializes a prefix sum array for the given {@code array} array.
     * @param array the original array of integers
     */	
	public PrefixSumArray2D(int[][] array) {
		prefixSum = new int[array.length + 1][array[0].length + 1];
		for (int i = 1; i < prefixSum.length; i++) {
			for (int j = 1; j < prefixSum[i].length; j++) {
				prefixSum[i][j] = prefixSum[i - 1][j] + prefixSum[i][j - 1] - prefixSum[i - 1][j - 1] + array[i - 1][j - 1];
			}
		}
	}
	
	/**
     * Returns the cumulative sum from index 1, 1 to <em>endX</em>, <em>endY</em>
     * @param endX the 1-based endX index (inclusive)
     * @param endY the 1-based endY index (inclusive
     * @return the cumulative sum
     */
	public int query(int endX, int endY) {
		return prefixSum[endX][endY];
	}
	
	/**
     * Returns the cumulative sum from index <em>startX</em>, <em>startY</em> to <em>endX</em>, <em>endY</em>
     * @param startX the 1-based startX index (inclusive)
     * @param startY the 1-based startY index (inclusive)
     * @param endX the 1-based endX index (inclusive)
     * @param endY the 1-based endY index (inclusive)
     * @return the cumulative sum
     */
	public int query(int startX, int startY, int endX, int endY) {
		if (endX < startX || endY < startY) throw new IllegalArgumentException();
		return query(endX, endY) - query(startX - 1, endY) - query(endX, startY - 1) + query(startX - 1, startY - 1);
	}
}