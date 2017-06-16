package algorithms.graph.shortestpath;

import java.util.ArrayList;
import java.util.HashSet;

import datastructures.IndexMinPQ;
import datastructures.MinPQ;
import datastructures.Stack;
import datastructures.graph.DirectedWeightedPath;
import datastructures.graph.WeightedDigraph;
import datastructures.graph.DirectedWeightedEdge;

/**
 *  The {@code YenDirectedKSP} class represents a data type for solving the
 *  Kth shortest path problem in variable edge-weighted digraphs
 *  where the edge weights are nonnegative.
 *  @author Wesley Leung
 */  
public class YenDirectedKSP {
    private final int source;
    private final int sink;
    private WeightedDigraph G;
    private final int V;
    private ArrayList<DirectedWeightedPath> paths;
    private MinPQ<DirectedWeightedPath> pq;
    private HashSet<DirectedWeightedEdge> removedEdges = new HashSet<DirectedWeightedEdge>();
    HashSet<Integer> removedVerticies = new HashSet<Integer>();
    private double[] distTo;
    private DirectedWeightedEdge[] edgeTo;
    private IndexMinPQ<Double> ipq;
    
    public YenDirectedKSP(WeightedDigraph G, int source, int sink) {
        this.G = G;
        this.V = G.V();
        validateVertex(source);
        validateVertex(sink);
        this.source = source;
        this.sink = sink;
        this.paths = new ArrayList<DirectedWeightedPath>();
        pq = new MinPQ<DirectedWeightedPath>();
        paths.add(dijkstraSP(source, sink));
    }
    
    private void find(int k) {
        if (paths.get(k - 1) == null || paths.get(k - 1).size() == 0) {
            paths.add(new DirectedWeightedPath());
            return;
        }
        int spurNode = source;
        for (int i = 0; i < paths.get(k - 1).size(); i++) {
            DirectedWeightedPath rootPath = new DirectedWeightedPath();
            removedEdges.clear();
            removedVerticies.clear();
            for (int j = 0; j < i; j++) {
                rootPath.add(paths.get(k - 1).get(j));
            }
            for (DirectedWeightedPath p: paths) {
                if (p.size() < rootPath.size()) continue;
                DirectedWeightedPath pPath = new DirectedWeightedPath();
                for (int j = 0; j < i && j < p.size(); j++) {
                    pPath.add(p.get(j));
                }
                if (rootPath.equals(pPath) || rootPath.size() == 0) {
                    removedEdges.add(p.get(i));
                }
            }
            int last = source;
            removedVerticies.add(source);
            for (DirectedWeightedEdge e: rootPath) {
                removedVerticies.add(e.to());
            }
            DirectedWeightedPath spurPath = dijkstraSP(spurNode, sink);
            DirectedWeightedPath totalPath = new DirectedWeightedPath();
            totalPath.addAll(rootPath);
            totalPath.addAll(spurPath);
            if (totalPath.length() > 0 && spurPath != null) pq.insert(totalPath);
            spurNode = paths.get(k - 1).get(i).to();
        }
        if (pq.isEmpty()) paths.add(new DirectedWeightedPath());
        else paths.add(pq.delMin());
    }
    
    public DirectedWeightedPath KSP(int K) {
        if (K >= paths.size()) {
            for (int k = paths.size(); k <= K; k++) {
                find(k);
            }
        }
        return paths.get(K);
    }
    
    private DirectedWeightedPath dijkstraSP(int s, int t) {
        distTo = new double[G.V()];
        edgeTo = new DirectedWeightedEdge[G.V()];

        validateVertex(s);

        for (int v = 0; v < G.V(); v++)
            distTo[v] = Double.POSITIVE_INFINITY;
        distTo[s] = 0.0;

        // relax vertices in order of distance from s
        ipq = new IndexMinPQ<Double>(G.V());
        ipq.insert(s, distTo[s]);
        while (!ipq.isEmpty()) {
            int v = ipq.delMin();
            for (DirectedWeightedEdge e : G.adj(v)) {
                if (removedEdges.contains(e) || removedVerticies.contains(e.to())) continue;
                relax(e);
            }
        }
        if(edgeTo[t] == null) return null;
        Stack<DirectedWeightedEdge> path = new Stack<DirectedWeightedEdge>();
        int x = t;
        for (DirectedWeightedEdge e = edgeTo[t]; e != null; e = edgeTo[x]) {
            path.push(e);
            x = e.to();
        }
        DirectedWeightedPath ret = new DirectedWeightedPath();
        while (!path.isEmpty()) {
            ret.add(path.pop());
        }
        return ret;
    }
    
    private void relax(DirectedWeightedEdge e) {
        int v = e.from();
        int w = e.to();
        if (distTo[w] > distTo[v] + e.weight()) {
            distTo[w] = distTo[v] + e.weight();
            edgeTo[w] = e;
            if (ipq.contains(w)) ipq.decreaseKey(w, distTo[w]);
            else                ipq.insert(w, distTo[w]);
        }
    }
    
    
    
    // throw an IllegalArgumentException unless {@code 0 <= v < V}
    private void validateVertex(int v) {
        if (v < 0 || v >= V)
            throw new IllegalArgumentException("vertex " + v + " is not between 0 and " + (V-1));
    }
}
