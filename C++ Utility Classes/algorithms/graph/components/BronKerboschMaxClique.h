#ifndef ALGORITHMS_GRAPH_COMPONENTS_BRONKERBOSCHMAXCLIQUE_H_
#define ALGORITHMS_GRAPH_COMPONENTS_BRONKERBOSCHMAXCLIQUE_H_

#include <bits/stdc++.h>
#include "datastructures/graph/Graph.h"
using namespace std;

/**
 * The Bron-Kerbosch algorithm is an algorithm for finding maximal cliques in an undirected graph using recursive backtracking.
 * A clique is defined to be a subset of a graph such that every pair of vertices in the subset are connected by an edge.
 *
 * Time complexity: O(3^(n/3))
 *
 */
class BronKerboschMaxClique {
private:
    bool **adj;
    int *W;
    int maxClique;

    int solve(int nodes, int curr, int pool, int excl) {
        if (pool == 0 && excl == 0) {
            int cnt = 0;
            for (int i = 0; i < nodes; i++)
                if ((curr & 1LL << i) > 0) cnt += W[i];
            return cnt;
        }
        int res = 0;
        int j = 0;
        for (int i = 0; i < nodes; i++)
            if ((pool & 1LL << i) > 0 || (excl & 1LL << i) > 0) j = i;
        for (int i = 0; i < nodes; i++) {
            if ((pool & 1LL << i) == 0 || adj[i][j]) continue;
            int ncurr = curr, npool = 0, nexcl = 0;
            ncurr |= 1LL << i;
            for (int k = 0; k < nodes; k++) {
                if (adj[i][k]) {
                    npool |= pool & 1LL << k;
                    nexcl |= excl & 1LL << k;
                }
            }
            res = max(res, solve(nodes, ncurr, npool, nexcl));
            pool &= ~(1LL << i);
            excl |= 1 >> i;
        }
        return res;
    }

public:
    /**
     * Adjacency matrix graph constructor.
     *
     * @param G an adjacency matrix graph
     * @param V the number of vertices
     */
    BronKerboschMaxClique(bool **G, int V) {
        adj = new bool*[V];
        W = new int[V];
        for (int v = 0; v < V; v++) {
            W[v] = 1;
            adj[v] = new bool[V];
            for (int w = 0; w < V; w++) adj[v][w] = G[v][w];
        }
        maxClique = solve(V, 0, (int) ((1LL << V) - 1), 0);
    }

    /**
     * Adjacency list graph constructor.
     *
     * @param G an adjacency list graph
     */
    BronKerboschMaxClique(Graph *G) {
        adj = new bool*[G->getV()];
        W = new int[G->getV()];
        for (int v = 0; v < G->getV(); v++) {
            W[v] = 1;
            adj[v] = new bool[G->getV()];
            for (int w = 0; w < G->getV(); w++) adj[v][w] = 0;
            for (int w : G->adj(v)) adj[v][w] = true;
        }
        maxClique = solve(G->getV(), 0, (int) ((1LL << G->getV()) - 1), 0);
    }

    /**
     * Adjacency matrix graph constructor with vertex weights.
     *
     * @param G an adjacency matrix graph
     * @param V the number of vertices
     * @param weight the weights of the vertices
     */
    BronKerboschMaxClique(bool **G, int V, int *weight) {
        adj = new bool*[V];
        W = new int[V];
        for (int v = 0; v < V; v++) {
            W[v] = weight[v];
            adj[v] = new bool[V];
            for (int w = 0; w < V; w++) adj[v][w] = G[v][w];
        }
        maxClique = solve(V, 0, (int) ((1LL << V) - 1), 0);
    }

    /**
     * Adjacency list graph constructor.
     *
     * @param G an adjacency list graph
     * @param weight the weights of the vertices
     */
    BronKerboschMaxClique(Graph *G, int *weight) {
        adj = new bool*[G->getV()];
        W = new int[G->getV()];
        for (int v = 0; v < G->getV(); v++) {
            W[v] = weight[v];
            adj[v] = new bool[G->getV()];
            for (int w = 0; w < G->getV(); w++) adj[v][w] = 0;
            for (int w : G->adj(v)) adj[v][w] = true;
        }
        maxClique = solve(G->getV(), 0, (int) ((1LL << G->getV()) - 1), 0);
    }

    int getMaxClique() {
        return maxClique;
    }
};

#endif /* ALGORITHMS_GRAPH_COMPONENTS_BRONKERBOSCHMAXCLIQUE_H_ */
