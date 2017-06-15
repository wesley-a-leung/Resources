package algorithms.math;

import java.util.Arrays;

public class LongestIncreasingSubsequence {
	
	private LongestIncreasingSubsequence() {}
	
	/** 
	 * Static method for determining the length of the longest increasing subsequence (allowing for skips)
	 * 
	 * Takes time proportional to <em>N</em> log <em>N</em> where N is the length of the sequence.
	 * 
	 * Takes space proportional to <em>N</em>.
	 * 
	 * @param sequence the array of integers to check
	 * @return the length of the longest increasing subsequence
	 */
	public static int solve(int[] sequence) {
		int[] dp = new int[sequence.length];
		int len = 0;
		for (int x: sequence) {
			int i = Arrays.binarySearch(dp, 0, len, x);
			if (i < 0) i = -(i + 1);
			dp[i] = x;
			if (i == len) len++;
		}
		return len;
	}

}
