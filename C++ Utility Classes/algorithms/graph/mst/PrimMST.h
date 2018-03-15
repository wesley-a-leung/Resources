#ifndef ALGORITHMS_GRAPH_MST_PRIMMST_H_
#define ALGORITHMS_GRAPH_MST_PRIMMST_H_

#include <bits/stdc++.h>
#include "datastructures/graph/WeightedGraph.h"

using namespace std;

typedef double unit;
const unit INF = numeric_limits<unit>::infinity();

class PrimMST {
private:
    unit weight;
    vector<WeightedEdge*> mst;
    bool *marked;
    unit *cost;
    WeightedEdge **edgeTo;
    priority_queue<pair<unit, int>, vector<pair<unit, int>>, greater<pair<unit, int>>> PQ;

public:
    /**
     * Compute a minimum spanning tree (or forest) of an edge-weighted graph.
     * @param G the edge-weighted graph
     */
    PrimMST(WeightedGraph *G) {
        weight = (unit) 0;
        marked = new bool[G->getV()];
        cost = new unit[G->getV()];
        edgeTo = new WeightedEdge*[G->getV()];
        for (int v = 0; v < G->getV(); v++) {
            marked[v] = false;
            cost[v] = INF;
            edgeTo[v] = nullptr;
        }
        for (int s = 0; s < G->getV(); s++) {
            if (marked[s]) continue;
            cost[s] = 0;
            PQ.push({cost[s], s});
            while (!PQ.empty()) {
                int v = PQ.top().second;
                PQ.pop();
                marked[v] = true;
                for (WeightedEdge *e : G->adj(v)) {
                    int w = e->other(v);
                    if (marked[w] || e->getWeight() >= cost[w]) continue;
                    cost[w] = e->getWeight();
                    edgeTo[w] = e;
                    PQ.push({cost[w], w});
                }
            }
        }
        for (int v = 0; v < G->getV(); v++) {
            if (edgeTo[v] == nullptr) {
                mst.push_back(edgeTo[v]);
                weight += edgeTo[v]->getWeight();
            }
        }
    }

    /**
     * Returns the edges in a minimum spanning tree (or forest).
     * @return the edges in a minimum spanning tree (or forest) as
     *    an iterable of edges
     */
    vector<WeightedEdge*> &edges() {
        return mst;
    }

    /**
     * Returns the sum of the edge weights in a minimum spanning tree (or forest).
     * @return the sum of the edge weights in a minimum spanning tree (or forest)
     */
    unit getWeight() {
        return weight;
    }
};

#endif /* ALGORITHMS_GRAPH_MST_PRIMMST_H_ */
