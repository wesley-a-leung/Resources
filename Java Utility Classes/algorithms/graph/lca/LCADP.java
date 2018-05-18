package algorithms.graph.lca;

import algorithms.graph.cycle.Cycle;
import datastructures.graph.Graph;

/**
 * The {@code LCADP} class represents a data structure for finding the
 * lowest common ancestor of 2 nodes in an acyclic graph using dynamic programming.
 * The constructor takes time proportional to V + E and afterwards,
 * the {@code lca()} method takes time proportional to log(V) to find the lowest common ancestor.
 *
 * @author Wesley Leung
 */
public class LCADP {
    private int V, LGV;
    private int[] depth;
    private int[][] parent;
    
    public LCADP(Graph G) {
        Cycle cycle = new Cycle(G);
        if (cycle.hasCycle()) throw new IllegalArgumentException("Graph is not acyclic.");
        V = G.V();
        LGV = (int) (Math.ceil(Math.log(V) / Math.log(2)) + 1);
        depth = new int[V];
        parent = new int[LGV][V];
        for (int i = 0; i < LGV; i++) {
            for (int j = 0; j < V; j++) {
                parent[i][j] = -1;
            }
        }
        dfs(G, 0, 0, -1);
        for (int i = 1; i < LGV; i++) {
            for (int j = 0; j < V; j++) {
                if (parent[i - 1][j] != -1) parent[i][j] = parent[i - 1][parent[i - 1][j]];
            }
        }
    }
    
    private void dfs(Graph G, int v, int d, int prev) {
        depth[v] = d;
        parent[0][v] = prev;
        for (int w : G.adj(v)) {
            if (w != prev) dfs(G, w, d + 1, v);
        }
    }
    
    /**
     * Returns the lowest common ancestor of vertex {@code v} and {@code w}.
     * 
     * @param v the first vertex
     * @param w the first vertex
     * @return the lowest common ancestor of vertex {@code v} and {@code w}
     */
    public int lca(int v, int w) {
        validateVertex(v);
        validateVertex(w);
        if (depth[v] < depth[w]) {
            int temp = v;
            v = w;
            w = temp;
        }
        for (int i = LGV - 1; i >= 0; i--) {
            if (parent[i][v] != -1 && depth[parent[i][v]] >= depth[w]) v = parent[i][v];
        }
        if (v == w) return v;
        for (int i = LGV - 1; i >= 0; i--) {
            if (parent[i][v] != parent[i][w]) {
                v = parent[i][v];
                w = parent[i][w];
            }
        }
        return parent[v][0];
    }
    
    private void validateVertex(int v) {
        int V = depth.length;
        if (v < 0 || v >= V)
            throw new IllegalArgumentException("vertex " + v + " is not between 0 and " + (V-1));
    }
}
