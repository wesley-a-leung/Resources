package algorithms.dp;

public class EggDropping {
	
	// This class should not be instantiated
	private EggDropping() {}
	
	/**
	 * Static method for computing the number of trials required to determine
	 * which floor out of {@code K} floors will an egg break, using {@code N}
	 * eggs, assuming eggs cannot be reused if broken.
	 * 
	 * Takes time proportional to <em>NK</em> where <em>N</em> is the number of 
	 * eggs and <em>K</em> is the number of floors
	 * 
	 * @param N Number of Eggs
	 * @param K Number of Floors
	 * @return The number of trials required
	 */
	public static int solve(int N, int K) {
		int[][] eggFloor = new int[N + 1][K + 1];
		int res;

		// We need one trial for one floor and 0 trials for 0 floors
		for (int i = 1; i <= N; i++) {
			eggFloor[i][0] = 0;
			eggFloor[i][1] = 1;
		}

		// We always need i trials for one egg and i floors.
		for (int i = 1; i <= K; i++)
			eggFloor[1][i] = i;

		for (int i = 2; i <= N; i++) {
			for (int j = 2; j <= K; j++) {
				eggFloor[i][j] = Integer.MAX_VALUE;
				for (int x = 1; x <= j; x++) {
					res = 1 + Math.max(eggFloor[i - 1][x - 1], eggFloor[i][j - x]);
					if (res < eggFloor[i][j])
						eggFloor[i][j] = res;
				}
			}
		}
		return eggFloor[N][K];
	}
}
