package algorithms.dp;

public class MinimumCoinChange {
	// This class should not be instantiated
	private MinimumCoinChange() {}
	
	/**
	 * Static method to determine the minimum number of coins to make change
	 * for {@code N} dollars given the coin values specified in the
	 * {@code coins} array. Each coin can be used an infinite amount
	 * of times.
	 * 
	 * Take time proportional to <em>NM</em> where <em>N</em> is the
	 * amount of dollars and <em>M</em> is the number of different
	 * coin values.
	 * 
	 * Take space proportional to <em>N</em>.
	 * 
	 * @param N      dollars
	 * @param coins  array of coin values
	 * @return       the minimum number of coins to make change
	 */
	public static int solve(int N, int[] coins) {
		int[] dp = new int[N + 1];
		dp[0] = 0;
		for (int i = 1; i <= N; i++) {
			dp[i] = Integer.MAX_VALUE;
		}
		for (int i = 1; i <= N; i++) {
			for (int j = 0; j < coins.length; j++) {
				if (coins[j] <= i) {
					int prev = dp[i - coins[j]];
					if (prev != Integer.MAX_VALUE && prev + 1 < dp[i]) dp[i] = prev + 1;
				}
			}
		}
		return dp[N];
	}
}
