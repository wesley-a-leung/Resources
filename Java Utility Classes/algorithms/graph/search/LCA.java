package algorithms.graph.search;

import algorithms.graph.cycle.Cycle;
import datastructures.graph.Graph;

/**
 * The {@code LCA} class represents a data structure for finding the
 * lowest common ancestor of 2 nodes in an acyclic graph using heavy-light decomposition.
 * The constructor takes time proportional to <em>V</em> +  <em>E</em> and afterwards,
 * the {@code lca()} method takes log <em>V</em> time to find the lowest common ancestor.
 * 
 * @author Wesley Leung
 */
public class LCA {
    private int[] depth, parent, chain, size, head;
    private int chainNum;
    
    public LCA(Graph G) {
        Cycle cycle = new Cycle(G);
        if (cycle.hasCycle()) throw new IllegalArgumentException("Graph is not acyclic.");
        depth = new int[G.V()];
        parent = new int[G.V()];
        chain = new int[G.V()];
        size = new int[G.V()];
        head = new int[G.V()];
        for (int i = 0; i < G.V(); i++) {
            head[i] = -1;
        }
        dfs(G, 0, 0, -1);
        hld(G, 0, -1);
    }
    
    private void dfs(Graph G, int v, int d, int prev) {
        depth[v] = d;
        parent[v] = prev;
        size[v] = 1;
        for (int w: G.adj(v)) {
            if (w != prev) {
                dfs(G, w, d + 1, v);
                size[v] += size[w];
            }
        }
    }
    
    private void hld(Graph G, int v, int prev) {
        if (head[chainNum] == -1) head[chainNum] = v;
        chain[v] = chainNum;
        int maxIndex = -1;
        for (int w: G.adj(v)) {
            if (w != prev && (maxIndex == -1 || size[maxIndex] < size[w])) maxIndex = w;
        }
        if (maxIndex != -1) hld(G, maxIndex, v);
        for (int w: G.adj(v)) {
            if (w != prev && w != maxIndex) {
                chainNum++;
                hld(G, w, v);
            }
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
        while (chain[v] != chain[w]) {
            if (depth[head[chain[v]]] < depth[head[chain[w]]]) w = parent[head[chain[w]]];
            else v = parent[head[chain[v]]];
        }
        if (depth[v] < depth[w]) return v;
        return w;
    }
    
    private void validateVertex(int v) {
        int V = size.length;
        if (v < 0 || v >= V)
            throw new IllegalArgumentException("vertex " + v + " is not between 0 and " + (V-1));
    }
}
