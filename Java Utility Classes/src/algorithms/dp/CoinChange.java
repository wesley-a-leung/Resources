package algorithms.dp;

public class CoinChange {
	
	// This class should not be instantiated
	private CoinChange() {}
	
	/**
	 * Static method to determine the number of ways to make change
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
	 * @return       the number of ways to make change
	 */
	public static int solve(int N, int[] coins) {
		int[] dp = new int[N + 1];
		for (int i = 0; i < coins.length; i++) {
			for (int j = 1; j <= N; j++) {
				if (coins[i] <= j) dp[j] += dp[j - coins[i]];
			}
		}
		return dp[N];
	}
}
