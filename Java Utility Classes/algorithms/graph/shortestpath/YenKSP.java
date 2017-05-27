package algorithms.graph.shortestpath;

import java.util.ArrayList;
import java.util.HashSet;

import datastructures.MinPQ;
import datastructures.graph.WeightedEdge;
import datastructures.graph.WeightedPath;
import datastructures.graph.WeightedGraph;

/**
 *  The {@code YenKSP} class represents a data type for solving the
 *  Kth shortest path problem in variable edge-weighted digraphs
 *  where the edge weights are nonnegative.
 *  @author Wesley Leung
 */  
public class YenKSP {
	private final int source;
	private final int sink;
	private WeightedGraph G;
	private final int V;
	private ArrayList<WeightedPath> paths;
	MinPQ<WeightedPath> pq;
	
	public YenKSP(WeightedGraph G, int source, int sink) {
		this.G = G;
		this.V = G.V();
		this.source = source;
		this.sink = sink;
		validateVertex(source);
		validateVertex(sink);
		this.paths = new ArrayList<WeightedPath>();
		pq = new MinPQ<WeightedPath>();
		paths.add(new WeightedPath(new DijkstraUndirectedSP(G, source).pathList(sink)));
	}
	
	private void find(int k) {
		if (paths.get(k - 1).size() == 0) {
			paths.add(new WeightedPath());
			return;
		}
		int spurNode = source;
		for (int i = 0; i < paths.get(k - 1).size(); i++) {
			WeightedPath rootPath = new WeightedPath();
			for (int j = 0; j < i; j++) {
				rootPath.add(paths.get(k - 1).get(j));
			}
			for (WeightedPath p: paths) {
				WeightedPath pPath = new WeightedPath();
				for (int j = 0; j < i && j < p.size(); j++) {
					pPath.add(p.get(j));
				}
				if (rootPath.equals(pPath) || rootPath.size() == 0) {
					G.removeEdge(p.get(i));
				}
			}
			int last = source;
			for (WeightedEdge e: rootPath) {
				G.removeEdge(e);
				HashSet<WeightedEdge> remove = new HashSet<WeightedEdge>();
				for (WeightedEdge f: G.adj(last)) {
					remove.add(f);
				}
				for (WeightedEdge f: remove) {
					G.removeEdge(f);
				}
				last = e.other(last);
			}
			DijkstraUndirectedSP sp = new DijkstraUndirectedSP(G, spurNode);
			WeightedPath spurPath = new WeightedPath(sp.pathList(sink));
			WeightedPath totalPath = new WeightedPath();
			totalPath.addAll(rootPath);
			totalPath.addAll(spurPath);
			if (totalPath.length() > 0 && sp.hasPathTo(sink)) pq.insert(totalPath);
			G.restoreEdges();
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
	
    // throw an IllegalArgumentException unless {@code 0 <= v < V}
    private void validateVertex(int v) {
        if (v < 0 || v >= V)
            throw new IllegalArgumentException("vertex " + v + " is not between 0 and " + (V-1));
    }
}
