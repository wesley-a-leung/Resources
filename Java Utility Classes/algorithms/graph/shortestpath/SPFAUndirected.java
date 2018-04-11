package algorithms.graph.shortestpath;

import java.util.ArrayDeque;
import java.util.Deque;
import java.util.Queue;
import java.util.Stack;

import datastructures.graph.WeightedEdge;
import datastructures.graph.WeightedGraph;

/**
 *  The {@code SPFA} class represents a data type for solving the
 *  single-source shortest paths problem in edge-weighted graphs with
 *  no negative cycles. 
 *  <p>
 *  The Shortest Path Faster Algorithm is an improvement on the Bellman-Ford-Moore Algorithm.
 *  This implementation uses the small label first technique.
 *  The constructor takes time proportional to <em>V</em> (<em>V</em> + <em>E</em>)
 *  in the worst case, and <em>E</em> on average, where <em>V</em> is the number of vertices
 *  and <em>E</em> is the number of edges.
 *  Afterwards, the {@code distTo()} and {@code hasPathTo()} methods take constant time;
 *  the {@code pathTo()} method takes time proportional to the number of edges returned.
 *
 *  @author Wesley Leung
 */
public class SPFAUndirected {
    private double[] distTo;          // distTo[v] = distance  of shortest s->v path
    private WeightedEdge[] edgeTo;    // edgeTo[v] = last edge on shortest s->v path
    private boolean[] inQueue;
    private Deque<Integer> q;
    
    /**
     * Computes a shortest-paths tree from the source vertex {@code s} to every other
     * vertex in the edge-weighted graph {@code G}.
     *
     * @param  G the edge-weighted graph
     * @param  s the source vertex
     * @throws IllegalArgumentException unless {@code 0 <= s < V}
     */
    public SPFAUndirected(WeightedGraph G, int s) {
        distTo = new double[G.V()];
        edgeTo = new WeightedEdge[G.V()];
        inQueue = new boolean[G.V()];
        q = new ArrayDeque<Integer>();

        validateVertex(s);

        for (int v = 0; v < G.V(); v++)
            distTo[v] = Double.POSITIVE_INFINITY;
        distTo[s] = 0.0;
        inQueue[s] = true;
        q.addLast(s);
        while (!q.isEmpty()) {
            int v = q.removeFirst();
            inQueue[v] = false;
            for (WeightedEdge e : G.adj(v)) {
                int w = e.other(v);
                if (distTo[w] > distTo[v] + e.weight()) {
                    distTo[w] = distTo[v] + e.weight();
                    edgeTo[w] = e;
                    if (!inQueue[w]) {
                        if (!q.isEmpty() && distTo[w] < distTo[q.peekFirst()]) q.addFirst(w);
                        else q.addLast(w);
                        inQueue[w] = true;
                    }
                }
            }
        }
    }

    /**
     * Returns the length of a shortest path from the source vertex {@code s} to vertex {@code v}.
     * @param  v the destination vertex
     * @return the length of a shortest path from the source vertex {@code s} to vertex {@code v};
     *         {@code Double.POSITIVE_INFINITY} if no such path
     * @throws IllegalArgumentException unless {@code 0 <= v < V}
     */
    public double distTo(int v) {
        validateVertex(v);
        return distTo[v];
    }

    /**
     * Returns true if there is a path from the source vertex {@code s} to vertex {@code v}.
     *
     * @param  v the destination vertex
     * @return {@code true} if there is a path from the source vertex
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
        Stack<WeightedEdge> rev = new Stack<WeightedEdge>();
        int x = v;
        for (WeightedEdge e = edgeTo[v]; e != null; e = edgeTo[x]) {
            rev.push(e);
            x = e.other(x);
        }
        Queue<WeightedEdge> path = new ArrayDeque<WeightedEdge>();
        while (!rev.isEmpty()) path.offer(rev.pop());
        return path;
    }

    // throw an IllegalArgumentException unless {@code 0 <= v < V}
    private void validateVertex(int v) {
        int V = distTo.length;
        if (v < 0 || v >= V)
            throw new IllegalArgumentException("vertex " + v + " is not between 0 and " + (V-1));
    }
}
