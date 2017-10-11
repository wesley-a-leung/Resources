package algorithms.dp;

public class ZeroOneKnapsack {
    // This class should not be instantiated
    private ZeroOneKnapsack() {}
    
    
    /**
     * Method for computing the maximum value that you can obtain from
     * putting {@code N} elements in a knapsack that can hold a maximum
     * of {@code M} weight, NOT allowing for repeated instances of items.
     *
     * Takes time proportional to <em>NM</em> where <em>N</em> is the number of
     * items and <em>M</em> is the size of the knapsack.
     *
     * Takes space proportional to <em>M</em>.
     *
     * @param M Size of the knapsack
     * @param weight Array of the weights of the items
     * @param value Array of the values of the items
     * @return the maximum value of the knapsack
     */
    public static int solve(int M, int[] weight, int[] value) {
        int N = weight.length;
        int[] dp = new int[M + 1];
        for (int i = 0; i < N; i++) {
            for (int j = M; j >= 0; j--) {
                if (weight[i] <= j) dp[j] = Math.max(dp[j], dp[j - weight[i]] + value[i]);
            }
        }
        return dp[M];
    }
}
