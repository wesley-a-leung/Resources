#ifndef ALGORITHMS_GRAPH_SHORTESTPATH_SPFAUNDIRECTED_H_
#define ALGORITHMS_GRAPH_SHORTESTPATH_SPFAUNDIRECTED_H_

#include <bits/stdc++.h>
#include "datastructures/graph/WeightedGraph.h"

using namespace std;

typedef double unit;
const unit INF = numeric_limits<unit>::infinity();
// rules for INF (infinity): (length of the longest path + length of greatest weight) < INF

unit *distTo;
WeightedEdge **edgeTo;
bool *inQueue;

// takes time proportional to VE in the worst case and E on average and space proportional to V
void spfa(WeightedGraph *G, int s) {
    deque<int> q;
    distTo = new unit[G->getV()];
    edgeTo = new WeightedEdge *[G->getV()];
    inQueue = new bool[G->getV()];
    for (int v = 0; v < G->getV(); v++) {
        distTo[v] = INF;
        edgeTo[v] = nullptr;
        inQueue[v] = false;
    }
    distTo[s] = 0;
    inQueue[s] = true;
    q.push_back(s);
    while (!q.empty()) {
        int v = q.front();
        q.pop_front();
        inQueue[v] = false;
        for (WeightedEdge *e : G->adj(v)) {
            int w = e->other(v);
            if (distTo[w] > distTo[v] + e->getWeight()) {
                distTo[w] = distTo[v] + e->getWeight();
                edgeTo[w] = e;
                if (!inQueue[w]) {
                    if (!q.empty() && distTo[w] < distTo[q.front()]) q.push_front(w);
                    else q.push_back(w);
                    inQueue[w] = true;
                }
            }
        }
    }
}

#endif /* ALGORITHMS_GRAPH_SHORTESTPATH_SPFAUNDIRECTED_H_ */
