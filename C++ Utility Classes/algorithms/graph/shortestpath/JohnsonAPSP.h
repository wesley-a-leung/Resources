#ifndef ALGORITHMS_GRAPH_SHORTESTPATH_JOHNSONAPSP_H_
#define ALGORITHMS_GRAPH_SHORTESTPATH_JOHNSONAPSP_H_

#include <bits/stdc++.h>
#include "datastructures/graph/WeightedDigraph.h"

using namespace std;

typedef double unit;
const unit INF = numeric_limits<unit>::infinity();
// rules for INF (infinity): (length of the longest path + length of greatest weight) < INF

unit **distTo;
DirectedWeightedEdge ***edgeTo;

// takes time proportional to VE log E + VE and space proportional to V^2
void johnsonAPSP(WeightedDigraph *G) {
    distTo = new unit *[G->getV()];
    edgeTo = new DirectedWeightedEdge **[G->getV()];
    unit h[G->getV() + 1];
    for (int i = 0; i < G->getV(); i++) {
        distTo[i] = new unit[G->getV()];
        edgeTo[i] = new DirectedWeightedEdge *[G->getV()];
    }

    vector<DirectedWeightedEdge*> edges = G->edges();

    for (int v = 0; v < G->getV(); v++) {
        h[v] = INF;
        // add edge from dummy node to each node
        edges.push_back(new DirectedWeightedEdge(G->getV(), v, 0));
        for (int w = 0; w < G->getV(); w++) {
            distTo[v][w] = INF;
        }
    }

    // bellman-ford from the dummy node
    h[G->getV()] = 0;
    for (int i = 0; i < G->getV(); i++) {
        for (DirectedWeightedEdge *e : edges) {
            int v = e->from();
            int w = e->to();
            if (h[w] > h[v] + e->getWeight()) {
                h[w] = h[v] + e->getWeight();
            }
        }
    }

    // detect negative cycle
    for (DirectedWeightedEdge *e : edges) {
        if (h[e->to()] > h[e->from()] + e->getWeight()) {
            throw runtime_error("Graph has a negative cycle.");
        }
    }

    // dijkstra from each node using the reweighted graph
    for (int s = 0; s < G->getV(); s++) {
        priority_queue<pair<unit, int>, vector<pair<unit, int>> , greater<pair<unit, int>>> pq;
        distTo[s][s] = 0;
        pq.push({distTo[s][s], s});
        while (!pq.empty()) {
            int v = pq.top().second;
            pq.pop();
            for (DirectedWeightedEdge *e : G->adj(v)) {
                int w = e->to();
                if (distTo[s][w] > distTo[s][v] + e->getWeight() + h[v] - h[w]) {
                    distTo[s][w] = distTo[s][v] + e->getWeight() + h[v] - h[w];
                    edgeTo[s][w] = e;
                    pq.push({distTo[s][w], w});
                }
            }
        }
    }

    // readjust weights
    for (int v = 0; v < G->getV(); v++) {
        for (int w = 0; w < G->getV(); w++) {
            distTo[v][w] = distTo[v][w] - h[v] + h[w];
        }
    }
}

#endif /* ALGORITHMS_GRAPH_SHORTESTPATH_JOHNSONAPSP_H_ */
