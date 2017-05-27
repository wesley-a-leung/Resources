package algorithms.graph.shortestpath;

import java.util.ArrayList;
import java.util.HashSet;

import datastructures.MinPQ;
import datastructures.graph.DirectedWeightedPath;
import datastructures.graph.WeightedDigraph;
import datastructures.graph.DirectedWeightedEdge;

/**
 *  The {@code YenKSP} class represents a data type for solving the
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
	MinPQ<DirectedWeightedPath> pq;
	
	public YenDirectedKSP(WeightedDigraph G, int source, int sink) {
		this.G = G;
		this.V = G.V();
		this.source = source;
		this.sink = sink;
		validateVertex(source);
		validateVertex(sink);
		this.paths = new ArrayList<DirectedWeightedPath>();
		pq = new MinPQ<DirectedWeightedPath>();
		paths.add(new DirectedWeightedPath(new DijkstraSP(G, source).pathList(sink)));
	}
	
	private void find(int k) {
		if (paths.get(k - 1).size() == 0) {
			paths.add(new DirectedWeightedPath());
			return;
		}
		int spurNode = source;
		for (int i = 0; i < paths.get(k - 1).size(); i++) {
			DirectedWeightedPath rootPath = new DirectedWeightedPath();
			for (int j = 0; j < i; j++) {
				rootPath.add(paths.get(k - 1).get(j));
			}
			for (DirectedWeightedPath p: paths) {
				DirectedWeightedPath pPath = new DirectedWeightedPath();
				for (int j = 0; j < i && j < p.size(); j++) {
					pPath.add(p.get(j));
				}
				if (rootPath.equals(pPath) || rootPath.size() == 0) {
					G.removeEdge(p.get(i));
				}
			}
			int last = source;
			for (DirectedWeightedEdge e: rootPath) {
				G.removeEdge(e);
				HashSet<DirectedWeightedEdge> remove = new HashSet<DirectedWeightedEdge>();
				for (DirectedWeightedEdge f: G.adj(last)) {
					remove.add(f);
				}
				for (DirectedWeightedEdge f: remove) {
					G.removeEdge(f);
				}
				last = e.to();
			}
			DijkstraSP sp = new DijkstraSP(G, spurNode);
			DirectedWeightedPath spurPath = new DirectedWeightedPath(sp.pathList(sink));
			DirectedWeightedPath totalPath = new DirectedWeightedPath();
			totalPath.addAll(rootPath);
			totalPath.addAll(spurPath);
			if (totalPath.length() > 0 && sp.hasPathTo(sink)) pq.insert(totalPath);
			G.restoreEdges();
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
	
    // throw an IllegalArgumentException unless {@code 0 <= v < V}
    private void validateVertex(int v) {
        if (v < 0 || v >= V)
            throw new IllegalArgumentException("vertex " + v + " is not between 0 and " + (V-1));
    }
}
