/*
 * DinicMaxFlow.h
 *
 *  Created on: Jul 17, 2017
 *      Author: Wesley Leung
 */

#ifndef ALGORITHMS_GRAPH_NETWORKFLOW_DINICMAXFLOW_H_
#define ALGORITHMS_GRAPH_NETWORKFLOW_DINICMAXFLOW_H_

#include <bits/stdc++.h>
#include <datastructures/graph/networkflow/FlowNetwork.h>

using namespace std;

// DISCLAIMER: THIS IS UNTESTED. MAY ONLY BE USEFUL FOR FINDING THE MAX-FLOW VALUE, NOT THE FLOW OF THE ACTUAL EDGES.

/**
 * The {@code DinicMaxFlow} class represents a data type for computing a
 * <em>maximum st-flow</em> in a flow network.
 * This implementation uses the concepts of level graph and flow blocking.
 * The constructor takes time proportional to <em>V</em><sup>2</sup><em>E</em>
 * in the worst case and extra space (not including the network)
 * proportional to <em>V</em><sup>2</sup><em>E</em>, where <em>V</em> is the number of vertices
 * and <em>E</em> is the number of edges.
 * Afterwards, the {@code inCut()} and {@code value()} methods take
 * constant time.
 * <p>
 * If the capacities and initial flow values are all integers, then this
 * implementation guarantees to compute an integer-valued maximum flow.
 * If the capacities and floating-point numbers, then floating-point
 * roundoff error can accumulate.
 *
 * @author Wesley Leung
 */
class DinicMaxFlow {
private:
    double value;
    int* level;
    int* start;

    // is there an augmenting path?
    // this implementation finds a shortest augmenting path (fewest number of edges)
    bool hasAugmentingPath(FlowNetwork *G, int s, int t) {
        for (int v = 0; v < G->getV(); v++) {
            level[v] = -1;
        }
        level[s] = 0;
        queue<int> q;
        q.push(s);
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (FlowEdge *e : G->adj(v)) {
                int w = e->other(v);
                if (level[w] < 0 && e->residualCapacityTo(w) > 0) {
                    level[w] = level[v] + 1;
                    q.push(w);
                }
            }
        }
        return level[t] != -1;
    }

    // v : current vertex
    // t : sink
    // start : start[v] stores the number of edges that have been explored from v
    // flow : current flow sent from parent
    double sendFlow(FlowNetwork *G, int v, int t, int start[], double flow) {
        if (v == t) return flow;
        for ( ; start[v] < G->adj(v).size(); start[v]++) {
            FlowEdge *e = G->adj(v)[start[v]];
            int w = e->other(v);
            if (level[w] == level[v] + 1 && e->residualCapacityTo(w) > 0) {
                double curFlow = min(flow, e->residualCapacityTo(w));
                double tempFlow = sendFlow(G, w, t, start, curFlow);
                if (tempFlow > 0) {
                    e->addResidualFlowTo(w, tempFlow);
                    return tempFlow;
                }
            }
        }
        return 0.0;
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
    DinicMaxFlow(FlowNetwork *G, int s, int t) {
        if (s == t) throw invalid_argument("Source equals sink");
        level = new int[G->getV()];
        start = new int[G->getV()];
        value = 0.0;
        while (hasAugmentingPath(G, s, t)) {
            for (int v = 0; v < G->getV(); v++) {
                start[v] = 0;
            }
            double flow;
            while ((flow = sendFlow(G, s, t, start, numeric_limits<double>::infinity())) > 0) {
                value += flow;
            }
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
        return level[v] != -1;
    }
};

#endif /* ALGORITHMS_GRAPH_NETWORKFLOW_DINICMAXFLOW_H_ */
