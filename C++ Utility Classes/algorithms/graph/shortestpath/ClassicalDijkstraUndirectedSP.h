/*
 * ClassicalDijkstraUndirectedSP.h
 *
 *  Created on: Aug 4, 2017
 *      Author: Wesley Leung
 */

#ifndef ALGORITHMS_GRAPH_SHORTESTPATH_CLASSICALDIJKSTRAUNDIRECTEDSP_H_
#define ALGORITHMS_GRAPH_SHORTESTPATH_CLASSICALDIJKSTRAUNDIRECTEDSP_H_

#include <bits/stdc++.h>
#include <datastructures/graph/WeightedGraph.h>

using namespace std;

double *distTo;
bool *marked;
WeightedEdge **edgeTo;

// takes time proportional to V ^ 2 and space proportional to V
// does not work with negative weights
void dijkstraSP(WeightedGraph *G, int s) {
    distTo = new double[G->getV()];
    edgeTo = new WeightedEdge *[G->getV()];
    marked = new bool[G->getV()];
    for (int v = 0; v < G->getV(); v++) {
        distTo[v] = numeric_limits<double>::infinity();
        marked[v] = false;
    }
    distTo[s] = 0.0;
    for (int v = 0; v < G->getV() - 1; v++) {
        int minV = -1;
        for (int w = 0; w < G->getV(); w++) {
            if (!marked[w] && (minV == -1 || distTo[minV] > distTo[w])) minV = w;
        }
        marked[minV] = true;
        for (WeightedEdge *e : G->adj(minV)) {
            int w = e->other(minV);
            if (distTo[w] > distTo[minV] + e->getWeight()) {
                distTo[w] = distTo[minV] + e->getWeight();
                edgeTo[w] = e;
            }
        }
    }
}

#endif /* ALGORITHMS_GRAPH_SHORTESTPATH_CLASSICALDIJKSTRAUNDIRECTEDSP_H_ */
