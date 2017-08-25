/*
 * PushRelabel.h
 *
 *  Created on: Jul 30, 2017
 *      Author: Wesley Leung
 */

#ifndef ALGORITHMS_GRAPH_NETWORKFLOW_PUSHRELABELMAXFLOW_H_
#define ALGORITHMS_GRAPH_NETWORKFLOW_PUSHRELABELMAXFLOW_H_

#include <bits/stdc++.h>
#include <datastructures/graph/networkflow/FlowNetwork.h>

using namespace std;

// DISCLAIMER: THIS IS UNTESTED. MAY ONLY BE USEFUL FOR FINDING THE MAX-FLOW VALUE, NOT THE FLOW OF THE ACTUAL EDGES.
/**
 * The {@code PushRelableMaxFlow} class represents a data type for computing a
 * <em>maximum st-flow</em> in a flow network.
 * This implementation uses the FIFO selection rule to reduce the running time
 * to <em>V</em><sup>3</sup>
 *
 * @author Wesley Leung
 */
class PushRelabelMaxFlow {
private:
    const double FLOATING_POINT_EPSILON = 1E-10;
    double value;
    int *height;
    double *excess;
    bool *done;

public:
    /**
     * Compute a maximum flow in the network {@code G}
     * from vertex {@code s} to vertex {@code t}.
     *
     * @param  G the flow network
     * @param  s the source vertex
     * @param  t the sink vertex
     * @throws invalid_argument if {@code s == t}
     */
    PushRelabelMaxFlow(FlowNetwork *G, int s, int t) {
        if (s == t) throw invalid_argument("Source equals sink");
        queue<int> q;
        height = new int[G->getV()];
        excess = new double[G->getV()];
        done = new bool[G->getV()];
        for (int v = 0; v < G->getV(); v++) {
            height[v] = 0;
            excess[v] = 0.0;
            done[v] = false;
        }
        height[s] = G->getV();
        for (FlowEdge *e : G->adj(s)) {
            int v = e->other(s);
            if (v == e->to()) {
                e->addResidualFlowTo(v, e->getCapacity());
                excess[s] -= e->getCapacity();
                excess[v] += e->getCapacity();
            }
            if (v != t) {
                done[v] = true;
                q.push(v);
            }
        }

        while (!q.empty()) {
            int v = q.front();
            int tempHeight = 2 * G->getV();
            for (FlowEdge *e : G->adj(v)) {
                if (abs(excess[v]) <= FLOATING_POINT_EPSILON) break;
                int w = e->other(v);
                if (e->residualCapacityTo(w) > FLOATING_POINT_EPSILON) {
                    if (height[v] > height[w]) {
                        // PUSH
                        double tempFlow = min(excess[v], e->residualCapacityTo(w));
                        e->addResidualFlowTo(w, tempFlow);
                        excess[v] -= tempFlow;
                        excess[w] += tempFlow;
                        // END PUSH
                        if (!done[w] && w != s && w != t) {
                            done[w] = true;
                            q.push(w);
                        }
                    } else {
                        tempHeight = min(height[v], tempHeight);
                    }
                }
            }
            if (excess[v] > FLOATING_POINT_EPSILON) height[v] = tempHeight + 1;
            else {
                done[v] = false;
                q.pop();
            }
        }
        value = excess[t];
    }

    /**
     * Returns the value of the maximum flow.
     *
     * @return the value of the maximum flow
     */
    double getValue()  {
        return value;
    }
};

#endif /* ALGORITHMS_GRAPH_NETWORKFLOW_PUSHRELABELMAXFLOW_H_ */
