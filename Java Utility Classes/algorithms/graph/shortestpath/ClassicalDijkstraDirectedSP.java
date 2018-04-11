package algorithms.graph.shortestpath;

import java.util.ArrayDeque;
import java.util.Queue;
import java.util.Stack;

import datastructures.graph.DirectedWeightedEdge;
import datastructures.graph.WeightedDigraph;

/**
 *  The {@code ClassicalDijkstraSP} class represents a data type for solving the
 *  single-source shortest paths problem in edge-weighted digraphs
 *  where the edge weights are nonnegative.
 *  <p>
 *  This implementation uses the classical Dijkstra's algorithm.
 *  The constructor takes time proportional to <em>V</em><sup>3</sup>,
 *  where <em>V</em> is the number of vertices and <em>E</em> is the number of edges.
 *  Afterwards, the {@code distTo()} and {@code hasPathTo()} methods take
 *  constant time and the {@code pathTo()} method takes time proportional to the
 *  number of edges in the shortest path returned.
 *
 *  @author Wesley Leung
 */
public class ClassicalDijkstraDirectedSP {
    private double[] distTo;          // distTo[v] = distance  of shortest s->v path
    private DirectedWeightedEdge[] edgeTo;    // edgeTo[v] = last edge on shortest s->v path
    private boolean[] marked;
    
    /**
     * Computes a shortest-paths tree from the source vertex {@code s} to every other
     * vertex in the edge-weighted digraph {@code G}.
     *
     * @param  G the edge-weighted digraph
     * @param  s the source vertex
     * @throws IllegalArgumentException unless {@code 0 <= s < V}
     */
    public ClassicalDijkstraDirectedSP(WeightedDigraph G, int s) {
        distTo = new double[G.V()];
        edgeTo = new DirectedWeightedEdge[G.V()];
        marked = new boolean[G.V()];

        validateVertex(s);

        for (int v = 0; v < G.V(); v++)
            distTo[v] = Double.POSITIVE_INFINITY;
        distTo[s] = 0.0;

        for (int v = 0; v < G.V() - 1; v++) {
            int minV = -1;
            for (int w = 0; w < G.V(); w++) {
                if (!marked[w] && (minV == -1 || distTo[minV] > distTo[w])) minV = w;
            }
            marked[minV] = true;
            for (DirectedWeightedEdge e : G.adj(minV)) {
                int w = e.to();
                if (distTo[w] > distTo[minV] + e.weight()) {
                    distTo[w] = distTo[minV] + e.weight();
                    edgeTo[w] = e;
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
     * Returns a shortest path from the source vertex {@code s} to vertex {@code v}.
     *
     * @param  v the destination vertex
     * @return a shortest path from the source vertex {@code s} to vertex {@code v}
     *         as an iterable of edges, and {@code null} if no such path
     * @throws IllegalArgumentException unless {@code 0 <= v < V}
     */
    public Iterable<DirectedWeightedEdge> pathTo(int v) {
        validateVertex(v);
        if (!hasPathTo(v)) return null;
        Stack<DirectedWeightedEdge> rev = new Stack<DirectedWeightedEdge>();
        for (DirectedWeightedEdge e = edgeTo[v]; e != null; e = edgeTo[e.from()]) {
            rev.push(e);
        }
        Queue<DirectedWeightedEdge> path = new ArrayDeque<DirectedWeightedEdge>();
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

