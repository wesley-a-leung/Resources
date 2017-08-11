package algorithms.dp;

/**
 * Bitmask dynamic programming solution that finds the shortest Hamiltonian path.
 * 
 * Take time proportional to <em>2</em><sup>N</sup><em>N</em><sup>2</sup> where
 * <em>N</em> is the number of vertices.
 */
public class HamiltonianPath {
    private static int[] order;
    private double minDist;
    
    /**
     * Computes the shortest Hamiltonian path.
     * 
     * @param dist the 2D array of distances between vertices
     */
    public HamiltonianPath(double[][] dist) {
        int n = dist.length;
        minDist = Double.POSITIVE_INFINITY;
        double[][] dp = new double[1 << n][n];
        order = new int[n];
        for (int i = 0; i < 1 << n; i++)
            for (int j = 0; j < n; j++)
                dp[i][j] = Double.POSITIVE_INFINITY;

        for (int i = 0; i < n; i++)
            dp[1 << i][i] = 0;

        for (int i = 1; i < 1 << n; i++)
            for (int j = 0; j < n; j++)
                if ((i & 1 << j) != 0)
                    for (int k = 0; k < n; k++)
                        if ((i & 1 << k) == 0)
                            dp[i ^ 1 << k][k] = Math.min(dp[i ^ 1 << k][k], dp[i][j] + dist[j][k]);

        for (int i = 0; i < n; i++)
            minDist = Math.min(minDist, dp[(1 << n) - 1][i]);

        // order
        int currState = (1 << n) - 1;
        int last = -1;
        for (int i = n - 1; i >= 0; i--) {
            int next = -1;
            for (int j = 0; j < n; j++)
                if ((currState & 1 << j) != 0 && (next == -1 || dp[currState][j] + (last == -1 ? 0 : dist[j][last]) < dp[currState][next] + (last == -1 ? 0 : dist[next][last])))
                    next = j;
            order[i] = last = next;
            currState ^= 1 << last;
        }
    }
    
    /**
     * Returns the length of the shortest Hamiltonian path.
     * 
     * @return the length of the shortest Hamiltonian path
     */
    public double length() {
        return minDist;
    }
    
    /**
     * Returns the order of the vertex in the shortest Hamiltonian path.
     * 
     * @param v the vertex
     * @return the order of the vertex in the shortest Hamiltonian path
     */
    public int getOrder(int v) {
        return order[v];
    }
}
