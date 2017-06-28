/*
 * EdmondsKarpMaxFlow.h
 *
 *  Created on: Jun 27, 2017
 *      Author: Wesley Leung
 */

#ifndef ALGORITHMS_GRAPH_NETWORKFLOW_EDMONDSKARPMAXFLOW_H_
#define ALGORITHMS_GRAPH_NETWORKFLOW_EDMONDSKARPMAXFLOW_H_

#include <bits/stdc++.h>
#include <datastructures/graph/networkflow/FlowEdge.h>
#include <datastructures/graph/networkflow/FlowNetwork.h>

using namespace std;

class EdmondsKarpMaxFlow {
private:
    static const double FLOATING_POINT_EPSILON = 1E-11;
    int V;          // number of vertices
    bool *marked;     // marked[v] = true iff s->v path in residual graph
    FlowEdge* edgeTo;    // edgeTo[v] = last edge on shortest residual s->v path
    double value;         // current value of max flow

    // is there an augmenting path?
    // if so, upon termination edgeTo[] will contain a parent-link representation of such a path
    // this implementation finds a shortest augmenting path (fewest number of edges),
    // which performs well both in theory and in practice
    bool hasAugmentingPath(FlowNetwork *G, int s, int t) {
        edgeTo = new FlowEdge*[G->V()];
        marked = new bool*[G->V()];

        // breadth-first search
        queue<int> q = new queue<int>();
        q.push(s);
        marked[s] = true;
        while (!q.empty() && !marked[t]) {
            int v = q.front();
            q.pop();

            for (FlowEdge e : G->adj(v)) {
                int w = e.other(v);

                // if residual capacity from v to w
                if (e.residualCapacityTo(w) > 0) {
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
        double excess = 0.0;
        for (FlowEdge e : G->adj(v)) {
            if (v == e.from()) excess -= e.flow();
            else               excess += e.flow();
        }
        return excess;
    }


public:
    /**
     * Compute a maximum flow and minimum cut in the network {@code G}
     * from vertex {@code s} to vertex {@code t}.
     *
     * @param  G the flow network
     * @param  s the source vertex
     * @param  t the sink vertex
     * @throws invalid_argument unless {@code 0 <= s < V}
     * @throws invalid_argument unless {@code 0 <= t < V}
     * @throws invalid_argument if {@code s == t}
     * @throws invalid_argument if initial flow is infeasible
     */
    EdmondsKarpMaxFlow(FlowNetwork *G, int s, int t) {
        V = G->getV();
        if (s == t)               throw invalid_argument("Source equals sink");

        // while there exists an augmenting path, use it
        value = excess(G, t);
        while (hasAugmentingPath(G, s, t)) {

            // compute bottleneck capacity
            double bottle = numeric_limits<double>::infinity();
            for (int v = t; v != s; v = edgeTo[v].other(v)) {
                bottle = min(bottle, edgeTo[v].residualCapacityTo(v));
            }

            // augment flow
            for (int v = t; v != s; v = edgeTo[v].other(v)) {
                edgeTo[v].addResidualFlowTo(v, bottle);
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
     * @throws invalid_argument unless {@code 0 <= v < V}
     */
    bool inCut(int v)  {
        return marked[v];
    }
};

#endif /* ALGORITHMS_GRAPH_NETWORKFLOW_EDMONDSKARPMAXFLOW_H_ */
