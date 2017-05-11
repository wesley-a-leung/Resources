package algorithms.string;

/**
 * Class that uses dynamic programming to compute the minimum
 * edit distance between 2 string in O(NM) time where N is the
 * length of the first string and M is the length of the second
 * string
 * 
 * It uses space of O(NM)
 * 
 * @author Wesley Leung
 */
public class MinimumEditDistance {
	
	// This class should not be instantiated
	private MinimumEditDistance() {}
	
	public static int minDistance(String word1, String word2) {
		int len1 = word1.length();
		int len2 = word2.length();
		
		// len1+1, len2+1, because finally return dp[len1][len2]
		int[][] dp = new int[len1 + 1][len2 + 1];
		
		for (int i = 0; i <= len1; i++) {
			dp[i][0] = i;
		}
		
		for (int j = 0; j <= len2; j++) {
			dp[0][j] = j;
		}
		
		//iterate though, and check last char
		for (int i = 1; i <= len1; i++) {
			char c1 = word1.charAt(i - 1);
			for (int j = 1; j <= len2; j++) {
				char c2 = word2.charAt(j - 1);
				
				//if last two chars equal
				if (c1 == c2) {
					//update dp value for +1 length
					dp[i][j] = dp[i - 1][j - 1];
				} else {
					int replace = dp[i - 1][j - 1] + 1;
					int insert = dp[i - 1][j] + 1;
					int delete = dp[i][j - 1] + 1;
					dp[i][j] = Math.min(replace, Math.min(insert, delete));
				}
			}
		}
		return dp[len1][len2];
	}
}

