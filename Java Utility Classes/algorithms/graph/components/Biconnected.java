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
    private ArrayList<HashSet<Integer>> components;
    private int cnt;
    private boolean[] articulation;
    private HashSet<Integer>[] bridgeList;  // adjacency bridge list
    private Stack<Pair<Integer, Integer>> s;

    public Biconnected(Graph G) {
        low = new int[G.V()];
        pre = new int[G.V()];
        bridgeList = new HashSet[G.V()];
        articulation = new boolean[G.V()];
        components = new ArrayList<HashSet<Integer>>();
        s = new Stack<Pair<Integer, Integer>>();
        for (int v = 0; v < G.V(); v++) {
            low[v] = -1;
            pre[v] = -1;
            bridgeList[v] = new HashSet<Integer>();
        }
        for (int v = 0; v < G.V(); v++) {
            if (pre[v] == -1) dfs(G, v, v);
            if (!s.isEmpty()) {
                components.add(new HashSet<Integer>());
                while (!s.isEmpty()) {
                    components.get(components.size() - 1).add(s.peek().first);
                    components.get(components.size() - 1).add(s.peek().second);
                    s.pop();
                }
            }
        }
    }

    private void dfs(Graph G, int v, int prev) {
        int children = 0;
        pre[v] = low[v] = cnt++;
        for (int w : G.adj(v)) {
            if (pre[w] == -1) {
                children++;
                s.push(new Pair<Integer, Integer>(v, w));
                dfs(G, w, v);
                // update low number
                low[v] = Math.min(low[v], low[w]);
                // non-root of DFS is an articulation point if low[w] >= pre[v]
                if ((prev == v && children > 1) || (prev != v && low[w] >= pre[v])) {
                    articulation[v] = true;
                    components.add(new HashSet<Integer>());
                    while (s.peek().first != v && s.peek().second != w) {
                        components.get(components.size() - 1).add(s.peek().first);
                        components.get(components.size() - 1).add(s.peek().second);
                        s.pop();
                    }
                    components.get(components.size() - 1).add(s.peek().first);
                    components.get(components.size() - 1).add(s.peek().second);
                    s.pop();
                }
                if (low[w] == pre[w]) {
                    bridgeList[v].add(w);
                    bridges++;
                }
            }
            // update low number - ignore reverse of edge leading to v
            else if (w != prev && pre[w] < low[v]) {
                low[v] = pre[w];
                s.push(new Pair<Integer, Integer>(v, w));
            }
        }
    }

    // is vertex v an articulation point?
    public boolean isArticulation(int v) {
        return articulation[v];
    }
    
    public ArrayList<HashSet<Integer>> components() {
        return components;
    }
    
    public HashSet<Integer> getComponent(int id) {
        return components.get(id);
    }
    
    public int getSize(int id) {
        return components.get(id).size();
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
