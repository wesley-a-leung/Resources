#ifndef ALGORITHMS_GRAPH_SHORTESTPATH_BELLMANFORDDIRECTEDSP_H_
#define ALGORITHMS_GRAPH_SHORTESTPATH_BELLMANFORDDIRECTEDSP_H_

#include <bits/stdc++.h>
#include <datastructures/graph/WeightedDigraph.h>

using namespace std;

typedef double unit;

unit *distTo;
DirectedWeightedEdge **edgeTo;

// takes time proportional to VE and space proportional to V
void bellmanFordSP(WeightedDigraph *G, int s) {
    distTo = new unit[G->getV()];
    edgeTo = new DirectedWeightedEdge *[G->getV()];
    for (int v = 0; v < G->getV(); v++) {
        distTo[v] = numeric_limits<unit>::max();
    }
    distTo[s] = 0.0;
    vector<DirectedWeightedEdge*> edges = G->edges();
    for (int i = 0; i < G->getV() - 1; i++) {
        for (DirectedWeightedEdge *e : edges) {
            int v = e->from();
            int w = e->to();
            if (distTo[w] > distTo[v] + e->getWeight()) {
                distTo[w] = distTo[v] + e->getWeight();
                edgeTo[w] = e;
            }
        }
    }

    for (DirectedWeightedEdge *e : G->edges()) {
        if (distTo[e->to()] > distTo[e->from()] + e->getWeight()) throw runtime_error("Graph has a negative cycle.");
    }
}

void bellmanFordSP(int V, vector<DirectedWeightedEdge*> &edges, int s) {
    distTo = new unit[V];
    edgeTo = new DirectedWeightedEdge *[V];
    for (int v = 0; v < V; v++) {
        distTo[v] = numeric_limits<unit>::max();
    }
    distTo[s] = 0.0;
    for (int i = 0; i < V - 1; i++) {
        for (DirectedWeightedEdge *e : edges) {
            int v = e->from();
            int w = e->to();
            if (distTo[w] > distTo[v] + e->getWeight()) {
                distTo[w] = distTo[v] + e->getWeight();
                edgeTo[w] = e;
            }
        }
    }

    for (DirectedWeightedEdge *e : edges) {
        if (distTo[e->to()] > distTo[e->from()] + e->getWeight()) throw runtime_error("Graph has a negative cycle.");
    }
}

#endif /* ALGORITHMS_GRAPH_SHORTESTPATH_BELLMANFORDDIRECTEDSP_H_ */
