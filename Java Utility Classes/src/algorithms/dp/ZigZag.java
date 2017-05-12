package algorithms.dp;

public class ZigZag {
	
	private ZigZag() {}
	
	/** 
	 * Static method for determining the length of the longest zigzag subsequence.
	 * A zigzag sequence alternates between increasing and decreasing (it can start
	 * with either).
	 * 
	 * Take time proportional to <em>N</em> where N is the length of the original
	 * sequence.
	 * 
	 * Take space proportional to 2<em>N</em>.
	 * 
	 * @param sequence the array of integers to check
	 * @return the length of the longest zigzag subsequence
	 */
	public static int solve(int[] sequence) {
		int[][] dp = new int[2][sequence.length];	// the 2d array that stores the solution of the maximum subsequence ending with index i
													// the array has a length of 2 because [0][i] stores the maximum solution where the
													// final difference is positive while [1][i] stores the maximum solution where the final
													// difference is negative
		dp[0][0] = 1; // the initial lengths must be at least 1 since a single number is considered a zig-zag sequence
		dp[1][0] = 1;
		for (int i = 1; i < sequence.length; i++) { // iterating through every number in the sequence to find the maximum subsequence ending with that index
			
			if (sequence[i] - sequence[i-1] > 0) { // positive difference
				dp[0][i] = Integer.max(dp[1][i-1] + 1, dp[0][i-1]);	// the length of the subsequence is the maximum of either the 
																	// negative solution + 1 (since signs need to alternate), or
																	// the current positive solution
				dp[1][i] = dp[1][i-1];	// the current negative solution remains the same
			} else if (sequence[i] - sequence[i-1] < 0) {	// negative difference
				dp[1][i] = Integer.max(dp[0][i-1] + 1, dp[1][i-1]);	// the length of the subsequence is the maximum of either the 
				 													// positive solution + 1 (since signs need to alternate), or 
																	// the current negative solution
				dp[0][i] = dp[0][i-1];								// the current positive solution remains the same
			} else { // in all other cases, the difference is 0, and it should not be included in the zig-zag sequence
				// both positive and negative solutions remain the same
				dp[0][i] = dp[0][i-1];
				dp[1][i] = dp[1][i-1];
			} // if else
		} // for i
		return Math.max(dp[0][sequence.length-1], dp[1][sequence.length-1]);	// the longest solution is the max subsequence that ends
																								// in the last index of either the positive or negative solution
	} // ZigZagDP method
}
