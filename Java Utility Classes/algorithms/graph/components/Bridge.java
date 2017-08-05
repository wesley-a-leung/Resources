package algorithms.graph.components;

import java.util.HashSet;

import datastructures.graph.Graph;

/**
 *  Identifies bridge edges. This decomposes a directed graph into
 *  two-edge connected components.
 *  Runs in O(E + V) time.
 *
 *  Key quantity:  low[v] = minimum DFS preorder number of v
 *  and the set of vertices w for which there is a back edge (x, w)
 *  with x a descendant of v and w an ancestor of v.
 *
 *  Note: code assumes no parallel edges, e.g., two parallel edges
 *  would be (incorrectly) identified as bridges.
 */
public class Bridge {
    private int bridges;      // number of bridges
    private int cnt;          // counter
    private int[] pre;        // pre[v] = order in which dfs examines v
    private int[] low;        // low[v] = lowest preorder of any vertex connected to v
    private HashSet<Integer>[] bridgeList;  // adjacency bridge list

    public Bridge(Graph G) {
        bridgeList = new HashSet[G.V()];
        low = new int[G.V()];
        pre = new int[G.V()];
        for (int v = 0; v < G.V(); v++) {
            low[v] = -1;
            pre[v] = -1;
            bridgeList[v] = new HashSet<Integer>();
        }
        
        for (int v = 0; v < G.V(); v++)
            if (pre[v] == -1)
                dfs(G, v, v);
    }

    public int components() { return bridges + 1; }
    
    public int countBridges() { return bridges; }

    private void dfs(Graph G, int u, int v) {
        pre[v] = cnt++;
        low[v] = pre[v];
        for (int w : G.adj(v)) {
            if (pre[w] == -1) {
                dfs(G, v, w);
                low[v] = Math.min(low[v], low[w]);
                if (low[w] == pre[w]) {
                    bridgeList[v].add(w);
                    bridges++;
                }
            }

            // update low number - ignore reverse of edge leading to v
            else if (w != u)
                low[v] = Math.min(low[v], pre[w]);
        }
    }
    
    public boolean isBridge(int v, int w) {
        return bridgeList[v].contains(w);
    }
}
