package datastructures.graph;

import java.util.Iterator;
import java.util.NoSuchElementException;

/**
 *  The {@code AdjMatrixWeightedgraph} class represents a edge-weighted
 *  graph of vertices named 0 through <em>V</em> - 1, where each
 *   edge is of type {@link WeightedEdge} and has a real-valued weight.
 *  It supports the following two primary operations: add a  edge
 *  to the graph and iterate over all of edges incident from a given vertex.
 *  It also provides
 *  methods for returning the number of vertices <em>V</em> and the number
 *  of edges <em>E</em>. Parallel edges are disallowed; self-loops are permitted.
 *  <p>
 *  This implementation uses an adjacency-matrix representation.
 *  All operations take constant time (in the worst case) except
 *  iterating over the edges incident from a given vertex, which takes
 *  time proportional to <em>V</em>.
 *  <p>
 *  For additional documentation,
 *  see <a href="http://algs4.cs.princeton.edu/44sp">Section 4.4</a> of
 *  <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne.
 *
 *  @author Robert Sedgewick
 *  @author Kevin Wayne
 */
public class AdjMatrixWeightedGraph {
    private static final String NEWLINE = System.getProperty("line.separator");

    private final int V;
    private int E;
    private WeightedEdge[][] adj;
    
    /**
     * Initializes an empty edge-weighted graph with {@code V} vertices and 0 edges.
     * @param V the number of vertices
     * @throws IllegalArgumentException if {@code V < 0}
     */
    public AdjMatrixWeightedGraph(int V) {
        if (V < 0) throw new IllegalArgumentException("number of vertices must be nonnegative");
        this.V = V;
        this.E = 0;
        this.adj = new WeightedEdge[V][V];
    }

    /**
     * Returns the number of vertices in the edge-weighted graph.
     * @return the number of vertices in the edge-weighted graph
     */
    public int V() {
        return V;
    }

    /**
     * Returns the number of edges in the edge-weighted graph.
     * @return the number of edges in the edge-weighted graph
     */
    public int E() {
        return E;
    }

    /**
     * Adds the  edge {@code e} to the edge-weighted graph (if there
     * is not already an edge with the same endpoints).
     * @param e the edge
     */
    public void addEdge(WeightedEdge e) {
        int v = e.either();
        int w = e.other(v);
        validateVertex(v);
        validateVertex(w);
        if (adj[v][w] == null) {
            E++;
            adj[v][w] = e;
            adj[w][v] = e;
        }
    }
    
    /**
     * Returns the weighted edge between v and w
     * 
     * @param v the first vertex
     * @param w second vertex
     * @return the weighted edge between v and w, null if there is no edge
     */
    public WeightedEdge getEdge(int v, int w) {
        validateVertex(v);
        validateVertex(w);
        return adj[v][w];
    }

    /**
     * Returns the  edges incident from vertex {@code v}.
     * @param v the vertex
     * @return the  edges incident from vertex {@code v} as an Iterable
     * @throws IllegalArgumentException unless {@code 0 <= v < V}
     */
    public Iterable<WeightedEdge> adj(int v) {
        validateVertex(v);
        return new AdjIterator(v);
    }

    // support iteration over graph vertices
    private class AdjIterator implements Iterator<WeightedEdge>, Iterable<WeightedEdge> {
        private int v;
        private int w = 0;

        public AdjIterator(int v) {
            this.v = v;
        }

        public Iterator<WeightedEdge> iterator() {
            return this;
        }

        public boolean hasNext() {
            while (w < V) {
                if (adj[v][w] != null) return true;
                w++;
            }
            return false;
        }

        public WeightedEdge next() {
            if (!hasNext()) {
                throw new NoSuchElementException();
            }
            return adj[v][w++];
        }

        public void remove() {
            throw new UnsupportedOperationException();
        }
    }

    /**
     * Returns a string representation of the edge-weighted graph. This method takes
     * time proportional to <em>V</em><sup>2</sup>.
     * @return the number of vertices <em>V</em>, followed by the number of edges <em>E</em>,
     *   followed by the <em>V</em> adjacency lists of edges
     */
    public String toString() {
        StringBuilder s = new StringBuilder();
        s.append(V + " " + E + NEWLINE);
        for (int v = 0; v < V; v++) {
            s.append(v + ": ");
            for (WeightedEdge e : adj(v)) {
                s.append(e + "  ");
            }
            s.append(NEWLINE);
        }
        return s.toString();
    }

    // throw an IllegalArgumentException unless {@code 0 <= v < V}
    private void validateVertex(int v) {
        if (v < 0 || v >= V)
            throw new IllegalArgumentException("vertex " + v + " is not between 0 and " + (V-1));
    }
}