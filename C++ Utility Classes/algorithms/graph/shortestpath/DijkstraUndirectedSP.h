/*
 * DijkstraUndirectedSP.h
 *
 *  Created on: Jul 8, 2017
 *      Author: Wesley Leung
 */

#ifndef ALGORITHMS_GRAPH_SHORTESTPATH_DIJKSTRAUNDIRECTEDSP_H_
#define ALGORITHMS_GRAPH_SHORTESTPATH_DIJKSTRAUNDIRECTEDSP_H_

#include <bits/stdc++.h>
#include <datastructures/graph/WeightedEdge.h>
#include <datastructures/graph/WeightedGraph.h>

using namespace std;

double *distTo;
WeightedEdge **edgeTo;
priority_queue<pair<double, int>, vector<pair<double, int>> , greater<pair<double, int>>> pq;

void dijkstraSP(WeightedGraph *G, int s) {
    distTo = new double[G->getV()];
    edgeTo = new WeightedEdge *[G->getV()];
    for (int v = 0; v < G->getV(); v++) {
        distTo[v] = numeric_limits<double>::infinity();
    }
    distTo[s] = 0.0;
    pq.push({distTo[s], s});
    while (!pq.empty()) {
        int v = pq.top().second;
        pq.pop();
        for (WeightedEdge *e : G->adj(v)) {
            int w = e->other(v);
            if (distTo[w] > distTo[v] + e->getWeight()) {
                distTo[w] = distTo[v] + e->getWeight();
                edgeTo[w] = e;
                pq.push({distTo[w], w});
            }
        }
    }
}

#endif /* ALGORITHMS_GRAPH_SHORTESTPATH_DIJKSTRAUNDIRECTEDSP_H_ */
