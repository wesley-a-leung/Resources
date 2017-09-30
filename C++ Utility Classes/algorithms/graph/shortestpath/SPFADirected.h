#ifndef ALGORITHMS_GRAPH_SHORTESTPATH_SPFADIRECTED_H_
#define ALGORITHMS_GRAPH_SHORTESTPATH_SPFADIRECTED_H_

#include <bits/stdc++.h>
#include <datastructures/graph/WeightedDigraph.h>

using namespace std;

double *distTo;
DirectedWeightedEdge **edgeTo;
bool *inQueue;

// takes time proportional to VE in the worst case and E on average and space proportional to V
void spfa(WeightedDigraph *G, int s) {
    deque<int> q;
    distTo = new double[G->getV()];
    edgeTo = new DirectedWeightedEdge *[G->getV()];
    inQueue = new bool[G->getV()];
    for (int v = 0; v < G->getV(); v++) {
        distTo[v] = numeric_limits<double>::infinity();
        inQueue[v] = false;
    }
    distTo[s] = 0.0;
    inQueue[s] = true;
    q.push_back(s);
    while (!q.empty()) {
        int v = q.front();
        q.pop_front();
        inQueue[v] = false;
        for (DirectedWeightedEdge *e : G->adj(v)) {
            int w = e->to();
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

#endif /* ALGORITHMS_GRAPH_SHORTESTPATH_SPFADIRECTED_H_ */
