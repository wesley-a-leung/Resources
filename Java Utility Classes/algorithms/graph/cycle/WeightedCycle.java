package algorithms.graph.cycle;

import datastructures.Stack;
import datastructures.graph.WeightedEdge;
import datastructures.graph.WeightedGraph;

/**
 *  The {@code WeightedCycle} class represents a data type for 
 *  determining whether an undirected weighted weighted graph has a cycle.
 *  The <em>hasCycle</em> operation determines whether the weighted graph has
 *  a cycle and, if so, the <em>cycle</em> operation returns one.
 *  <p>
 *  This implementation uses depth-first search.
 *  The constructor takes time proportional to <em>V</em> + <em>E</em>
 *  (in the worst case),
 *  where <em>V</em> is the number of vertices and <em>E</em> is the number of edges.
 *  Afterwards, the <em>hasCycle</em> operation takes constant time;
 *  the <em>cycle</em> operation takes time proportional
 *  to the length of the cycle.
 *  <p>
 *
 *  @author Robert Sedgewick
 *  @author Kevin Wayne
 */
public class WeightedCycle {
    private boolean[] marked;
    private int[] edgeTo;
    private Stack<Integer> cycle;

    /**
     * Determines whether the undirected weighted graph {@code G} has a cycle and,
     * if so, finds such a cycle.
     *
     * @param G the undirected weighted graph
     */
    public WeightedCycle(WeightedGraph G) {
        if (hasSelfLoop(G)) return;
        if (hasParallelEdges(G)) return;
        marked = new boolean[G.V()];
        edgeTo = new int[G.V()];
        for (int v = 0; v < G.V(); v++)
            if (!marked[v])
                dfs(G, -1, v);
    }


    // does this weighted graph have a self loop?
    // side effect: initialize cycle to be self loop
    private boolean hasSelfLoop(WeightedGraph G) {
        for (int v = 0; v < G.V(); v++) {
        	for (WeightedEdge e: G.adj(v)) {
            	int w = e.other(v);
                if (v == w) {
                    cycle = new Stack<Integer>();
                    cycle.push(v);
                    cycle.push(v);
                    return true;
                }
            }
        }
        return false;
    }

    // does this weighted graph have two parallel edges?
    // side effect: initialize cycle to be two parallel edges
    private boolean hasParallelEdges(WeightedGraph G) {
        marked = new boolean[G.V()];

        for (int v = 0; v < G.V(); v++) {

            // check for parallel edges incident to v
            for (WeightedEdge e: G.adj(v)) {
            	int w = e.other(v);
                if (marked[w]) {
                    cycle = new Stack<Integer>();
                    cycle.push(v);
                    cycle.push(w);
                    cycle.push(v);
                    return true;
                }
                marked[w] = true;
            }

            // reset so marked[v] = false for all v
            for (WeightedEdge e: G.adj(v)) {
            	int w = e.other(v);
                marked[w] = false;
            }
        }
        return false;
    }

    /**
     * Returns true if the weighted graph {@code G} has a cycle.
     *
     * @return {@code true} if the weighted graph has a cycle; {@code false} otherwise
     */
    public boolean hasCycle() {
        return cycle != null;
    }

     /**
     * Returns a cycle in the weighted graph {@code G}.
     * @return a cycle if the weighted graph {@code G} has a cycle,
     *         and {@code null} otherwise
     */
    public Iterable<Integer> cycle() {
        return cycle;
    }

    private void dfs(WeightedGraph G, int u, int v) {
        marked[v] = true;
        for (WeightedEdge e: G.adj(v)) {
        	int w = e.other(v);

            // short circuit if cycle already found
            if (cycle != null) return;

            if (!marked[w]) {
                edgeTo[w] = v;
                dfs(G, v, w);
            }

            // check for cycle (but disregard reverse of edge leading to v)
            else if (w != u) {
                cycle = new Stack<Integer>();
                for (int x = v; x != w; x = edgeTo[x]) {
                    cycle.push(x);
                }
                cycle.push(w);
                cycle.push(v);
            }
        }
    }
}
