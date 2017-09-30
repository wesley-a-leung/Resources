#ifndef ALGORITHMS_GRAPH_SHORTESTPATH_DIJKSTRADIRECTEDSP_H_
#define ALGORITHMS_GRAPH_SHORTESTPATH_DIJKSTRADIRECTEDSP_H_

#include <bits/stdc++.h>
#include <datastructures/graph/WeightedDigraph.h>

using namespace std;

double *distTo;
DirectedWeightedEdge **edgeTo;

// takes time proportional to E log E and space proportional to V
// works with negative edges weights, but cannot detect negative cycles
void dijkstraSP(WeightedDigraph *G, int s) {
    priority_queue<pair<double, int>, vector<pair<double, int>> , greater<pair<double, int>>> pq;
    distTo = new double[G->getV()];
    edgeTo = new DirectedWeightedEdge *[G->getV()];
    for (int v = 0; v < G->getV(); v++) {
        distTo[v] = numeric_limits<double>::infinity();
    }
    distTo[s] = 0.0;
    pq.push({distTo[s], s});
    while (!pq.empty()) {
        int v = pq.top().second;
        pq.pop();
        for (DirectedWeightedEdge *e : G->adj(v)) {
            int w = e->to();
            if (distTo[w] > distTo[v] + e->getWeight()) {
                distTo[w] = distTo[v] + e->getWeight();
                edgeTo[w] = e;
                pq.push({distTo[w], w});
            }
        }
    }
}

#endif /* ALGORITHMS_GRAPH_SHORTESTPATH_DIJKSTRADIRECTEDSP_H_ */
