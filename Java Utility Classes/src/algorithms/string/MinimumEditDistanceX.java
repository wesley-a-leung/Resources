package algorithms.string;

/**
 * Class that uses dynamic programming to compute the minimum
 * edit distance between 2 string in O(NM) time where N is the
 * length of the first string and M is the length of the second
 * string
 * 
 * This class uses optimized space of O(2M)
 * 
 * @author Wesley Leung
 */
public class MinimumEditDistanceX {
	
	// This class should not be instantiated
	private MinimumEditDistanceX() {}
	
	public static int minDistance(String word1, String word2) {
		int len1 = word1.length();
		int len2 = word2.length();
		
		int[][] dp = new int[2][len2 + 1];
		
		int s = 1;
		
		for (int i = 0; i <= len1; i++) {
			s = 1 - s;
			for (int j = 0; j <= len2; j++) {
				if (i == 0) {
					dp[s][j] = j;
				} else if (j == 0) {
					dp[s][j] = i;
				} else if (word1.charAt(i - 1) == word2.charAt(j - 1)) {
					dp[s][j] = dp[1 - s][j - 1];
				} else {
					int replace = dp[1 - s][j - 1] + 1;
					int insert = dp[1 - s][j] + 1;
					int delete = dp[s][j - 1] + 1;
	 
					int min = replace > insert ? insert : replace;
					min = delete > min ? min : delete;
					dp[s][j] = min;
				}
			}
		}
		return dp[s][len2];
	}
}

