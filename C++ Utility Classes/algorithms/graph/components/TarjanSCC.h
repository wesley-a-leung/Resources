#ifndef ALGORITHMS_GRAPH_COMPONENTS_TARJANSCC_H_
#define ALGORITHMS_GRAPH_COMPONENTS_TARJANSCC_H_

#include <bits/stdc++.h>
#include "datastructures/graph/Digraph.h"

using namespace std;

/**
 *  The {@code TarjanSCC} class represents a data type for
 *  determining the strong components in a digraph.
 *  The <em>id</em> operation determines in which strong component
 *  a given vertex lies; the <em>areStronglyConnected</em> operation
 *  determines whether two vertices are in the same strong component;
 *  and the <em>count</em> operation determines the number of strong
 *  components.

 *  The <em>component identifier</em> of a component is one of the
 *  vertices in the strong component: two vertices have the same component
 *  identifier if and only if they are in the same strong component.

 *  <p>
 *  This implementation uses Tarjan's algorithm.
 *  The constructor takes time proportional to <em>V</em> + <em>E</em>
 *  (in the worst case),
 *  where <em>V</em> is the number of vertices and <em>E</em> is the number of edges.
 *  Afterwards, the <em>id</em>, <em>count</em>, and <em>areStronglyConnected</em>
 *  operations take constant time.
 *  For alternate implementations of the same API, see
 *  {@link KosarajuSharirSCC} and {@link GabowSCC}.
 *  <p>
 *  For additional documentation,
 *  see <a href="http://algs4.cs.princeton.edu/42digraph">Section 4.2</a> of
 *  <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne.
 *
 *  @author Robert Sedgewick
 *  @author Kevin Wayne
 */
class TarjanSCC {
private:
    bool *marked;                    // marked[v] = has v been visited?
    int *id;                         // id[v] = id of strong component containing v
    vector<vector<int>> component;   // component[i] = vertices in component i
    int *low;                        // low[v] = low number of v
    int pre;                         // preorder number counter
    int count;                       // number of strongly-connected components
    stack<int> s;

    void dfs(Digraph *G, int v) {
        marked[v] = true;
        low[v] = pre++;
        int min = low[v];
        s.push(v);
        for (int w : G->adj(v)) {
            if (!marked[w]) dfs(G, w);
            if (low[w] < min) min = low[w];
        }
        if (min < low[v]) {
            low[v] = min;
            return;
        }
        int w;
        component.push_back(vector<int>());
        do {
            w = s.top();
            s.pop();
            id[w] = count;
            component[count].push_back(w);
            low[w] = G->getV();
        } while (w != v);
        count++;
    }

public:
    /**
     * Computes the strong components of the digraph {@code G}.
     * @param G the digraph
     */
    TarjanSCC(Digraph *G) {
        marked = new bool[G->getV()];
        id = new int[G->getV()];
        low = new int[G->getV()];
        for (int v = 0; v < G->getV(); v++) {
            marked[v] = false;
        }
        pre = 0;
        count = 0;
        for (int v = 0; v < G->getV(); v++) {
            if (!marked[v]) dfs(G, v);
        }
    }

    /**
     * Returns the number of strong components.
     * @return the number of strong components
     */
    int getCount() {
        return count;
    }

    /**
     * Are vertices {@code v} and {@code w} in the same strong component?
     * @param  v one vertex
     * @param  w the other vertex
     * @return {@code true} if vertices {@code v} and {@code w} are in the same
     *         strong component, and {@code false} otherwise
     */
    bool stronglyConnected(int v, int w) {
        return id[v] == id[w];
    }

    /**
     * Returns the component id of the strong component containing vertex {@code v}.
     * @param  v the vertex
     * @return the component id of the strong component containing vertex {@code v}
     */
    int getId(int v) {
        return id[v];
    }

    /**
     * Returns the vertices in the strong component with id {@code id}.
     *
     * @param id the id number
     * @return the vertices in the strong component with id {@code id}
     */
    vector<int> &getComponentOfId(int id) {
        return component[id];
    }

    /**
     * Returns the vertices in the strong component containing vertex {@code v}.
     *
     * @param v the vertex
     * @return the vertices in the strong component containing vertex {@code v}
     */
    vector<int> &getComponent(int v) {
        return component[id[v]];
    }

    /**
     * Returns the size of the strong component containing vertex {@code v}.
     * @param  v the vertex
     * @return the size of the strong component containing vertex {@code v}
     */
    int getSize(int v) {
        return (int) component[id[v]].size();
    }

    /**
     * Returns the size of the specified id.
     * @param  id the id number
     * @return the size of the specified id
     */
    int getIdSize(int id) {
        return (int) component[id].size();
    }
};

#endif /* ALGORITHMS_GRAPH_COMPONENTS_TARJANSCC_H_ */
