#ifndef ALGORITHMS_GRAPH_SHORTESTPATH_CLASSICALDIJKSTRADIRECTEDSP_H_
#define ALGORITHMS_GRAPH_SHORTESTPATH_CLASSICALDIJKSTRADIRECTEDSP_H_

#include <bits/stdc++.h>
#include <datastructures/graph/WeightedDigraph.h>

using namespace std;

typedef double unit;
const unit INF = numeric_limits<unit>::infinity();
// rules for INF (infinity): (length of the longest path + length of greatest weight) < INF

unit *distTo;
bool *marked;
DirectedWeightedEdge **edgeTo;

// takes time proportional to V^2 and space proportional to V
// does not work with negative weights
void dijkstraSP(WeightedDigraph *G, int s) {
    distTo = new unit[G->getV()];
    edgeTo = new DirectedWeightedEdge *[G->getV()];
    marked = new bool[G->getV()];
    for (int v = 0; v < G->getV(); v++) {
        distTo[v] = INF;
        marked[v] = false;
    }
    distTo[s] = 0;
    for (int v = 0; v < G->getV() - 1; v++) {
        int minV = -1;
        for (int w = 0; w < G->getV(); w++) {
            if (!marked[w] && (minV == -1 || distTo[minV] > distTo[w])) minV = w;
        }
        marked[minV] = true;
        for (DirectedWeightedEdge *e : G->adj(minV)) {
            int w = e->to();
            if (distTo[w] > distTo[minV] + e->getWeight()) {
                distTo[w] = distTo[minV] + e->getWeight();
                edgeTo[w] = e;
            }
        }
    }
}

#endif /* ALGORITHMS_GRAPH_SHORTESTPATH_CLASSICALDIJKSTRADIRECTEDSP_H_ */
