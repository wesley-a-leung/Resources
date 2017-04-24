package algorithms.graph.cycle;

import datastructures.Queue;
import datastructures.Stack;
import datastructures.graph.Graph;
import algorithms.graph.search.BreadthFirstPaths;

/******************************************************************************
 *  Compilation:  javac EulerianPath.java
 *  Execution:    java EulerianPath V E
 *  Dependencies: Graph.java Stack.java StdOut.java
 *
 *  Find an Eulerian path in a graph, if one exists.
 *
 ******************************************************************************/

/**
 *  The {@code EulerianPath} class represents a data type
 *  for finding an Eulerian path in a graph.
 *  An <em>Eulerian path</em> is a path (not necessarily simple) that
 *  uses every edge in the graph exactly once.
 *  <p>
 *  This implementation uses a nonrecursive depth-first search.
 *  The constructor runs in O(<em>E</em> + <em>V</em>) time,
 *  and uses O(<em>E</em> + <em>V</em>) extra space,
 *  where <em>E</em> is the number of edges and <em>V</em> the number of vertices
 *  All other methods take O(1) time.
 *  <p>
 *  To compute Eulerian cycles in graphs, see {@link EulerianCycle}.
 *  To compute Eulerian cycles and paths in digraphs, see
 *  {@link DirectedEulerianCycle} and {@link DirectedEulerianPath}.
 *  <p>
 *  For additional documentation,
 *  see <a href="http://algs4.cs.princeton.edu/41graph">Section 4.1</a> of
 *  <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne.
 * 
 * @author Robert Sedgewick
 * @author Kevin Wayne
 * @author Nate Liu
 */
public class EulerianPath {
    private Stack<Integer> path = null;   // Eulerian path; null if no suh path

    // an undirected edge, with a field to indicate whether the edge has already been used
    private static class Edge {
        private final int v;
        private final int w;
        private boolean isUsed;

        public Edge(int v, int w) {
            this.v = v;
            this.w = w;
            isUsed = false;
        }

        // returns the other vertex of the edge
        public int other(int vertex) {
            if      (vertex == v) return w;
            else if (vertex == w) return v;
            else throw new IllegalArgumentException("Illegal endpoint");
        }
    }

    /**
     * Computes an Eulerian path in the specified graph, if one exists.
     * 
     * @param G the graph
     */
    public EulerianPath(Graph G) {

        // find vertex from which to start potential Eulerian path:
        // a vertex v with odd degree(v) if it exits;
        // otherwise a vertex with degree(v) > 0
        int oddDegreeVertices = 0;
        int s = nonIsolatedVertex(G);
        for (int v = 0; v < G.V(); v++) {
            if (G.degree(v) % 2 != 0) {
                oddDegreeVertices++;
                s = v;
            }
        }

        // graph can't have an Eulerian path
        // (this condition is needed for correctness)
        if (oddDegreeVertices > 2) return;

        // special case for graph with zero edges (has a degenerate Eulerian path)
        if (s == -1) s = 0;

        // create local view of adjacency lists, to iterate one vertex at a time
        // the helper Edge data type is used to avoid exploring both copies of an edge v-w
        Queue<Edge>[] adj = (Queue<Edge>[]) new Queue[G.V()];
        for (int v = 0; v < G.V(); v++)
            adj[v] = new Queue<Edge>();

        for (int v = 0; v < G.V(); v++) {
            int selfLoops = 0;
            for (int w : G.adj(v)) {
                // careful with self loops
                if (v == w) {
                    if (selfLoops % 2 == 0) {
                        Edge e = new Edge(v, w);
                        adj[v].enqueue(e);
                        adj[w].enqueue(e);
                    }
                    selfLoops++;
                }
                else if (v < w) {
                    Edge e = new Edge(v, w);
                    adj[v].enqueue(e);
                    adj[w].enqueue(e);
                }
            }
        }

        // initialize stack with any non-isolated vertex
        Stack<Integer> stack = new Stack<Integer>();
        stack.push(s);

        // greedily search through edges in iterative DFS style
        path = new Stack<Integer>();
        while (!stack.isEmpty()) {
            int v = stack.pop();
            while (!adj[v].isEmpty()) {
                Edge edge = adj[v].dequeue();
                if (edge.isUsed) continue;
                edge.isUsed = true;
                stack.push(v);
                v = edge.other(v);
            }
            // push vertex with no more leaving edges to path
            path.push(v);
        }

        // check if all edges are used
        if (path.size() != G.E() + 1)
            path = null;

        assert certifySolution(G);
    }

    /**
     * Returns the sequence of vertices on an Eulerian path.
     * 
     * @return the sequence of vertices on an Eulerian path;
     *         {@code null} if no such path
     */
    public Iterable<Integer> path() {
        return path;
    }

    /**
     * Returns true if the graph has an Eulerian path.
     * 
     * @return {@code true} if the graph has an Eulerian path;
     *         {@code false} otherwise
     */
    public boolean hasEulerianPath() {
        return path != null;
    }


    // returns any non-isolated vertex; -1 if no such vertex
    private static int nonIsolatedVertex(Graph G) {
        for (int v = 0; v < G.V(); v++)
            if (G.degree(v) > 0)
                return v;
        return -1;
    }


    /**************************************************************************
     *
     *  The code below is solely for testing correctness of the data type.
     *
     **************************************************************************/

    // Determines whether a graph has an Eulerian path using necessary
    // and sufficient conditions (without computing the path itself):
    //    - degree(v) is even for every vertex, except for possibly two
    //    - the graph is connected (ignoring isolated vertices)
    // This method is solely for unit testing.
    private static boolean hasEulerianPath(Graph G) {
        if (G.E() == 0) return true;

        // Condition 1: degree(v) is even except for possibly two
        int oddDegreeVertices = 0;
        for (int v = 0; v < G.V(); v++)
            if (G.degree(v) % 2 != 0)
                oddDegreeVertices++;
        if (oddDegreeVertices > 2) return false;

        // Condition 2: graph is connected, ignoring isolated vertices
        int s = nonIsolatedVertex(G);
        BreadthFirstPaths bfs = new BreadthFirstPaths(G, s);
        for (int v = 0; v < G.V(); v++)
            if (G.degree(v) > 0 && !bfs.hasPathTo(v))
                return false;

        return true;
    }

    // check that solution is correct
    private boolean certifySolution(Graph G) {

        // internal consistency check
        if (hasEulerianPath() == (path() == null)) return false;

        // hashEulerianPath() returns correct value
        if (hasEulerianPath() != hasEulerianPath(G)) return false;

        // nothing else to check if no Eulerian path
        if (path == null) return true;

        // check that path() uses correct number of edges
        if (path.size() != G.E() + 1) return false;

        // check that path() is a path in G

        return true;
    }
}