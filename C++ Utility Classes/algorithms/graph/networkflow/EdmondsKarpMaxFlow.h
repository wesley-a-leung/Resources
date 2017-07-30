/*
 * EdmondsKarpMaxFlow.h
 *
 *  Created on: Jun 27, 2017
 *      Author: Wesley Leung
 */

#ifndef ALGORITHMS_GRAPH_NETWORKFLOW_EDMONDSKARPMAXFLOW_H_
#define ALGORITHMS_GRAPH_NETWORKFLOW_EDMONDSKARPMAXFLOW_H_

#include <bits/stdc++.h>
#include <datastructures/graph/networkflow/FlowNetwork.h>

using namespace std;

class EdmondsKarpMaxFlow {
private:
    bool *marked;     // marked[v] = true if s->v path in residual graph
    FlowEdge **edgeTo;    // edgeTo[v] = last edge on shortest residual s->v path
    double value;         // current value of max flow

    // is there an augmenting path?
    // if so, upon termination edgeTo[] will contain a parent-link representation of such a path
    // this implementation finds a shortest augmenting path (fewest number of edges),
    // which performs well both in theory and in practice
    bool hasAugmentingPath(FlowNetwork *G, int s, int t) {
        for (int i = 0; i < G->getV(); i++) {
            edgeTo[i] = nullptr;
            marked[i] = false;
        }
        // breadth-first search
        queue<int> q;
        q.push(s);
        marked[s] = true;
        while (!q.empty() && !marked[t]) {
            int v = q.front();
            q.pop();

            for (FlowEdge *e : G->adj(v)) {
                int w = e->other(v);

                // if residual capacity from v to w
                if (e->residualCapacityTo(w) > 0) {
                    if (!marked[w]) {
                        edgeTo[w] = e;
                        marked[w] = true;
                        q.push(w);
                    }
                }
            }
        }

        // is there an augmenting path?
        return marked[t];
    }

    // return excess flow at vertex v
    double excess(FlowNetwork *G, int v) {
        double excessCapacity = 0.0;
        for (FlowEdge *e : G->adj(v)) {
            if (v == e->from()) excessCapacity -= e->getFlow();
            else               excessCapacity += e->getFlow();
        }
        return excessCapacity;
    }


public:
    /**
     * Compute a maximum flow and minimum cut in the network {@code G}
     * from vertex {@code s} to vertex {@code t}.
     *
     * @param  G the flow network
     * @param  s the source vertex
     * @param  t the sink vertex
     * @throws invalid_argument if {@code s == t}
     */
    EdmondsKarpMaxFlow(FlowNetwork *G, int s, int t) {
        if (s == t)               throw invalid_argument("Source equals sink");
        edgeTo = new FlowEdge*[G->getV()];
        marked = new bool[G->getV()];
        // while there exists an augmenting path, use it
        value = excess(G, t);
        while (hasAugmentingPath(G, s, t)) {
            // compute bottleneck capacity
            double bottle = numeric_limits<double>::infinity();
            for (int v = t; v != s; v = edgeTo[v]->other(v)) {
                bottle = min(bottle, edgeTo[v]->residualCapacityTo(v));
            }

            // augment flow
            for (int v = t; v != s; v = edgeTo[v]->other(v)) {
                edgeTo[v]->addResidualFlowTo(v, bottle);
            }

            value += bottle;
        }
    }

    /**
     * Returns the value of the maximum flow.
     *
     * @return the value of the maximum flow
     */
    double getValue()  {
        return value;
    }

    /**
     * Returns true if the specified vertex is on the {@code s} side of the mincut.
     *
     * @param  v vertex
     * @return {@code true} if vertex {@code v} is on the {@code s} side of the micut;
     *         {@code false} otherwise
     */
    bool inCut(int v)  {
        return marked[v];
    }
};

#endif /* ALGORITHMS_GRAPH_NETWORKFLOW_EDMONDSKARPMAXFLOW_H_ */
