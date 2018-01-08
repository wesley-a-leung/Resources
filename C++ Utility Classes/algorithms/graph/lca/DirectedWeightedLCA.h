#ifndef ALGORITHMS_GRAPH_LCA_DIRECTEDWEIGHTEDLCA_H_
#define ALGORITHMS_GRAPH_LCA_DIRECTEDWEIGHTEDLCA_H_

#include <bits/stdc++.h>
#include <datastructures/graph/WeightedDigraph.h>

using namespace std;

/**
 * The {@code DirectedWeightedLCA} class represents a data structure for finding the
 * lowest common ancestor of 2 nodes in an acyclic digraph using heavy-light decomposition.
 * The constructor takes time proportional to <em>V</em> +  <em>E</em> and afterwards,
 * the {@code lca()} method takes log <em>V</em> time to find the lowest common ancestor.
 *
 * @author Wesley Leung
 */
class DirectedWeightedLCA {
private:
    int *depth, *parent, *chain, *size, *head;
    int chainNum;

    void dfs(WeightedDigraph *G, int v, int d, int prev) {
        depth[v] = d;
        parent[v] = prev;
        size[v] = 1;
        for (DirectedWeightedEdge *e: G->adj(v)) {
            int w = e->to();
            if (w != prev) {
                dfs(G, w, d + 1, v);
                size[v] += size[w];
            }
        }
    }

    void hld(WeightedDigraph *G, int v, int prev) {
        if (head[chainNum] == -1) head[chainNum] = v;
        chain[v] = chainNum;
        int maxIndex = -1;
        for (DirectedWeightedEdge *e: G->adj(v)) {
            int w = e->to();
            if (w != prev && (maxIndex == -1 || size[maxIndex] < size[w])) maxIndex = w;
        }
        if (maxIndex != -1) hld(G, maxIndex, v);
        for (DirectedWeightedEdge *e: G->adj(v)) {
            int w = e->to();
            if (w != prev && w != maxIndex) {
                chainNum++;
                hld(G, w, v);
            }
        }
    }

public:
    DirectedWeightedLCA(WeightedDigraph *G) {
        depth = new int[G->getV()];
        parent = new int[G->getV()];
        chain = new int[G->getV()];
        size = new int[G->getV()];
        head = new int[G->getV()];
        chainNum = 0;
        for (int i = 0; i < G->getV(); i++) {
            head[i] = -1;
        }
        dfs(G, 0, 0, -1);
        hld(G, 0, -1);
    }

    /**
     * Returns the lowest common ancestor of vertex {@code v} and {@code w}.
     *
     * @param v the first vertex
     * @param w the first vertex
     * @return the lowest common ancestor of vertex {@code v} and {@code w}
     */
    int lca(int v, int w) {
        while (chain[v] != chain[w]) {
            if (depth[head[chain[v]]] < depth[head[chain[w]]]) w = parent[head[chain[w]]];
            else v = parent[head[chain[v]]];
        }
        if (depth[v] < depth[w]) return v;
        return w;
    }
};

#endif /* ALGORITHMS_GRAPH_LCA_DIRECTEDWEIGHTEDLCA_H_ */
