package algorithms.graph.shortestpath;

import datastructures.IndexMinPQ;
import datastructures.Stack;
import datastructures.graph.EdgeWeightedGraph;
import datastructures.graph.WeightedEdge;

/**
 *  The {@code DijkstraUndirectedSP} class represents a data type for solving
 *  the single-source shortest paths problem in edge-weighted graphs
 *  where the edge weights are nonnegative.
 *  <p>
 *  This implementation uses Dijkstra's algorithm with a binary heap.
 *  The constructor takes time proportional to <em>E</em> log <em>V</em>,
 *  where <em>V</em> is the number of vertices and <em>E</em> is the number of edges.
 *  Afterwards, the {@code distTo()} and {@code hasPathTo()} methods take
 *  constant time and the {@code pathTo()} method takes time proportional to the
 *  number of edges in the shortest path returned.
 *  <p>
 *  For additional documentation,    
 *  see <a href="http://algs4.cs.princeton.edu/44sp">Section 4.4</a> of    
 *  <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne. 
 *  See {@link DijkstraSP} for a version on edge-weighted digraphs.
 *
 *  @author Robert Sedgewick
 *  @author Kevin Wayne
 *  @author Nate Liu
 */
public class DijkstraUndirectedSP {
    private double[] distTo;          // distTo[v] = distance  of shortest s->v path
    private WeightedEdge[] edgeTo;            // edgeTo[v] = last edge on shortest s->v path
    private IndexMinPQ<Double> pq;    // priority queue of vertices

    /**
     * Computes a shortest-paths tree from the source vertex {@code s} to every
     * other vertex in the edge-weighted graph {@code G}.
     *
     * @param  G the edge-weighted digraph
     * @param  s the source vertex
     * @throws IllegalArgumentException if an edge weight is negative
     * @throws IllegalArgumentException unless {@code 0 <= s < V}
     */
    public DijkstraUndirectedSP(EdgeWeightedGraph G, int s) {
        for (WeightedEdge e : G.edges()) {
            if (e.weight() < 0)
                throw new IllegalArgumentException("edge " + e + " has negative weight");
        }

        distTo = new double[G.V()];
        edgeTo = new WeightedEdge[G.V()];

        validateVertex(s);

        for (int v = 0; v < G.V(); v++)
            distTo[v] = Double.POSITIVE_INFINITY;
        distTo[s] = 0.0;

        // relax vertices in order of distance from s
        pq = new IndexMinPQ<Double>(G.V());
        pq.insert(s, distTo[s]);
        while (!pq.isEmpty()) {
            int v = pq.delMin();
            for (WeightedEdge e : G.adj(v))
                relax(e, v);
        }

    }

    // relax edge e and update pq if changed
    private void relax(WeightedEdge e, int v) {
        int w = e.other(v);
        if (distTo[w] > distTo[v] + e.weight()) {
            distTo[w] = distTo[v] + e.weight();
            edgeTo[w] = e;
            if (pq.contains(w)) pq.decreaseKey(w, distTo[w]);
            else                pq.insert(w, distTo[w]);
        }
    }

    /**
     * Returns the length of a shortest path between the source vertex {@code s} and
     * vertex {@code v}.
     *
     * @param  v the destination vertex
     * @return the length of a shortest path between the source vertex {@code s} and
     *         the vertex {@code v}; {@code Double.POSITIVE_INFINITY} if no such path
     * @throws IllegalArgumentException unless {@code 0 <= v < V}
     */
    public double distTo(int v) {
        validateVertex(v);
        return distTo[v];
    }

    /**
     * Returns true if there is a path between the source vertex {@code s} and
     * vertex {@code v}.
     *
     * @param  v the destination vertex
     * @return {@code true} if there is a path between the source vertex
     *         {@code s} to vertex {@code v}; {@code false} otherwise
     * @throws IllegalArgumentException unless {@code 0 <= v < V}
     */
    public boolean hasPathTo(int v) {
        validateVertex(v);
        return distTo[v] < Double.POSITIVE_INFINITY;
    }

    /**
     * Returns a shortest path between the source vertex {@code s} and vertex {@code v}.
     *
     * @param  v the destination vertex
     * @return a shortest path between the source vertex {@code s} and vertex {@code v};
     *         {@code null} if no such path
     * @throws IllegalArgumentException unless {@code 0 <= v < V}
     */
    public Iterable<WeightedEdge> pathTo(int v) {
        validateVertex(v);
        if (!hasPathTo(v)) return null;
        Stack<WeightedEdge> path = new Stack<WeightedEdge>();
        int x = v;
        for (WeightedEdge e = edgeTo[v]; e != null; e = edgeTo[x]) {
            path.push(e);
            x = e.other(x);
        }
        return path;
    }


    // check optimality conditions:
    // (i) for all edges e = v-w:            distTo[w] <= distTo[v] + e.weight()
    // (ii) for all edge e = v-w on the SPT: distTo[w] == distTo[v] + e.weight()
    private boolean check(EdgeWeightedGraph G, int s) {

        // check that edge weights are nonnegative
        for (WeightedEdge e : G.edges()) {
            if (e.weight() < 0) {
                System.err.println("negative edge weight detected");
                return false;
            }
        }

        // check that distTo[v] and edgeTo[v] are consistent
        if (distTo[s] != 0.0 || edgeTo[s] != null) {
            System.err.println("distTo[s] and edgeTo[s] inconsistent");
            return false;
        }
        for (int v = 0; v < G.V(); v++) {
            if (v == s) continue;
            if (edgeTo[v] == null && distTo[v] != Double.POSITIVE_INFINITY) {
                System.err.println("distTo[] and edgeTo[] inconsistent");
                return false;
            }
        }

        // check that all edges e = v-w satisfy distTo[w] <= distTo[v] + e.weight()
        for (int v = 0; v < G.V(); v++) {
            for (WeightedEdge e : G.adj(v)) {
                int w = e.other(v);
                if (distTo[v] + e.weight() < distTo[w]) {
                    System.err.println("edge " + e + " not relaxed");
                    return false;
                }
            }
        }

        // check that all edges e = v-w on SPT satisfy distTo[w] == distTo[v] + e.weight()
        for (int w = 0; w < G.V(); w++) {
            if (edgeTo[w] == null) continue;
            WeightedEdge e = edgeTo[w];
            if (w != e.either() && w != e.other(e.either())) return false;
            int v = e.other(w);
            if (distTo[v] + e.weight() != distTo[w]) {
                System.err.println("edge " + e + " on shortest path not tight");
                return false;
            }
        }
        return true;
    }

    // throw an IllegalArgumentException unless {@code 0 <= v < V}
    private void validateVertex(int v) {
        int V = distTo.length;
        if (v < 0 || v >= V)
            throw new IllegalArgumentException("vertex " + v + " is not between 0 and " + (V-1));
    }
}