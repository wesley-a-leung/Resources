#ifndef ALGORITHMS_GRAPH_NETWORKFLOW_HOPCROFTKARPMAXMATCH_H_
#define ALGORITHMS_GRAPH_NETWORKFLOW_HOPCROFTKARPMAXMATCH_H_

#include <bits/stdc++.h>
#include "algorithms/graph/matching/BipartiteX.h"
#include "datastructures/graph/Graph.h"

using namespace std;

/**
 *  The {@code HopcroftKarp} class represents a data type for computing a
 *  <em>maximum (cardinality) matching</em> and a
 *  <em>minimum (cardinality) vertex cover</em> in a bipartite graph.
 *  A <em>bipartite graph</em> in a graph whose vertices can be partitioned
 *  into two disjoint sets such that every edge has one endpoint in either set.
 *  A <em>matching</em> in a graph is a subset of its edges with no common
 *  vertices. A <em>maximum matching</em> is a matching with the maximum number
 *  of edges.
 *  A <em>perfect matching</em> is a matching which matches all vertices in the graph.
 *  A <em>vertex cover</em> in a graph is a subset of its vertices such that
 *  every edge is incident to at least one vertex. A <em>minimum vertex cover</em>
 *  is a vertex cover with the minimum number of vertices.
 *  By Konig's theorem, in any biparite
 *  graph, the maximum number of edges in matching equals the minimum number
 *  of vertices in a vertex cover.
 *  The maximum matching problem in <em>nonbipartite</em> graphs is
 *  also important, but all known algorithms for this more general problem
 *  are substantially more complicated.
 *  <p>
 *  This implementation uses the <em>Hopcroft-Karp algorithm</em>.
 *  The order of growth of the running time in the worst case is
 *  (<em>E</em> + <em>V</em>) sqrt(<em>V</em>),
 *  where <em>E</em> is the number of edges and <em>V</em> is the number
 *  of vertices in the graph. It uses extra space (not including the graph)
 *  proportional to <em>V</em>.
 *  <p>
 *  See also {@link BipartiteMatching}, which solves the problem in
 *  O(<em>E V</em>) time using the <em>alternating path algorithm</em>
 *  and <a href = "http://algs4.cs.princeton.edu/65reductions/BipartiteMatchingToMaxflow.java.html">BipartiteMatchingToMaxflow</a>,
 *  which solves the problem in O(<em>E V</em>) time via a reduction
 *  to the maxflow problem.
 *  <p>
 *  For additional documentation, see
 *  <a href="http://algs4.cs.princeton.edu/65reductions">Section 6.5</a>
 *  <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne.
 *
 *  @author Robert Sedgewick
 *  @author Kevin Wayne
 */
class HopcroftKarpMaxMatch {
private:
    int UNMATCHED = -1;

    int V;                 // number of vertices in the graph
    BipartiteX *bipartition;      // the bipartition
    int cardinality;             // cardinality of current matching
    int *mate;                  // mate[v] =  w if v-w is an edge in current matching
                                         //         = -1 if v is not in current matching
    bool *inMinVertexCover;  // inMinVertexCover[v] = true iff v is in min vertex cover
    bool *marked;            // marked[v] = true iff v is reachable via alternating path
    int *distTo;                // distTo[v] = number of edges on shortest path to v

    // is the edge v-w a forward edge not in the matching or a reverse edge in the matching?
    bool isResidualGraphEdge(int v, int w) {
        if ((mate[v] != w) &&  bipartition->getColor(v)) return true;
        if ((mate[v] == w) && !bipartition->getColor(v)) return true;
        return false;
    }

    // is the edge v-w in the level graph?
    bool isLevelGraphEdge(int v, int w) {
        return (distTo[w] == distTo[v] + 1) && isResidualGraphEdge(v, w);
    }

    /*
     * is there an augmenting path?
     *   - if so, upon termination adj[] contains the level graph;
     *   - if not, upon termination marked[] specifies those vertices reachable via an alternating
     *     path from one side of the bipartition
     *
     * an alternating path is a path whose edges belong alternately to the matching and not
     * to the matching
     *
     * an augmenting path is an alternating path that starts and ends at unmatched vertices
     */
    bool hasAugmentingPath(Graph *G) {
        // shortest path distances
        for (int v = 0; v < V; v++) {
            marked[v] = false;
            distTo[v] = INT_MAX;
        }

        // breadth-first search (starting from all unmatched vertices on one side of bipartition)
        queue<int> q;
        for (int v = 0; v < V; v++) {
            if (bipartition->getColor(v) && !isMatched(v)) {
                q.push(v);
                marked[v] = true;
                distTo[v] = 0;
            }
        }

        // run BFS until an augmenting path is found
        // (and keep going until all vertices at that distance are explored)
        bool hasAugmentingPath = false;
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (int w : G->adj(v)) {

                // forward edge not in matching or backwards edge in matching
                if (isResidualGraphEdge(v, w)) {
                    if (!marked[w]) {
                        distTo[w] = distTo[v] + 1;
                        marked[w] = true;
                        if (!isMatched(w))
                            hasAugmentingPath = true;

                        // stop enqueuing vertices once an alternating path has been discovered
                        // (no vertex on same side will be marked if its shortest path distance longer)
                        if (!hasAugmentingPath) q.push(w);
                    }
                }
            }
        }

        return hasAugmentingPath;
    }

public:
    /**
     * Determines a maximum matching (and a minimum vertex cover)
     * in a bipartite graph.
     *
     * @param  G the bipartite graph
     * @throws invalid_argument if {@code G} is not bipartite
     */
    HopcroftKarpMaxMatch(Graph *G) {
        bipartition = new BipartiteX(G);
        if (!bipartition->isBipartite()) {
            throw invalid_argument("graph is not bipartite");
        }

        // initialize empty matching
        V = G->getV();
        cardinality = 0;
        mate = new int[V];
        marked = new bool[V];
        distTo = new int[V];
        for (int v = 0; v < V; v++)
            mate[v] = UNMATCHED;

        // the call to hasAugmentingPath() provides enough info to reconstruct level graph
        while (hasAugmentingPath(G)) {

            // to be able to iterate over each adjacency list, keeping track of which
            // vertex in each adjacency list needs to be explored next
            int iter[V];
            vector<int> adj[V];
            for (int i = 0; i < V; i++) {
                adj[i] = G->adj(i);
                iter[i] = 0;
            }

            // for each unmatched vertex s on one side of bipartition
            for (int s = 0; s < V; s++) {
                if (isMatched(s) || !bipartition->getColor(s)) continue;   // or use distTo[s] == 0

                // find augmenting path from s using nonrecursive DFS
                stack<int> path;
                path.push(s);
                while (!path.empty()) {
                    int v = path.top();

                    // retreat, no more edges in level graph leaving v
                    if (iter[v] >= adj[v].size())
                        path.pop();

                    // advance
                    else {
                        // process edge v-w only if it is an edge in level graph
                        int w = adj[v][iter[v]++];
                        if (!isLevelGraphEdge(v, w)) continue;

                        // add w to augmenting path
                        path.push(w);

                        // augmenting path found: update the matching
                        if (!isMatched(w)) {
                            // StdOut.println("augmenting path: " + toString(path));

                            while (!path.empty()) {
                                int x = path.top();
                                path.pop();
                                int y = path.top();
                                path.pop();
                                mate[x] = y;
                                mate[y] = x;
                            }
                            cardinality++;
                        }
                    }
                }
            }
        }

        // also find a min vertex cover
        inMinVertexCover = new bool[V];
        for (int v = 0; v < V; v++) {
            inMinVertexCover[v] = false;
            if (bipartition->getColor(v) && !marked[v]) inMinVertexCover[v] = true;
            if (!bipartition->getColor(v) && marked[v]) inMinVertexCover[v] = true;
        }
    }

    /**
     * Returns the vertex to which the specified vertex is matched in
     * the maximum matching computed by the algorithm.
     *
     * @param  v the vertex
     * @return the vertex to which vertex {@code v} is matched in the
     *         maximum matching; {@code -1} if the vertex is not matched
     * @throws invalid_argument unless {@code 0 <= v < V}
     *
     */
    int getMate(int v) {
        return mate[v];
    }

    /**
     * Returns true if the specified vertex is matched in the maximum matching
     * computed by the algorithm.
     *
     * @param  v the vertex
     * @return {@code true} if vertex {@code v} is matched in maximum matching;
     *         {@code false} otherwise
     * @throws invalid_argument unless {@code 0 <= v < V}
     *
     */
    bool isMatched(int v) {
        return mate[v] != UNMATCHED;
    }

    /**
     * Returns the number of edges in any maximum matching.
     *
     * @return the number of edges in any maximum matching
     */
    int getCardinality() {
        return cardinality;
    }

    /**
     * Returns true if the graph contains a perfect matching.
     * That is, the number of edges in a maximum matching is equal to one half
     * of the number of vertices in the graph (so that every vertex is matched).
     *
     * @return {@code true} if the graph contains a perfect matching;
     *         {@code false} otherwise
     */
    bool isPerfect() {
        return cardinality * 2 == V;
    }

    /**
     * Returns true if the specified vertex is in the minimum vertex cover
     * computed by the algorithm.
     *
     * @param  v the vertex
     * @return {@code true} if vertex {@code v} is in the minimum vertex cover;
     *         {@code false} otherwise
     * @throws invalid_argument unless {@code 0 <= v < V}
     */
    bool getInMinVertexCover(int v) {
        return inMinVertexCover[v];
    }
};

#endif /* ALGORITHMS_GRAPH_NETWORKFLOW_HOPCROFTKARPMAXMATCH_H_ */
