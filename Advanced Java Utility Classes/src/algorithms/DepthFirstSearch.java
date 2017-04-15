package algorithms;

import datastructures.Graph;

/******************************************************************************
 *  Compilation:  javac DepthFirstSearch.java
 *  Execution:    java DepthFirstSearch filename.txt s
 *  Dependencies: Graph.java StdOut.java
 *  Data files:   http://algs4.cs.princeton.edu/41graph/tinyG.txt
 *                http://algs4.cs.princeton.edu/41graph/mediumG.txt
 *
 *  Run depth first search on an undirected graph.
 *  Runs in O(E + V) time.
 *
 *  % java DepthFirstSearch tinyG.txt 0
 *  0 1 2 3 4 5 6 
 *  NOT connected
 *
 *  % java DepthFirstSearch tinyG.txt 9
 *  9 10 11 12 
 *  NOT connected
 *
 ******************************************************************************/

/**
 *  The {@code DepthFirstSearch} class represents a data type for 
 *  determining the vertices connected to a given source vertex <em>s</em>
 *  in an undirected graph. For versions that find the paths, see
 *  {@link DepthFirstPaths} and {@link BreadthFirstPaths}.
 *  <p>
 *  This implementation uses depth-first search.
 *  The constructor takes time proportional to <em>V</em> + <em>E</em>
 *  (in the worst case),
 *  where <em>V</em> is the number of vertices and <em>E</em> is the number of edges.
 *  It uses extra space (not including the graph) proportional to <em>V</em>.
 *  <p>
 *  For additional documentation, see <a href="http://algs4.cs.princeton.edu/41graph">Section 4.1</a>   
 *  of <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne.
 *
 *  @author Robert Sedgewick
 *  @author Kevin Wayne
 */
public class DepthFirstSearch {
    private boolean[] marked;    // marked[v] = is there an s-v path?
    private int count;           // number of vertices connected to s

    /**
     * Computes the vertices in graph {@code G} that are
     * connected to the source vertex {@code s}.
     * @param G the graph
     * @param s the source vertex
     * @throws IllegalArgumentException unless {@code 0 <= s < V}
     */
    public DepthFirstSearch(Graph G, int s) {
        marked = new boolean[G.V()];
        validateVertex(s);
        dfs(G, s);
    }

    // depth first search from v
    private void dfs(Graph G, int v) {
        count++;
        marked[v] = true;
        for (int w : G.adj(v)) {
            if (!marked[w]) {
                dfs(G, w);
            }
        }
    }

    /**
     * Is there a path between the source vertex {@code s} and vertex {@code v}?
     * @param v the vertex
     * @return {@code true} if there is a path, {@code false} otherwise
     * @throws IllegalArgumentException unless {@code 0 <= v < V}
     */
    public boolean marked(int v) {
        validateVertex(v);
        return marked[v];
    }

    /**
     * Returns the number of vertices connected to the source vertex {@code s}.
     * @return the number of vertices connected to the source vertex {@code s}
     */
    public int count() {
        return count;
    }

    // throw an IllegalArgumentException unless {@code 0 <= v < V}
    private void validateVertex(int v) {
        int V = marked.length;
        if (v < 0 || v >= V)
            throw new IllegalArgumentException("vertex " + v + " is not between 0 and " + (V-1));
    }
}
