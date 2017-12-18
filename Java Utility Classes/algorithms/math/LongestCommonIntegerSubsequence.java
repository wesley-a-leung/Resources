package algorithms.math;

public class LongestCommonIntegerSubsequence {    
    private int[] subsequence;
    private int length;
    
    public LongestCommonIntegerSubsequence(int[] a, int[] b) {
        process(a, b);
    }
    
    private void process(int[] a, int[] b) {
        int[][] dp = new int[a.length + 1][b.length + 1];
        for (int i = 1; i <= a.length; i++)
            for (int j = 1; j <= b.length; j++) {
                if (a[i - 1] == b[j - 1])
                    dp[i][j] = 1 + dp[i - 1][j - 1];
                else
                    dp[i][j] = Math.max(dp[i - 1][j], dp[i][j - 1]);
            }
        this.length = dp[a.length][b.length];
        this.subsequence = new int[this.length];
        for (int i = a.length, j = b.length, k = length; i > 0 && j > 0;) {
            if (a[i - 1] == b[j - 1]) {
                this.subsequence[k--] = a[i - 1];
                i--;
                j--;
            } else if (dp[i - 1][j] > dp[i][j - 1]) {
                i--;
            } else {
                j--;
            }
        }
    }
    
    public int[] getSubsequence() {
        return this.subsequence;
    }
    
    public int getLength() {
        return this.length;
    }
}
