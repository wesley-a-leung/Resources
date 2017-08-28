package algorithms.dp;

public class UniqueZigZag {
    
    private UniqueZigZag() {}
    
    /** 
     * Static method for determining the number of unique zigzag sequences of a specified length.
     * Since this number can be very large, it will be modulo by the number specified
     * A zigzag sequence alternates between increasing and decreasing (it can start
     * with either).
     * 
     * Takes time proportional to <em>N</em> where N is the length of the original
     * sequence.
     * 
     * Takes space proportional to 2<em>N</em>.
     * 
     * @param N the length of the zigzag sequences to count
     * @param MOD the number modulo the answer by
     * @return number of unique zigzag sequences
     */
    public static long solve(int N, long MOD) {
        long [][] dp = new long[2][N + 2];
        dp[1][1] = 1L;
        for (int i = 2; i <= N; i++) {
            for (int j = 1; j <= i; j++) {
                if (i % 2 == 0) {
                    dp[0][j] = (dp[1][j - 1] + dp[0][j - 1]) % MOD;
                    dp[0][j + 1] = dp[0][j];
                } else {
                    dp[1][j] = (dp[0][i] - dp[0][j - 1] + dp[1][j - 1] + MOD) % MOD;
                    dp[1][j + 1] = dp[1][j];
                }
            }
        }
        return dp[N % 2][N] % MOD;
    }
}
