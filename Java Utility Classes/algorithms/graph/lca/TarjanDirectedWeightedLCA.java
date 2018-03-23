package algorithms.graph.lca;

import java.util.ArrayList;

import algorithms.graph.cycle.DirectedWeightedCycle;
import datastructures.Pair;
import datastructures.UF;
import datastructures.graph.DirectedWeightedEdge;
import datastructures.graph.WeightedDigraph;

/**
 * The {@code TarjanWeightedLCA} class represents a data structure for finding the
 * lowest common ancestor when the queries are known in advance. The constructor
 * take time proportional to <em>N</em> + <em>Q</em>.
 * 
 * @author Wesley Leung
 */
public class TarjanDirectedWeightedLCA {
    private static final boolean WHITE = false;
    private static final boolean BLACK = true;
    
    private boolean[] color;
    private int[] ancestor;
    private ArrayList<Pair<Integer, Integer>>[] queriesList;
    private int[] answers;
    private UF uf;
    
    public TarjanDirectedWeightedLCA(WeightedDigraph G, Pair<Integer, Integer>[] queries) {
        DirectedWeightedCycle cycle = new DirectedWeightedCycle(G);
        if (cycle.hasCycle()) throw new IllegalArgumentException("Graph is not acyclic.");
        color = new boolean[G.V()];
        ancestor = new int[G.V()];
        queriesList = new ArrayList[G.V()];
        answers = new int[queries.length];
        uf = new UF(G.V());
        for (int v = 0; v < G.V(); v++) {
            color[v] = WHITE;
            queriesList[v] = new ArrayList<Pair<Integer, Integer>>();
        }
        for (int i = 0; i < queries.length; i++) {
            queriesList[queries[i].first].add(new Pair<Integer, Integer>(queries[i].second, i));
            queriesList[queries[i].second].add(new Pair<Integer, Integer>(queries[i].first, i));
        }
        dfs(G, 0, -1);
    }
    
    private void dfs(WeightedDigraph G, int v, int prev) {
        ancestor[v] = v;
        for (DirectedWeightedEdge e : G.adj(v)) {
            int w = e.to();
            if (w == prev) continue;
            dfs(G, w, v);
            uf.union(v, w);
            ancestor[uf.find(v)] = v;
        }
        color[v] = BLACK;
        for (Pair<Integer, Integer> q : queriesList[v]) {
            if (color[q.first] == BLACK) answers[q.second] = ancestor[uf.find(q.first)];
        }
    }
    
    public int[] answers() {
        return answers;
    }
}
