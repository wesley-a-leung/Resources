package algorithms.string;

public class LongestCommonStringSubsequence {
    
    private String subsequence;
    private int length;
    
    public LongestCommonStringSubsequence(String s1, String s2) {
        process(s1, s2);
    }
    
    private void process(String s1, String s2) {
        int[][] dp = new int[s1.length() + 1][s2.length() + 1];
        for (int i = 1; i <= s1.length(); i++)
            for (int j = 1; j <= s2.length(); j++) {
                if (s1.charAt(i - 1) == s2.charAt(j - 1))
                    dp[i][j] = 1 + dp[i - 1][j - 1];
                else
                    dp[i][j] = Math.max(dp[i - 1][j], dp[i][j - 1]);
            }
        this.length = dp[s1.length()][s2.length()];
        StringBuilder ret = new StringBuilder("");
        for (int i = 1, j = 1; i <= s1.length() && j <= s2.length();) {
            if (s1.charAt(i - 1) == s2.charAt(j - 1)) {
                ret.append(s1.charAt(i - 1));
                i++;
                j++;
            } else if (dp[i - 1][j] > dp[i][j - 1]) {
                i++;
            } else {
                j++;
            }
        }
        this.subsequence = ret.toString();
    }
    
    public String getSubsequence() {
        return this.subsequence;
    }
    
    public int getLength() {
        return this.length;
    }
}
