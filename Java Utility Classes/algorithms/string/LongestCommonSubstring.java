package algorithms.string;

public class LongestCommonSubstring {
	private String substring;
	private int length;
	
	public LongestCommonSubstring(String s1, String s2) {
		process(s1, s2);
	}
	
	private void process(String s1, String s2) {
		int[][] dp = new int[s1.length() + 1][s2.length() + 1];
		int start = 0, maxLen = 0;
		for (int i = 1; i <= s1.length(); i++) {
			for (int j = 1; j <= s2.length(); j++) {
				if (s1.charAt(i - 1) == s2.charAt(j - 1)) {
					dp[i][j] = 1 + dp[i - 1][j - 1];
					if (dp[i][j] > maxLen) {
						maxLen = dp[i][j];
						start = i - dp[i][j];
					}
				}
			}
		}
		this.length = maxLen;
		this.substring = s1.substring(start, start + maxLen);
	}
	
	public String substring() {
		return this.substring;
	}
	
	public int length() {
		return this.length;
	}
}
