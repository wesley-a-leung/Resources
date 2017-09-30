#ifndef ALGORITHMS_GRAPH_SEARCH_HLD_H_
#define ALGORITHMS_GRAPH_SEARCH_HLD_H_

#include <bits/stdc++.h>
#include <datastructures/graph/Graph.h>

using namespace std;

/**
 * The {@code HLD} class represents a data structure for computing queries on graphs that involve finding the
 * lowest common ancestor of 2 nodes in an acyclic graph using heavy-light decomposition.
 * The constructor takes time proportional to <em>V</em> +  <em>E</em> and afterwards,
 * the {@code lca()} method takes log <em>V</em> time to find the lowest common ancestor.
 *
 * @author Wesley Leung
 */
class HLD {
private:
    int *depth, *parent, *chain, *size, *head, *base, *vertex;
    int chainNum, baseNum;

    void dfs(Graph *G, int v, int d, int prev) {
        depth[v] = d;
        parent[v] = prev;
        size[v] = 1;
        for (int w: G->adj(v)) {
            if (w != prev) {
                dfs(G, w, d + 1, v);
                size[v] += size[w];
            }
        }
    }

    void hld(Graph *G, int v, int prev) {
        if (head[chainNum] == -1) head[chainNum] = v;
        chain[v] = chainNum;
        base[v] = baseNum;
        vertex[baseNum++] = v;
        int maxIndex = -1;
        for (int w: G->adj(v)) {
            if (w != prev && (maxIndex == -1 || size[maxIndex] < size[w])) maxIndex = w;
        }
        if (maxIndex != -1) hld(G, maxIndex, v);
        for (int w: G->adj(v)) {
            if (w != prev && w != maxIndex) {
                chainNum++;
                hld(G, w, v);
            }
        }
    }

    int merge(int a, int b); // to be implemented
    int query(int l, int r, bool reverse); // to be implemented;

    int queryUp(int v, int w, bool reverse, bool includeW) {
        int ans = 0;
        while (chain[v] != chain[w]) {
            ans = reverse ? merge(ans, query(base[head[chain[v]]], base[v], reverse)) : merge(query(base[head[chain[v]]], base[v], reverse), ans);
            v = parent[head[chain[v]]];
        }
        if (!includeW && v == w) return ans;
        return reverse ? merge(ans, query(base[w] + !includeW, base[v], reverse)) : merge(query(base[w] + !includeW, base[v], reverse), ans);
    }

public:
    HLD(Graph *G) {
        depth = new int[G->getV()];
        parent = new int[G->getV()];
        chain = new int[G->getV()];
        size = new int[G->getV()];
        head = new int[G->getV()];
        base = new int[G->getV()];
        vertex = new int[G->getV()];
        chainNum = 0;
        baseNum = 1;
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

    int queryPath(int v, int w) {
        int lcaVertex = lca(v, w);
        return merge(queryUp(v, lcaVertex, true, false), queryUp(w, lcaVertex, false, true));
    }
};

#endif /* ALGORITHMS_GRAPH_SEARCH_HLD_H_ */
