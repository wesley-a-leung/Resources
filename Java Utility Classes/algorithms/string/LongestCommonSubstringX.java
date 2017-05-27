package algorithms.string;

public class LongestCommonSubstringX {
	private String substring;
	private int length;
	
	public LongestCommonSubstringX(String s1, String s2) {
		process(s1, s2);
	}
	
	private void process(String s1, String s2) {
		int[][] dp = new int[2][s2.length() + 1];
		int start = 0, maxLen = 0;
		for (int i = 1; i <= s1.length(); i++) {
			for (int j = 1; j <= s2.length(); j++) {
				if (s1.charAt(i - 1) == s2.charAt(j - 1)) {
					dp[i%2][j] = 1 + dp[1 - i%2][j - 1];
					if (dp[i%2][j] > maxLen) {
						maxLen = dp[i%2][j];
						start = i - dp[i%2][j];
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
