package algorithms.graph.components;

import datastructures.graph.AdjMatrixGraph;
import datastructures.graph.Graph;

/**
 * The Bron-Kerbosch algorithm is an algorithm for finding maximal cliques in an undirected graph using recursive backtracking.
 * A clique is defined to be a subset of a graph such that every pair of vertices in the subset are connected by an edge.
 * 
 * Time complexity: O(3^(n/3))
 * 
 * @author Wesley Leung
 *
 */
public class BronKerboschMaxClique {
    private boolean[][] adj;
    private int[] W;
    private int maxClique;
    
    public BronKerboschMaxClique(AdjMatrixGraph G) {
        adj = new boolean[G.V()][G.V()];
        W = new int[G.V()];
        for (int v = 0; v < G.V(); v++) {
            W[v] = 1;
            for (int w = 0; w < G.V(); w++) {
                adj[v][w] = G.hasEdge(v, w);
            }
        }
        maxClique = solve(G.V(), 0, (1 << G.V()) - 1, 0);
    }
    
    public BronKerboschMaxClique(Graph G) {
        adj = new boolean[G.V()][G.V()];
        W = new int[G.V()];
        for (int v = 0; v < G.V(); v++) {
            W[v] = 1;
            for (int w : G.adj(v)) {
                adj[v][w] = true;
            }
        }
        maxClique = solve(G.V(), 0, (1 << G.V()) - 1, 0);
    }
    
    public BronKerboschMaxClique(AdjMatrixGraph G, int[] weight) {
        adj = new boolean[G.V()][G.V()];
        W = new int[G.V()];
        for (int v = 0; v < G.V(); v++) {
            W[v] = weight[v];
            for (int w = 0; w < G.V(); w++) {
                adj[v][w] = G.hasEdge(v, w);
            }
        }
        maxClique = solve(G.V(), 0, (1 << G.V()) - 1, 0);
    }
    
    public BronKerboschMaxClique(Graph G, int[] weight) {
        adj = new boolean[G.V()][G.V()];
        W = new int[G.V()];
        for (int v = 0; v < G.V(); v++) {
            W[v] = weight[v];
            for (int w : G.adj(v)) {
                adj[v][w] = true;
            }
        }
        maxClique = solve(G.V(), 0, (1 << G.V()) - 1, 0);
    }
    
    public int maxClique() {
        return maxClique;
    }
    
    private int solve(int nodes, int curr, int pool, int excl) {
        if (pool == 0 && excl == 0) {
            int cnt = 0;
            for (int i = 0; i < nodes; i++)
                if ((curr & 1l << i) > 0) cnt += W[i];
            return cnt;
        }
        int res = 0;
        int j = 0;
        for (int i = 0; i < nodes; i++)
            if ((pool & 1l << i) > 0 || (excl & 1l << i) > 0) j = i;

        for (int i = 0; i < nodes; i++) {
            if ((pool & 1l << i) == 0 || adj[i][j]) continue;
            int ncurr = curr, npool = 0, nexcl = 0;
            ncurr |= 1l << i;

            for (int k = 0; k < nodes; k++) {
                if (adj[i][k]) {
                    npool |= pool & 1l << k;
                    nexcl |= excl & 1l << k;
                }
            }
            res = Math.max(res, solve(nodes, ncurr, npool, nexcl));

            pool &= ~(1l << i);
            excl |= 1 >> i;
        }
        return res;
    }
}