/*
 * DepthFirstSearch.h
 *
 *  Created on: Jul 2, 2017
 *      Author: Wesley Leung
 */

#ifndef ALGORITHMS_GRAPH_SEARCH_DEPTHFIRSTSEARCH_H_
#define ALGORITHMS_GRAPH_SEARCH_DEPTHFIRSTSEARCH_H_

#include <bits/stdc++.h>
#include <datastructures/graph/Graph.h>

using namespace std;

bool *marked;
int *depth;
int *edgeTo;

/**
 * Recursive function
 *
 * @param G the graph
 * @param v the current vertex
 * @param d the current depth
 */
void dfs(Graph *G, int v, int d) {
    marked[v] = true;
    depth[v] = d;
    for (int w : G->adj(v)) {
        if (marked[w]) continue;
        edgeTo[w] = v;
        dfs(G, w, d + 1);
    }
}

/**
 * Computes a path between {@code s} and every other vertex in graph {@code G}.
 * @param G the graph
 * @param v the source vertex
 */
void dfs(Graph *G, int s) {
    marked = new bool[G->getV()];
    depth = new int[G->getV()];
    edgeTo = new int[G->getV()];
    for (int v = 0; v < G->getV(); v++) {
        marked[v] = false;
        depth[v] = -1;
        edgeTo[v] = -1;
    }
    dfs(G, s, 0);
}

#endif /* ALGORITHMS_GRAPH_SEARCH_DEPTHFIRSTSEARCH_H_ */
