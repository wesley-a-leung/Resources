#ifndef ALGORITHMS_GRAPH_NETWORKFLOW_EDMONDSKARPMAXFLOW_H_
#define ALGORITHMS_GRAPH_NETWORKFLOW_EDMONDSKARPMAXFLOW_H_

#include <bits/stdc++.h>
#include "datastructures/graph/networkflow/FlowNetwork.h"

using namespace std;

/**
 *  The {@code EdmondsKarpMaxFlow} class represents a data type for computing a
 *  <em>maximum st-flow</em> and <em>minimum st-cut</em> in a flow
 *  network.
 *  <p>
 *  This implementation uses the <em>Ford-Fulkerson</em> algorithm with
 *  the <em>Edmonds-Karp shortest augmenting path</em> heuristic.
 *  The constructor takes time proportional to <em>E V</em> (<em>E</em> + <em>V</em>)
 *  in the worst case and extra space (not including the network)
 *  proportional to <em>V</em>, where <em>V</em> is the number of vertices
 *  and <em>E</em> is the number of edges. In practice, the algorithm will
 *  run much faster.
 *  Afterwards, the {@code inCut()} and {@code value()} methods take
 *  constant time.
 *  <p>
 *  If the capacities and initial flow values are all integers, then this
 *  implementation guarantees to compute an integer-valued maximum flow.
 *  If the capacities and floating-point numbers, then floating-point
 *  roundoff error can accumulate.
 *  <p>
 *  For additional documentation,
 *  see <a href="http://algs4.cs.princeton.edu/64maxflow">Section 6.4</a> of
 *  <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne.
 *
 *  @author Robert Sedgewick
 *  @author Kevin Wayne
 */
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
        value = 0;
        // while there exists an augmenting path, use it
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
