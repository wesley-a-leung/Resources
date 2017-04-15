package algorithms;

/******************************************************************************
 *  Compilation:  javac DirectedEulerianCycle.java
 *  Execution:    java DirectedEulerianCycle V E
 *  Dependencies: Digraph.java Stack.java StdOut.java
 *                BreadthFirstPaths.java
 *                DigraphGenerator.java StdRandom.java
 *
 *  Find an Eulerian cycle in a digraph, if one exists.
 *
 ******************************************************************************/

import java.util.Iterator;

import datastructures.Digraph;
import datastructures.Graph;
import datastructures.Stack;

/**
 *  The {@code DirectedEulerianCycle} class represents a data type
 *  for finding an Eulerian cycle or path in a digraph.
 *  An <em>Eulerian cycle</em> is a cycle (not necessarily simple) that
 *  uses every edge in the digraph exactly once.
 *  <p>
 *  This implementation uses a nonrecursive depth-first search.
 *  The constructor runs in O(<Em>E</em> + <em>V</em>) time,
 *  and uses O(<em>V</em>) extra space, where <em>E</em> is the
 *  number of edges and <em>V</em> the number of vertices
 *  All other methods take O(1) time.
 *  <p>
 *  To compute Eulerian paths in digraphs, see {@link DirectedEulerianPath}.
 *  To compute Eulerian cycles and paths in undirected graphs, see
 *  {@link EulerianCycle} and {@link EulerianPath}.
 *  <p>
 *  For additional documentation,
 *  see <a href="http://algs4.cs.princeton.edu/42digraph">Section 4.2</a> of
 *  <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne.
 * 
 *  @author Robert Sedgewick
 *  @author Kevin Wayne
 *  @author Nate Liu
 */
public class DirectedEulerianCycle {
    private Stack<Integer> cycle = null;  // Eulerian cycle; null if no such cylce

    /**
     * Computes an Eulerian cycle in the specified digraph, if one exists.
     * 
     * @param G the digraph
     */
    public DirectedEulerianCycle(Digraph G) {

        // must have at least one edge
        if (G.E() == 0) return;

        // necessary condition: indegree(v) = outdegree(v) for each vertex v
        // (without this check, DFS might return a path instead of a cycle)
        for (int v = 0; v < G.V(); v++)
            if (G.outdegree(v) != G.indegree(v))
                return;

        // create local view of adjacency lists, to iterate one vertex at a time
        Iterator<Integer>[] adj = (Iterator<Integer>[]) new Iterator[G.V()];
        for (int v = 0; v < G.V(); v++)
            adj[v] = G.adj(v).iterator();

        // initialize stack with any non-isolated vertex
        int s = nonIsolatedVertex(G);
        Stack<Integer> stack = new Stack<Integer>();
        stack.push(s);

        // greedily add to putative cycle, depth-first search style
        cycle = new Stack<Integer>();
        while (!stack.isEmpty()) {
            int v = stack.pop();
            while (adj[v].hasNext()) {
                stack.push(v);
                v = adj[v].next();
            }
            // add vertex with no more leaving edges to cycle
            cycle.push(v);
        }

        // check if all edges have been used
        // (in case there are two or more vertex-disjoint Eulerian cycles)
        if (cycle.size() != G.E() + 1)
            cycle = null;

        assert certifySolution(G);
    }

    /**
     * Returns the sequence of vertices on an Eulerian cycle.
     * 
     * @return the sequence of vertices on an Eulerian cycle;
     *         {@code null} if no such cycle
     */
    public Iterable<Integer> cycle() {
        return cycle;
    }

    /**
     * Returns true if the digraph has an Eulerian cycle.
     * 
     * @return {@code true} if the digraph has an Eulerian cycle;
     *         {@code false} otherwise
     */
    public boolean hasEulerianCycle() {
        return cycle != null;
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

    // Determines whether a digraph has an Eulerian cycle using necessary
    // and sufficient conditions (without computing the cycle itself):
    //    - at least one edge
    //    - indegree(v) = outdegree(v) for every vertex
    //    - the graph is connected, when viewed as an undirected graph
    //      (ignoring isolated vertices)
    private static boolean hasEulerianCycle(Digraph G) {

        // Condition 0: at least 1 edge
        if (G.E() == 0) return false;

        // Condition 1: indegree(v) == outdegree(v) for every vertex
        for (int v = 0; v < G.V(); v++)
            if (G.outdegree(v) != G.indegree(v))
                return false;

        // Condition 2: graph is connected, ignoring isolated vertices
        Graph H = new Graph(G.V());
        for (int v = 0; v < G.V(); v++)
            for (int w : G.adj(v))
                H.addEdge(v, w);
        
        // check that all non-isolated vertices are conneted
        int s = nonIsolatedVertex(G);
        BreadthFirstPaths bfs = new BreadthFirstPaths(H, s);
        for (int v = 0; v < G.V(); v++)
            if (H.degree(v) > 0 && !bfs.hasPathTo(v))
                return false;

        return true;
    }

    // check that solution is correct
    private boolean certifySolution(Digraph G) {

        // internal consistency check
        if (hasEulerianCycle() == (cycle() == null)) return false;

        // hashEulerianCycle() returns correct value
        if (hasEulerianCycle() != hasEulerianCycle(G)) return false;

        // nothing else to check if no Eulerian cycle
        if (cycle == null) return true;

        // check that cycle() uses correct number of edges
        if (cycle.size() != G.E() + 1) return false;

        // check that cycle() is a directed cycle of G

        return true;
    }

}
