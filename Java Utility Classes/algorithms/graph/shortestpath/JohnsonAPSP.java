package algorithms.graph.shortestpath;

import java.util.ArrayList;

import algorithms.graph.cycle.DirectedWeightedCycle;
import datastructures.IndexMinPQ;
import datastructures.Stack;
import datastructures.graph.DirectedWeightedEdge;
import datastructures.graph.WeightedDigraph;

/**
 *  The {@code JohnsonAPSP} class represents a data type for solving the
 *  all-pairs shortest paths problem in edge-weighted digraphs with
 *  no negative cycles.
 *  The edge weights can be positive, negative, or zero.
 *  This class finds either a shortest path between every pair of vertices
 *  or a negative cycle.
 *  <p>
 *  This implementation uses the Johnson's algorithm by running the Bellman-Ford algorithm from
 *  a dummy node to detect negative cycles, reweighting the graph, then running Dijkstra's algorithm
 *  from each vertex, and finally, readjusting the weights.
 *  The constructor takes time proportional to <em>VE</em> log <em>V</em> + <em>VE</em>
 *  in the worst case, where <em>V</em> is the number of vertices.
 *  Afterwards, the {@code dist()}, {@code hasPath()}, and {@code hasNegativeCycle()}
 *  methods take constant time; the {@code path()} and {@code negativeCycle()}
 *  method takes time proportional to the number of edges returned.
 *  
 *  @author Wesley Leung
 */
public class JohnsonAPSP {
    private boolean hasNegativeCycle;  // is there a negative cycle?
    private double[][] distTo;         // distTo[v][w] = length of shortest v->w path
    private DirectedWeightedEdge[][] edgeTo;   // edgeTo[v][w] = last edge on shortest v->w path
    private IndexMinPQ<Double> pq;
    
    /**
     * Computes a shortest paths tree from each vertex to to every other vertex in
     * the edge-weighted digraph {@code G}. If no such shortest path exists for
     * some pair of vertices, it computes a negative cycle.
     * @param G the edge-weighted digraph
     */
    public JohnsonAPSP(WeightedDigraph G) {
        distTo = new double[G.V()][G.V()];
        edgeTo = new DirectedWeightedEdge[G.V()][G.V()];
        double[] h = new double[G.V() + 1];

        ArrayList<DirectedWeightedEdge> edges = G.edges();
        
        for (int v = 0; v < G.V(); v++) {
            h[v] = Double.POSITIVE_INFINITY;
            // add edge from dummy node to each node
            edges.add(new DirectedWeightedEdge(G.V(), v, 0.0));
            for (int w = 0; w < G.V(); w++) {
                distTo[v][w] = Double.POSITIVE_INFINITY;
            }
        }
        
        // bellman-ford from dummy node
        h[G.V()] = 0.0;
        for (int i = 0; i < G.V(); i++) {
            for (DirectedWeightedEdge e : edges) {
                int v = e.from();
                int w = e.to();
                if (h[w] > h[v] + e.weight()) {
                    h[w] = h[v] + e.weight();
                }
            }
        }
        
        // detect negative cycle
        for (DirectedWeightedEdge e : edges) {
            if (h[e.to()] > h[e.from()] + e.weight()) {
                hasNegativeCycle = true;
                return;
            }
        }
        
        // dijkstra from each node using the reweighted graph
        for (int s = 0; s < G.V(); s++) {
            pq = new IndexMinPQ<Double>(G.V());
            distTo[s][s] = 0.0;
            pq.insert(s, distTo[s][s]);
            while (!pq.isEmpty()) {
                int v = pq.delMin();
                for (DirectedWeightedEdge e : G.adj(v)) {
                    int w = e.to();
                    if (distTo[s][w] > distTo[s][v] + e.weight() + h[v] - h[w]) {
                        distTo[s][w] = distTo[s][v] + e.weight() + h[v] - h[w];
                        edgeTo[s][w] = e;
                        if (pq.contains(w)) pq.decreaseKey(w, distTo[s][w]);
                        else                pq.insert(w, distTo[s][w]);
                    }
                }
            }
        }
        
        // readjust weights
        for (int v = 0; v < G.V(); v++) {
            for (int w = 0; w < G.V(); w++) {
                distTo[v][w] = distTo[v][w] - h[v] + h[w];
            }
        }
    }
    
    /**
     * Is there a negative cycle?
     * @return {@code true} if there is a negative cycle, and {@code false} otherwise
     */
    public boolean hasNegativeCycle() {
        return hasNegativeCycle;
    }

    /**
     * Returns a negative cycle, or {@code null} if there is no such cycle.
     * @return a negative cycle as an iterable of edges,
     * or {@code null} if there is no such cycle
     */
    public Iterable<DirectedWeightedEdge> negativeCycle() {
        for (int v = 0; v < distTo.length; v++) {
            // negative cycle in v's predecessor graph
            if (distTo[v][v] < 0.0) {
                int V = edgeTo.length;
                WeightedDigraph spt = new WeightedDigraph(V);
                for (int w = 0; w < V; w++)
                    if (edgeTo[v][w] != null)
                        spt.addEdge(edgeTo[v][w]);
                DirectedWeightedCycle finder = new DirectedWeightedCycle(spt);
                assert finder.hasCycle();
                return finder.cycle();
            }
        }
        return null;
    }

    /**
     * Is there a path from the vertex {@code s} to vertex {@code t}?
     * @param  s the source vertex
     * @param  t the destination vertex
     * @return {@code true} if there is a path from vertex {@code s}
     *         to vertex {@code t}, and {@code false} otherwise
     * @throws IllegalArgumentException unless {@code 0 <= s < V}
     * @throws IllegalArgumentException unless {@code 0 <= t < V}
     */
    public boolean hasPath(int s, int t) {
        validateVertex(s);
        validateVertex(t);
        return distTo[s][t] < Double.POSITIVE_INFINITY;
    }

    /**
     * Returns the length of a shortest path from vertex {@code s} to vertex {@code t}.
     * @param  s the source vertex
     * @param  t the destination vertex
     * @return the length of a shortest path from vertex {@code s} to vertex {@code t};
     *         {@code Double.POSITIVE_INFINITY} if no such path
     * @throws UnsupportedOperationException if there is a negative cost cycle
     * @throws IllegalArgumentException unless {@code 0 <= v < V}
     */
    public double dist(int s, int t) {
        validateVertex(s);
        validateVertex(t);
        if (hasNegativeCycle())
            throw new UnsupportedOperationException("Negative cost cycle exists");
        return distTo[s][t];
    }

    /**
     * Returns a shortest path from vertex {@code s} to vertex {@code t}.
     * @param  s the source vertex
     * @param  t the destination vertex
     * @return a shortest path from vertex {@code s} to vertex {@code t}
     *         as an iterable of edges, and {@code null} if no such path
     * @throws UnsupportedOperationException if there is a negative cost cycle
     * @throws IllegalArgumentException unless {@code 0 <= v < V}
     */
    public Iterable<DirectedWeightedEdge> path(int s, int t) {
        validateVertex(s);
        validateVertex(t);
        if (hasNegativeCycle())
            throw new UnsupportedOperationException("Negative cost cycle exists");
        if (!hasPath(s, t)) return null;
        Stack<DirectedWeightedEdge> path = new Stack<DirectedWeightedEdge>();
        for (DirectedWeightedEdge e = edgeTo[s][t]; e != null; e = edgeTo[s][e.from()]) {
            path.push(e);
        }
        return path;
    }
    
    // throw an IllegalArgumentException unless {@code 0 <= v < V}
    private void validateVertex(int v) {
        int V = distTo.length;
        if (v < 0 || v >= V)
            throw new IllegalArgumentException("vertex " + v + " is not between 0 and " + (V-1));
    }
}
