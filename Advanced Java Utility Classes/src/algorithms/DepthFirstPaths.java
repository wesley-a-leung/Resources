package algorithms;

/******************************************************************************
 *  Compilation:  javac DepthFirstPaths.java
 *  Execution:    java DepthFirstPaths G s
 *  Dependencies: Graph.java Stack.java StdOut.java
 *  Data files:   http://algs4.cs.princeton.edu/41graph/tinyCG.txt
 *                http://algs4.cs.princeton.edu/41graph/tinyG.txt
 *                http://algs4.cs.princeton.edu/41graph/mediumG.txt
 *                http://algs4.cs.princeton.edu/41graph/largeG.txt
 *
 *  Run depth first search on an undirected graph.
 *  Runs in O(E + V) time.
 *
 *  %  java Graph tinyCG.txt
 *  6 8
 *  0: 2 1 5 
 *  1: 0 2 
 *  2: 0 1 3 4 
 *  3: 5 4 2 
 *  4: 3 2 
 *  5: 3 0 
 *
 *  % java DepthFirstPaths tinyCG.txt 0
 *  0 to 0:  0
 *  0 to 1:  0-2-1
 *  0 to 2:  0-2
 *  0 to 3:  0-2-3
 *  0 to 4:  0-2-3-4
 *  0 to 5:  0-2-3-5
 *
 ******************************************************************************/

import datastructures.Graph;
import datastructures.Stack;

/**
 *  The {@code DepthFirstPaths} class represents a data type for finding
 *  paths from a source vertex <em>s</em> to every other vertex
 *  in an undirected graph.
 *  <p>
 *  This implementation uses depth-first search.
 *  The constructor takes time proportional to <em>V</em> + <em>E</em>,
 *  where <em>V</em> is the number of vertices and <em>E</em> is the number of edges.
 *  It uses extra space (not including the graph) proportional to <em>V</em>.
 *  <p>
 *  For additional documentation, see <a href="http://algs4.cs.princeton.edu/41graph">Section 4.1</a>   
 *  of <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne.
 *
 *  @author Robert Sedgewick
 *  @author Kevin Wayne
 */
public class DepthFirstPaths {
    private boolean[] marked;    // marked[v] = is there an s-v path?
    private int[] edgeTo;        // edgeTo[v] = last edge on s-v path
    private int[] depth;      // depth[v] = depth of vertex v
    private final int s;         // source vertex

    /**
     * Computes a path between {@code s} and every other vertex in graph {@code G}.
     * @param G the graph
     * @param s the source vertex
     * @throws IllegalArgumentException unless {@code 0 <= s < V}
     */
    public DepthFirstPaths(Graph G, int s) {
        this.s = s;
        edgeTo = new int[G.V()];
        depth = new int[G.V()];
        for (int v = 0; v < G.V(); v++)
            depth[v] = -1;
        marked = new boolean[G.V()];
        validateVertex(s);
        dfs(G, s, 0);
    }

    // depth first search from v
    private void dfs(Graph G, int v, int i) {
        marked[v] = true;
        depth[v] = i;
        for (int w : G.adj(v)) {
            if (!marked[w]) {
                edgeTo[w] = v;
                dfs(G, w, i+1);
            }
        }
    }

    /**
     * Is there a path between the source vertex {@code s} and vertex {@code v}?
     * @param v the vertex
     * @return {@code true} if there is a path, {@code false} otherwise
     * @throws IllegalArgumentException unless {@code 0 <= v < V}
     */
    public boolean hasPathTo(int v) {
        validateVertex(v);
        return marked[v];
    }
    
    /**
     * Returns the depth of vertex {@code v} from the source vertex {@code s}
     * @param v the vertex
     * @return the depth of a vertex
     * @throws IllegalArgumentException unless {@code 0 <= v < V}
     */
    public int depthOf(int v) {
        validateVertex(v);
        return depth[v];
    }

    /**
     * Returns a path between the source vertex {@code s} and vertex {@code v}, or
     * {@code null} if no such path.
     * @param  v the vertex
     * @return the sequence of vertices on a path between the source vertex
     *         {@code s} and vertex {@code v}, as an Iterable
     * @throws IllegalArgumentException unless {@code 0 <= v < V}
     */
    public Iterable<Integer> pathTo(int v) {
        validateVertex(v);
        if (!hasPathTo(v)) return null;
        Stack<Integer> path = new Stack<Integer>();
        for (int x = v; x != s; x = edgeTo[x])
            path.push(x);
        path.push(s);
        return path;
    }

    // throw an IllegalArgumentException unless {@code 0 <= v < V}
    private void validateVertex(int v) {
        int V = marked.length;
        if (v < 0 || v >= V)
            throw new IllegalArgumentException("vertex " + v + " is not between 0 and " + (V-1));
    }
}
