package algorithms.graph.shortestpath;

import java.util.ArrayList;
import java.util.ArrayDeque;
import java.util.Queue;
import java.util.Stack;

import datastructures.graph.DirectedWeightedEdge;
import datastructures.graph.WeightedDigraph;

/**
 *  The {@code BellmanFordDirectedSP} class represents a data type for solving the
 *  single-source shortest paths problem in edge-weighted digraphs with
 *  no negative cycles. 
 *  The edge weights can be positive, negative, or zero.
 *  This class finds the shortest path from the source vertex <em>s</em>
 *  to every other vertex, or detects a negative cycle if there is one.
 *  <p>
 *  This implementation uses the Bellman-Ford-Moore algorithm.
 *  The constructor takes time proportional to <em>VE</em>
 *  in the worst case, where <em>V</em> is the number of vertices and <em>E</em>
 *  is the number of edges.
 *  Afterwards, the {@code distTo()} and {@code hasPathTo()} methods take constant time;
 *  the {@code pathTo()} method takes time proportional to the number of edges returned.
 *
 *  @author Wesley Leung
 */
public class BellmanFordDirectedSP {
    private double[] distTo;          // distTo[v] = distance  of shortest s->v path
    private DirectedWeightedEdge[] edgeTo;    // edgeTo[v] = last edge on shortest s->v path
    
    /**
     * Computes a shortest-paths tree from the source vertex {@code s} to every other
     * vertex in the edge-weighted digraph {@code G}.
     *
     * @param  G the edge-weighted digraph
     * @param  s the source vertex
     * @throws IllegalArgumentException unless {@code 0 <= s < V}
     * @throws IllegalArgumentException if there is a negative cycle
     */
    public BellmanFordDirectedSP(WeightedDigraph G, int s) {
        distTo = new double[G.V()];
        edgeTo = new DirectedWeightedEdge[G.V()];

        validateVertex(s);

        for (int v = 0; v < G.V(); v++)
            distTo[v] = Double.POSITIVE_INFINITY;
        distTo[s] = 0.0;
        
        ArrayList<DirectedWeightedEdge> edges = G.edges();
        for (int i = 0; i < G.V() - 1; i++) {
            for (DirectedWeightedEdge e : edges) {
                int v = e.from();
                int w = e.to();
                if (distTo[w] > distTo[v] + e.weight()) {
                    distTo[w] = distTo[v] + e.weight();
                    edgeTo[w] = e;
                }
            }
        }
        
        for (DirectedWeightedEdge e : G.edges()) {
            if (distTo[e.to()] > distTo[e.from()] + e.weight()) throw new IllegalArgumentException("Graph has a negative cycle.");
        }
    }
    
    /**
     * Computes a shortest-paths tree from the source vertex {@code s} to every other
     * vertex in the edge array.
     *
     * @param  V the number of vertices
     * @param  edges the edge array
     * @param  s the source vertex
     * @throws IllegalArgumentException unless {@code 0 <= s < V}
     * @throws IllegalArgumentException if there is a negative cycle
     */
    public BellmanFordDirectedSP(int V, DirectedWeightedEdge[] edges, int s) {
        distTo = new double[V];
        edgeTo = new DirectedWeightedEdge[V];

        validateVertex(s);

        for (int v = 0; v < V; v++)
            distTo[v] = Double.POSITIVE_INFINITY;
        distTo[s] = 0.0;

        for (int i = 0; i < V - 1; i++) {
            for (DirectedWeightedEdge e : edges) {
                int v = e.from();
                int w = e.to();
                if (distTo[w] > distTo[v] + e.weight()) {
                    distTo[w] = distTo[v] + e.weight();
                    edgeTo[w] = e;
                }
            }
        }
        
        for (DirectedWeightedEdge e : edges) {
            if (distTo[e.to()] > distTo[e.from()] + e.weight()) throw new IllegalArgumentException("Graph has a negative cycle.");
        }
    }
    
    /**
     * Computes a shortest-paths tree from the source vertex {@code s} to every other
     * vertex in the edge list.
     *
     * @param  V the number of vertices
     * @param  edges the edge list
     * @param  s the source vertex
     * @throws IllegalArgumentException unless {@code 0 <= s < V}
     * @throws IllegalArgumentException if there is a negative cycle
     */
    public BellmanFordDirectedSP(int V, Iterable<DirectedWeightedEdge> edges, int s) {
        distTo = new double[V];
        edgeTo = new DirectedWeightedEdge[V];

        validateVertex(s);

        for (int v = 0; v < V; v++)
            distTo[v] = Double.POSITIVE_INFINITY;
        distTo[s] = 0.0;

        for (int i = 0; i < V - 1; i++) {
            for (DirectedWeightedEdge e : edges) {
                int v = e.from();
                int w = e.to();
                if (distTo[w] > distTo[v] + e.weight()) {
                    distTo[w] = distTo[v] + e.weight();
                    edgeTo[w] = e;
                }
            }
        }
        
        for (DirectedWeightedEdge e : edges) {
            if (distTo[e.to()] > distTo[e.from()] + e.weight()) throw new IllegalArgumentException("Graph has a negative cycle.");
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
