#ifndef ALGORITHMS_GRAPH_MST_BORUVKAMST_H_
#define ALGORITHMS_GRAPH_MST_BORUVKAMST_H_

#include <bits/stdc++.h>
#include "datastructures/graph/WeightedGraph.h"
#include "datastructures/math/UF.h"

using namespace std;

/**
 *  The {@code BoruvkaMST} class represents a data type for computing a
 *  <em>minimum spanning tree</em> in an edge-weighted graph.
 *  The edge weights can be positive, zero, or negative and need not
 *  be distinct. If the graph is not connected, it computes a <em>minimum
 *  spanning forest</em>, which is the union of minimum spanning trees
 *  in each connected component. The {@code weight()} method returns the
 *  weight of a minimum spanning tree and the {@code edges()} method
 *  returns its edges.
 *  <p>
 *  This implementation uses <em>Boruvka's algorithm</em> and the union-find
 *  data type.
 *  The constructor takes time proportional to <em>E</em> log <em>V</em>
 *  and extra space (not including the graph) proportional to <em>V</em>,
 *  where <em>V</em> is the number of vertices and <em>E</em> is the number of edges.
 *  Afterwards, the {@code weight()} method takes constant time
 *  and the {@code edges()} method takes time proportional to <em>V</em>.
 *  <p>
 *  For additional documentation,
 *  see <a href="http://algs4.cs.princeton.edu/43mst">Section 4.3</a> of
 *  <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne.
 *  For alternate implementations, see {@link LazyPrimMST}, {@link PrimMST},
 *  and {@link KruskalMST}.
 *
 *  @author Robert Sedgewick
 *  @author Kevin Wayne
 */
typedef double unit;
class BoruvkaMST {
private:
    vector<WeightedEdge*> mst;    // edges in MST
    unit weight;                // weight of MST
    WeightedEdge **closest;

public:
    /**
     * Compute a minimum spanning tree (or forest) of an edge-weighted graph.
     * @param V the number of vertices
     * @param graphEdges the edges of the graph
     */
    BoruvkaMST(int V, vector<WeightedEdge*> &graphEdges) {
        weight = (unit) 0;
        closest = new WeightedEdge*[V];
        UF uf(V);
        // repeat at most log V times or until we have V-1 edges
        for (int t = 1; t < V && mst.size() < V - 1; t = t + t) {

            for (int i = 0; i < V; i++) {
                closest[i] = nullptr;
            }

            // foreach tree in forest, find closest edge
            // if edge weights are equal, ties are broken in favor of first edge in G.edges()
            for (WeightedEdge *e : graphEdges) {
                int v = e->either(), w = e->other(v);
                int i = uf.find(v), j = uf.find(w);
                if (i == j) continue;   // same tree
                if (closest[i] == nullptr || *e < *closest[i]) closest[i] = e;
                if (closest[j] == nullptr || *e < *closest[j]) closest[j] = e;
            }

            // add newly discovered edges to MST
            for (int i = 0; i < V; i++) {
                WeightedEdge *e = closest[i];
                if (e != nullptr) {
                    int v = e->either(), w = e->other(v);
                    // don't add the same edge twice
                    if (!uf.connected(v, w)) {
                        mst.push_back(e);
                        weight += e->getWeight();
                        uf.join(v, w);
                    }
                }
            }
        }
    }

    /**
     * Compute a minimum spanning tree (or forest) of an edge-weighted graph.
     * @param G the edge-weighted graph
     */
    BoruvkaMST(WeightedGraph *G) : BoruvkaMST(G->getV(), G->edges()) {}

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

#endif /* ALGORITHMS_GRAPH_MST_BORUVKAMST_H_ */
