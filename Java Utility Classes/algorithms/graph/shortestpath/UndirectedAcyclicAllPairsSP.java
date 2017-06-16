package algorithms.graph.shortestpath;

import algorithms.graph.search.WeightedLCA;
import datastructures.Queue;
import datastructures.graph.WeightedEdge;
import datastructures.graph.WeightedGraph;

/**
 * The {@code UndirectedAcyclicAllPairsSP} class represents a data type for solving
 * the all-pairs shortest paths problem in edge-weighted undirected acyclic
 * graphs. The edge weights can be positive, negative, or zero.
 * 
 * This implementation uses a lowest common ancestor algorithm that performs queries
 * in log <em>V</em> time.
 * 
 * The constructor takes time proportional to <em>V</em> + <em>E</em>. Afterwards, the
 * {@code dist()} takes time proportional to log <em>V</em>.
 * 
 * @author Wesley Leung
 *
 */
public class UndirectedAcyclicAllPairsSP {
    private double distTo[]; // stores the distance from an arbitrary root
    private WeightedLCA LCA;
    
    public UndirectedAcyclicAllPairsSP(WeightedGraph G) {
        distTo = new double[G.V()];
        LCA = new WeightedLCA(G); // will handle the illegal argument exception for acyclic graph
        bfs(G, 0);
    }
    
    private void bfs(WeightedGraph G, int s) {
        Queue<Integer> q = new Queue<Integer>();
        boolean[] marked = new boolean[G.V()];
        distTo[s] = 0;
        marked[s] = true;
        q.enqueue(s);

        while (!q.isEmpty()) {
            int v = q.dequeue();
            for (WeightedEdge e: G.adj(v)) {
                int w = e.other(v);
                if (!marked[w]) {
                    distTo[w] = distTo[v] + e.weight();
                    marked[w] = true;
                    q.enqueue(w);
                }
            }
        }
    }
    
    /**
     * Returns the length of a shortest path from vertex {@code s} to vertex {@code t}.
     * @param  s the source vertex
     * @param  t the destination vertex
     * @return the length of a shortest path from vertex {@code s} to vertex {@code t};
     * @throws IllegalArgumentException unless {@code 0 <= s < V}
     * @throws IllegalArgumentException unless {@code 0 <= t < V}
     */
    public double dist(int s, int t) {
        validateVertex(s);
        validateVertex(t);
        return distTo[s] + distTo[t] - 2 * distTo[LCA.lca(s, t)];
    }
    
    private void validateVertex(int v) {
        int V = distTo.length;
        if (v < 0 || v >= V)
            throw new IllegalArgumentException("vertex " + v + " is not between 0 and " + (V-1));
    }
}
