package datastructures.graph;

import java.util.Iterator;
import java.util.NoSuchElementException;

/**
 * The {@code AdjMatrixDigraph} class represents a digraph vertices named 0
 * through <em>V</em> - 1. It supports the following two primary operations: add
 * a directed edge to the digraph and iterate over all of edges incident from a
 * given vertex. It also provides methods for returning the number of vertices
 * <em>V</em> and the number of edges <em>E</em>. Parallel edges are disallowed;
 * self-loops are permitted.
 * <p>
 * This implementation uses an adjacency-matrix representation. All operations
 * take constant time (in the worst case) except iterating over the edges
 * incident from a given vertex, which takes time proportional to <em>V</em>.
 * 
 * @author Wesley Leung
 */
public class AdjMatrixDigraph {
    private static final String NEWLINE = System.getProperty("line.separator");

    private final int V;
    private int E;
    private boolean[][] adj;
    
    /**
     * Initializes an empty digraph with {@code V} vertices and 0 edges.
     * @param V the number of vertices
     * @throws IllegalArgumentException if {@code V < 0}
     */
    public AdjMatrixDigraph(int V) {
        if (V < 0) throw new IllegalArgumentException("number of vertices must be nonnegative");
        this.V = V;
        this.E = 0;
        this.adj = new boolean[V][V];
    }

    /**
     * Returns the number of vertices in the digraph.
     * @return the number of vertices in the digraph
     */
    public int V() {
        return V;
    }

    /**
     * Returns the number of edges in the digraph.
     * @return the number of edges in the digraph
     */
    public int E() {
        return E;
    }

    /**
     * Adds the edge to the digraph (if there is not already an edge with the same endpoints).
     * @param v the from vertex
     * @param w the to vertex
     */
    public void addEdge(int v, int w) {
        validateVertex(v);
        validateVertex(w);
        if (!adj[v][w]) {
            E++;
            adj[v][w] = true;
        }
    }
    
    /**
     * Returns whether there is an edge from v to w
     * 
     * @param v the from vertex
     * @param w the to vertex
     * @return true if there is an edge from v to w, false otherwise
     */
    public boolean hasEdge(int v, int w) {
        validateVertex(v);
        validateVertex(w);
        return adj[v][w];
    }

    /**
     * Returns the directed edges incident from vertex {@code v}.
     * @param v the vertex
     * @return the directed edges incident from vertex {@code v} as an Iterable
     * @throws IllegalArgumentException unless {@code 0 <= v < V}
     */
    public Iterable<Integer> adj(int v) {
        validateVertex(v);
        return new AdjIterator(v);
    }

    // support iteration over graph vertices
    private class AdjIterator implements Iterator<Integer>, Iterable<Integer> {
        private int v;
        private int w = 0;

        public AdjIterator(int v) {
            this.v = v;
        }

        public Iterator<Integer> iterator() {
            return this;
        }

        public boolean hasNext() {
            while (w < V) {
                if (adj[v][w]) return true;
                w++;
            }
            return false;
        }

        public Integer next() {
            if (!hasNext()) {
                throw new NoSuchElementException();
            }
            return w++;
        }

        public void remove() {
            throw new UnsupportedOperationException();
        }
    }

    /**
     * Returns a string representation of the digraph. This method takes
     * time proportional to <em>V</em><sup>2</sup>.
     * @return the number of vertices <em>V</em>, followed by the number of edges <em>E</em>,
     *   followed by the <em>V</em> adjacency lists of edges
     */
    public String toString() {
        StringBuilder s = new StringBuilder();
        s.append(V + " " + E + NEWLINE);
        for (int v = 0; v < V; v++) {
            s.append(v + ": ");
            for (int w: adj(v)) {
                s.append(w + "  ");
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