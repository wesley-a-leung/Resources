package algorithms.dp;

/**
 * Bitmask dynamic programming solution that finds the shortest Hamiltonian cycle.
 * Also known as the traveling salesman problem.
 * 
 * Take time proportional to <em>2</em><sup>N</sup><em>N</em><sup>2</sup> where
 * <em>N</em> is the number of vertices.
 */
public class HamiltonianCycle {
    private static int[] order;
    private double minDist;
    
    /**
     * Computes the shortest Hamiltonian cycle.
     * 
     * @param dist the 2D array of distances between vertices
     */
    public HamiltonianCycle(double[][] dist) {
        int n = dist.length;
        minDist = Double.POSITIVE_INFINITY;
        double[][] dp = new double[1 << n][n];
        order = new int[n];
        for (int i = 0; i < 1 << n; i++)
            for (int j = 0; j < n; j++)
                dp[i][j] = Double.POSITIVE_INFINITY;

        dp[1][0] = 0;

        for (int i = 1; i < 1 << n; i += 2)
            for (int j = 0; j < n; j++)
                if ((i & 1 << j) != 0)
                    for (int k = 1; k < n; k++)
                        if ((i & 1 << k) == 0)
                            dp[i ^ 1 << k][k] = Math.min(dp[i ^ 1 << k][k], dp[i][j] + dist[j][k]);

        for (int i = 1; i < n; i++)
            minDist = Math.min(minDist, dp[(1 << n) - 1][i] + dist[i][0]);

        // order
        int currPos = 0;
        int currState = (1 << n) - 1;
        for (int i = n - 1; i >= 0; i--) {
            int next = -1;
            for (int j = 0; j < n; j++)
                if ((currState & 1 << j) != 0 && (next == -1 || dp[currState][j] + dist[j][currPos] < dp[currState][next] + dist[next][currPos]))
                    next = j;
            order[i] = currPos = next;
            currState ^= 1 << currPos;
        }
    }
    
    /**
     * Returns the length of the shortest Hamiltonian cycle.
     * 
     * @return the length of the shortest Hamiltonian cycle
     */
    public double length() {
        return minDist;
    }
    
    /**
     * Returns the order of the vertex in the shortest Hamiltonian cycle.
     * 
     * @param v the vertex
     * @return the order of the vertex in the shortest Hamiltonian cycle
     */
    public int getOrder(int v) {
        return order[v];
    }
}
