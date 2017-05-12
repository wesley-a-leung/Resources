package algorithms.math;

public class MaximumNonConsecutiveSubsequence {
	
	private MaximumNonConsecutiveSubsequence() {}
	
	/** 
	 * Static method for determining the maximum value of a non consecutive subsequence
	 * (no 2 elements chosen may be next to each other.
	 * 
	 * Take time proportional to <em>N</em> where N is the length of the original
	 * sequence.
	 * 
	 * Take space proportional to <em>N</em>.
	 * 
	 * @param sequence the array of integers to check
	 * @return the maximum value of a non consecutive subsequence
	 */
	public static int solve(int[] sequence) {
		int[] dp = new int[sequence.length];	// the dp array that stores the solutions
		// 2 initial cases
		dp[0] = sequence[0]; // if there is only 1 value, then then that value is the solution
		dp[1] = Integer.max(dp[0], sequence[1]); // if there are 2 values, then the maximum of the 2 is the solution
		for (int i = 2; i < sequence.length; i++) { // we start at 2 since 0 and 1 have been solved
			dp[i] = Integer.max(dp[i-2] + sequence[i], dp[i-1]);	// the solutions is the maximum of either the value at index i
																	// plus the solution at i-2, but if the solution at i-1 is greater,
																	// then we should skip index i and use i-1 instead
		} // for i
		return dp[sequence.length-1];
	} // OptimiseDonationsDP method
}
