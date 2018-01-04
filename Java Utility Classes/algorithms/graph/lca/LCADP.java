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
        parent = new int[V][LGV];
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < LGV; j++) {
                parent[i][j] = -1;
            }
        }
        dfs(G, 0, 0, -1);
        for (int j = 1; j < LGV; j++) {
            for (int i = 0; i < V; i++) {
                if (parent[i][j - 1] != -1) parent[i][j] = parent[parent[i][j - 1]][j - 1];
            }
        }
    }
    
    private void dfs(Graph G, int v, int d, int prev) {
        depth[v] = d;
        parent[v][0] = prev;
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
            if (depth[parent[v][i]] >= depth[w]) v = parent[v][i];
        }
        if (v == w) return v;
        for (int i = LGV - 1; i >= 0; i--) {
            if (parent[v][i] != parent[w][i]) {
                v = parent[v][i];
                w = parent[w][i];
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
