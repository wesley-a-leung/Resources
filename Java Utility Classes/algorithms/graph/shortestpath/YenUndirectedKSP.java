package algorithms.graph.shortestpath;

import java.util.ArrayList;
import java.util.HashSet;

import datastructures.IndexMinPQ;
import datastructures.MinPQ;
import datastructures.Stack;
import datastructures.graph.WeightedEdge;
import datastructures.graph.WeightedPath;
import datastructures.graph.WeightedGraph;

/**
 *  The {@code YenKSP} class represents a data type for solving the
 *  Kth shortest path problem in variable edge-weighted digraphs
 *  where the edge weights are nonnegative.
 *  @author Wesley Leung
 */  
public class YenUndirectedKSP {
	private final int source;
	private final int sink;
	private WeightedGraph G;
	private final int V;
	private ArrayList<WeightedPath> paths;
	private MinPQ<WeightedPath> pq;
	private HashSet<WeightedEdge> removedEdges = new HashSet<WeightedEdge>();
	HashSet<Integer> removedVerticies = new HashSet<Integer>();
	private double[] distTo;
	private WeightedEdge[] edgeTo;
	private IndexMinPQ<Double> ipq;
	
	public YenUndirectedKSP(WeightedGraph G, int source, int sink) {
		this.G = G;
		this.V = G.V();
		validateVertex(source);
		validateVertex(sink);
		this.source = source;
		this.sink = sink;
		this.paths = new ArrayList<WeightedPath>();
		pq = new MinPQ<WeightedPath>();
		paths.add(dijkstraSP(source, sink));
	}
	
	private void find(int k) {
		if (paths.get(k - 1) == null || paths.get(k - 1).size() == 0) {
			paths.add(new WeightedPath());
			return;
		}
		int spurNode = source;
		for (int i = 0; i < paths.get(k - 1).size(); i++) {
			WeightedPath rootPath = new WeightedPath();
			removedEdges.clear();
			removedVerticies.clear();
			for (int j = 0; j < i; j++) {
				rootPath.add(paths.get(k - 1).get(j));
			}
			for (WeightedPath p: paths) {
				if (p.size() < rootPath.size()) continue;
				WeightedPath pPath = new WeightedPath();
				for (int j = 0; j < i && j < p.size(); j++) {
					pPath.add(p.get(j));
				}
				if (rootPath.equals(pPath) || rootPath.size() == 0) {
					removedEdges.add(p.get(i));
				}
			}
			int last = source;
			removedVerticies.add(source);
			for (WeightedEdge e: rootPath) {
				removedVerticies.add(e.other(last));
				last = e.other(last);
			}
			WeightedPath spurPath = dijkstraSP(spurNode, sink);
			WeightedPath totalPath = new WeightedPath();
			totalPath.addAll(rootPath);
			totalPath.addAll(spurPath);
			if (totalPath.length() > 0 && spurPath != null) pq.insert(totalPath);
			spurNode = paths.get(k - 1).get(i).other(spurNode);
		}
		if (pq.isEmpty()) paths.add(new WeightedPath());
		else paths.add(pq.delMin());
	}
	
	public WeightedPath KSP(int K) {
		if (K >= paths.size()) {
			for (int k = paths.size(); k <= K; k++) {
				find(k);
			}
		}
		return paths.get(K);
	}
	
	private WeightedPath dijkstraSP(int s, int t) {
		distTo = new double[G.V()];
        edgeTo = new WeightedEdge[G.V()];

        validateVertex(s);

        for (int v = 0; v < G.V(); v++)
            distTo[v] = Double.POSITIVE_INFINITY;
        distTo[s] = 0.0;

        // relax vertices in order of distance from s
        ipq = new IndexMinPQ<Double>(G.V());
        ipq.insert(s, distTo[s]);
        while (!ipq.isEmpty()) {
            int v = ipq.delMin();
            for (WeightedEdge e : G.adj(v)) {
            	if (removedEdges.contains(e) || removedVerticies.contains(e.other(v))) continue;
            	relax(e, v);
            }
        }
        if(edgeTo[t] == null) return null;
        Stack<WeightedEdge> path = new Stack<WeightedEdge>();
        int x = t;
        for (WeightedEdge e = edgeTo[t]; e != null; e = edgeTo[x]) {
            path.push(e);
            x = e.other(x);
        }
        WeightedPath ret = new WeightedPath();
        while (!path.isEmpty()) {
        	ret.add(path.pop());
        }
        return ret;
	}
	
    private void relax(WeightedEdge e, int v) {
        int w = e.other(v);
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
