package algorithms.graph.cycle;

/******************************************************************************
 *  Compilation:  javac DirectedEulerianPath.java
 *  Execution:    java DirectedEulerianPath V E
 *  Dependencies: Digraph.java Stack.java StdOut.java
 *                BreadthFirstPaths.java
 *                DigraphGenerator.java StdRandom.java
 *
 *  Find an Eulerian path in a digraph, if one exists.
 *
 ******************************************************************************/

import java.util.Iterator;

import datastructures.Stack;
import datastructures.graph.Digraph;
import datastructures.graph.Graph;
import algorithms.graph.search.BreadthFirstPaths;

/**
 *  The {@code DirectedEulerianPath} class represents a data type
 *  for finding an Eulerian path in a digraph.
 *  An <em>Eulerian path</em> is a path (not necessarily simple) that
 *  uses every edge in the digraph exactly once.
 *  <p>
 *  This implementation uses a nonrecursive depth-first search.
 *  The constructor runs in O(E + V) time, and uses O(V) extra space,
 *  where E is the number of edges and V the number of vertices
 *  All other methods take O(1) time.
 *  <p>
 *  To compute Eulerian cycles in digraphs, see {@link DirectedEulerianCycle}.
 *  To compute Eulerian cycles and paths in undirected graphs, see
 *  {@link EulerianCycle} and {@link EulerianPath}.
 *  <p>
 *  For additional documentation,
 *  see <a href="http://algs4.cs.princeton.edu/42digraph">Section 4.2</a> of
 *  <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne.
 * 
 * @author Robert Sedgewick
 * @author Kevin Wayne
 * @author Nate Liu
 */
public class DirectedEulerianPath {
    private Stack<Integer> path = null;   // Eulerian path; null if no suh path

    /**
     * Computes an Eulerian path in the specified digraph, if one exists.
     * 
     * @param G the digraph
     */
    public DirectedEulerianPath(Digraph G) {

        // find vertex from which to start potential Eulerian path:
        // a vertex v with outdegree(v) > indegree(v) if it exits;
        // otherwise a vertex with outdegree(v) > 0
        int deficit = 0;
        int s = nonIsolatedVertex(G);
        for (int v = 0; v < G.V(); v++) {
            if (G.outdegree(v) > G.indegree(v)) {
                deficit += (G.outdegree(v) - G.indegree(v));
                s = v;
            }
        }

        // digraph can't have an Eulerian path
        // (this condition is needed)
        if (deficit > 1) return;

        // special case for digraph with zero edges (has a degenerate Eulerian path)
        if (s == -1) s = 0;

        // create local view of adjacency lists, to iterate one vertex at a time
        Iterator<Integer>[] adj = (Iterator<Integer>[]) new Iterator[G.V()];
        for (int v = 0; v < G.V(); v++)
            adj[v] = G.adj(v).iterator();

        // greedily add to cycle, depth-first search style
        Stack<Integer> stack = new Stack<Integer>();
        stack.push(s);
        path = new Stack<Integer>();
        while (!stack.isEmpty()) {
            int v = stack.pop();
            while (adj[v].hasNext()) {
                stack.push(v);
                v = adj[v].next();
            }
            // push vertex with no more available edges to path
            path.push(v);
        }
            
        // check if all edges have been used
        if (path.size() != G.E() + 1)
            path = null;

        assert check(G);
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
     * Returns true if the digraph has an Eulerian path.
     * 
     * @return {@code true} if the digraph has an Eulerian path;
     *         {@code false} otherwise
     */
    public boolean hasEulerianPath() {
        return path != null;
    }


    // returns any non-isolated vertex; -1 if no such vertex
    private static int nonIsolatedVertex(Digraph G) {
        for (int v = 0; v < G.V(); v++)
            if (G.outdegree(v) > 0)
                return v;
        return -1;
    }


    /**************************************************************************
     *
     *  The code below is solely for testing correctness of the data type.
     *
     **************************************************************************/

    // Determines whether a digraph has an Eulerian path using necessary
    // and sufficient conditions (without computing the path itself):
    //    - indegree(v) = outdegree(v) for every vertex,
    //      except one vertex v may have outdegree(v) = indegree(v) + 1
    //      (and one vertex v may have indegree(v) = outdegree(v) + 1)
    //    - the graph is connected, when viewed as an undirected graph
    //      (ignoring isolated vertices)
    // This method is solely for unit testing.
    private static boolean hasEulerianPath(Digraph G) {
        if (G.E() == 0) return true;

        // Condition 1: indegree(v) == outdegree(v) for every vertex,
        // except one vertex may have outdegree(v) = indegree(v) + 1
        int deficit = 0;
        for (int v = 0; v < G.V(); v++)
            if (G.outdegree(v) > G.indegree(v))
                deficit += (G.outdegree(v) - G.indegree(v));
        if (deficit > 1) return false;

        // Condition 2: graph is connected, ignoring isolated vertices
        Graph H = new Graph(G.V());
        for (int v = 0; v < G.V(); v++)
            for (int w : G.adj(v))
                H.addEdge(v, w);
        
        // check that all non-isolated vertices are connected
        int s = nonIsolatedVertex(G);
        BreadthFirstPaths bfs = new BreadthFirstPaths(H, s);
        for (int v = 0; v < G.V(); v++)
            if (H.degree(v) > 0 && !bfs.hasPathTo(v))
                return false;

        return true;
    }


    private boolean check(Digraph G) {

        // internal consistency check
        if (hasEulerianPath() == (path() == null)) return false;

        // hashEulerianPath() returns correct value
        if (hasEulerianPath() != hasEulerianPath(G)) return false;

        // nothing else to check if no Eulerian path
        if (path == null) return true;

        // check that path() uses correct number of edges
        if (path.size() != G.E() + 1) return false;

        // check that path() is a directed path in G

        return true;
    }
}
