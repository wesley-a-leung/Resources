#ifndef ALGORITHMS_GRAPH_SHORTESTPATH_DIJKSTRAUNDIRECTEDSP_H_
#define ALGORITHMS_GRAPH_SHORTESTPATH_DIJKSTRAUNDIRECTEDSP_H_

#include <bits/stdc++.h>
#include <datastructures/graph/WeightedGraph.h>

using namespace std;

typedef double unit;
const unit INF = numeric_limits<unit>::infinity();
// rules for INF (infinity): (length of the longest path + length of greatest weight) <= INF

unit *distTo;
WeightedEdge **edgeTo;

// takes time proportional to E log E and space proportional to V
// works with negative edges weights, but cannot detect negative cycles
void dijkstraSP(WeightedGraph *G, int s) {
    priority_queue<pair<unit, int>, vector<pair<unit, int>>, greater<pair<unit, int>>> PQ;
    distTo = new unit[G->getV()];
    edgeTo = new WeightedEdge *[G->getV()];
    for (int v = 0; v < G->getV(); v++) {
        distTo[v] = INF;
    }
    distTo[s] = 0;
    PQ.push({distTo[s], s});
    while (!PQ.empty()) {
        int v = PQ.top().second;
        PQ.pop();
        for (WeightedEdge *e : G->adj(v)) {
            int w = e->other(v);
            if (distTo[w] > distTo[v] + e->getWeight()) {
                distTo[w] = distTo[v] + e->getWeight();
                edgeTo[w] = e;
                PQ.push({distTo[w], w});
            }
        }
    }
}

#endif /* ALGORITHMS_GRAPH_SHORTESTPATH_DIJKSTRAUNDIRECTEDSP_H_ */
