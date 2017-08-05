package algorithms.graph.components;

import java.util.HashSet;
import java.util.ArrayList;

import datastructures.Pair;
import datastructures.Stack;
import datastructures.graph.Graph;

/**
 *  Identifies articulation points and bridges.
 *  This can be used to decompose a graph into biconnected components.
 *  Runs in O(E + V) time.
 */
public class Biconnected {
    private int bridges;
    private int[] low;
    private int[] pre;
    private ArrayList<ArrayList<Pair<Integer, Integer>>> components;
    private int cnt;
    private boolean[] articulation;
    private HashSet<Integer>[] bridgeList;  // adjacency bridge list
    private Stack<Pair<Integer, Integer>> s;

    public Biconnected(Graph G) {
        low = new int[G.V()];
        pre = new int[G.V()];
        bridgeList = new HashSet[G.V()];
        articulation = new boolean[G.V()];
        components = new ArrayList<ArrayList<Pair<Integer, Integer>>>();
        s = new Stack<Pair<Integer, Integer>>();
        for (int v = 0; v < G.V(); v++) {
            low[v] = -1;
            pre[v] = -1;
            bridgeList[v] = new HashSet<Integer>();
        }
        for (int v = 0; v < G.V(); v++) {
            if (pre[v] == -1) dfs(G, v, v);
        }
    }

    private void dfs(Graph G, int u, int v) {
        int children = 0;
        pre[v] = cnt++;
        low[v] = pre[v];
        for (int w : G.adj(v)) {
            if (pre[w] == -1) {
                children++;
                s.push(new Pair<Integer, Integer>(v, w));
                dfs(G, v, w);
                // update low number
                low[v] = Math.min(low[v], low[w]);
                // non-root of DFS is an articulation point if low[w] >= pre[v]
                if (low[w] >= pre[v] && u != v) {
                    articulation[v] = true;
                    components.add(new ArrayList<Pair<Integer, Integer>>());
                    while (s.peek().first != u && s.peek().second != v) {
                        components.get(components.size() - 1).add(new Pair<Integer, Integer>(s.peek().first, s.peek().second));
                        s.pop();
                    }
                    components.get(components.size() - 1).add(new Pair<Integer, Integer>(s.peek().first, s.peek().second));
                    s.pop();
                }
                if (low[w] == pre[w]) {
                    bridgeList[v].add(w);
                    bridges++;
                }
            }
            // update low number - ignore reverse of edge leading to v
            else if (w != u) low[v] = Math.min(low[v], pre[w]);
        }
        // root of DFS is an articulation point if it has more than 1 child
        if (u == v && children > 1) articulation[v] = true;
    }

    // is vertex v an articulation point?
    public boolean isArticulation(int v) {
        return articulation[v];
    }
    
    public ArrayList<ArrayList<Pair<Integer, Integer>>> components() {
        return components;
    }
    
    public int countComponents() {
        return components.size();
    }

    public int countBridges() {
        return bridges;
    }
    
    public boolean isBridge(int v, int w) {
        return bridgeList[v].contains(w);
    }
}
