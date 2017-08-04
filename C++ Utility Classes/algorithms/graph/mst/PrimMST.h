/*
 * PrimMST.h
 *
 *  Created on: Aug 4, 2017
 *      Author: Wesley Leung
 */

#ifndef ALGORITHMS_GRAPH_MST_PRIMMST_H_
#define ALGORITHMS_GRAPH_MST_PRIMMST_H_

#include <bits/stdc++.h>
#include <datastructures/graph/WeightedGraph.h>

using namespace std;

/**
 *  The {@code PrimMST} class represents a data type for computing a
 *  <em>minimum spanning tree</em> in an edge-weighted graph.
 *  The edge weights can be positive, zero, or negative and need not
 *  be distinct. If the graph is not connected, it computes a <em>minimum
 *  spanning forest</em>, which is the union of minimum spanning trees
 *  in each connected component. The {@code weight()} method returns the
 *  weight of a minimum spanning tree and the {@code edges()} method
 *  returns its edges.
 *  <p>
 *  This implementation uses a lazy version of <em>Prim's algorithm</em>
 *  with a binary heap of edges.
 *  The constructor takes time proportional to <em>E</em> log <em>E</em>
 *  and extra space (not including the graph) proportional to <em>E</em>,
 *  where <em>V</em> is the number of vertices and <em>E</em> is the number of edges.
 *  Afterwards, the {@code weight()} method takes constant time
 *  and the {@code edges()} method takes time proportional to <em>V</em>.
 *  <p>
 *  For additional documentation,
 *  see <a href="http://algs4.cs.princeton.edu/43mst">Section 4.3</a> of
 *  <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne.
 *  For alternate implementations, see {@link KruskalMST}, and {@link BoruvkaMST}.
 *
 *  @author Robert Sedgewick
 *  @author Kevin Wayne
 */
class PrimMST {
private:
    double weight;       // total weight of MST
    vector<WeightedEdge*> mst;     // edges in the MST
    bool *marked;    // marked[v] = true if v on tree
    priority_queue<WeightedEdge*, vector<WeightedEdge*>, WeightedEdge_greater> pq;      // edges with one endpoint in tree

    // add all edges e incident to v onto pq if the other endpoint has not yet been scanned
    void scan(WeightedGraph *G, int v) {
        marked[v] = true;
        for (WeightedEdge *e : G->adj(v)) {
            if (!marked[e->other(v)]) pq.push(e);
        }
    }

    // run Prim's algorithm
    void prim(WeightedGraph *G, int s) {
        while (!pq.empty()) {                          // better to stop when mst has V-1 edges
            WeightedEdge *e = pq.top();                // smallest edge on pq
            pq.pop();
            int v = e->either(), w = e->other(v);      // two endpoints
            if (marked[v] && marked[w]) continue;      // lazy, both v and w already scanned
            mst.push_back(e);                          // add e to MST
            weight += e->getWeight();
            if (!marked[v]) scan(G, v);                // v becomes part of tree
            if (!marked[w]) scan(G, w);                // w becomes part of tree
        }
    }

public:
    /**
     * Compute a minimum spanning tree (or forest) of an edge-weighted graph.
     * @param G the edge-weighted graph
     */
    PrimMST(WeightedGraph *G) {
        marked = new bool[G->getV()];
        for (int v = 0; v < G->getV(); v++) {     // run Prim from all vertices to
            if (!marked[v]) prim(G, v);           // get a minimum spanning forest
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
    double getWeight() {
        return weight;
    }
};

#endif /* ALGORITHMS_GRAPH_MST_PRIMMST_H_ */
