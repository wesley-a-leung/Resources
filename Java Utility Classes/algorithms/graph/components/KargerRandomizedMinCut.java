package algorithms.graph.components;

import java.util.ArrayList;

import datastructures.UF;
import datastructures.graph.Graph;

/**
 * The {@code KargerRandomizedMinCut} class represents a data type for computing
 * a <em>global minimum cut</em> in an graph. A <em>cut</em> is a partition of
 * the set of vertices of a graph into two nonempty subsets. An edge that has
 * one endpoint in each subset of a cut is a <em>crossing edge</em>.
 * 
 * The randomized algorithm has a 1 / (V choose 2) probability of success where
 * V is the number of vertices. The algorithm is repeated multiple times to
 * increase the probability of success.
 * 
 * @author Wesley Leung
 *
 */
public class KargerRandomizedMinCut {
    private ArrayList<Edge> allEdges;
    private ArrayList<Edge> cutEdges;
    private static int globalMinEdges;
    
    /**
     * Computes the minimum cut from this graph after a specified number of trials.
     * 
     * @param G the graph
     * @param trials number of trials to perform
     */
    public KargerRandomizedMinCut(Graph G, int trials) {
        allEdges = new ArrayList<Edge>(G.E());
        cutEdges = new ArrayList<Edge>();
        for (int v = 0; v < G.V(); v++) {
            int selfLoops = 0;
            for (int w: G.adj(v)) {
                if (v < w) {
                    allEdges.add(new Edge(v, w));
                }
                // only add one copy of each self loop (self loops will be consecutive)
                else if (v == w) {
                    if (selfLoops % 2 == 0) {
                        allEdges.add(new Edge(v, w));
                        selfLoops++;
                    }
                }
            }
        }
        globalMinEdges = Integer.MAX_VALUE;
        for (int i = 0; i < trials; i++) {
            int localMinEdges = 0;
            ArrayList<Edge> localCutEdges = new ArrayList<Edge>();
            UF uf = new UF(G.V());
            contract(G, uf);
            for (Edge e: allEdges) {
                if (!uf.connected(e.v, e.w)) {
                    localMinEdges++;
                    localCutEdges.add(e);
                }
            }
            if (localMinEdges < globalMinEdges) {
                globalMinEdges = localMinEdges;
                cutEdges = (ArrayList<Edge>) localCutEdges.clone();
            }
        }
    }
    
    private void contract(Graph G, UF uf) {
        int curSize = allEdges.size();
        int nodesLeft = G.V();
        while (nodesLeft > 2) {
            int ran = (int) (Math.random() * curSize);
            Edge e = allEdges.get(ran);
            if (!uf.connected(e.v, e.w)) {
                uf.union(e.v, e.w);
                nodesLeft--;
            }
            Edge temp = allEdges.get(--curSize);
            allEdges.set(curSize, e);
            allEdges.set(ran, temp);
        }
    }
    
    /**
     * Returns the number of cut edges.
     * 
     * @return the number of cut edges
     */
    public int minCut() {
        return globalMinEdges;
    }
    
    /**
     * Returns all the cut edges from the graph.
     * 
     * @return all the cut edges from the graph, as an iterable
     */
    public Iterable<Edge> cutEdges() {
        return cutEdges;
    }
    
    private class Edge {
        private int v;
        private int w;
        
        public Edge(int v, int w) {
            this.v = v;
            this.w = w;
        }
        
        /**
         * Returns either endpoint of this edge.
         *
         * @return either endpoint of this edge
         */
        public int either() {
            return v;
        }

        /**
         * Returns the endpoint of this edge that is different from the given vertex.
         *
         * @param  vertex one endpoint of this edge
         * @return the other endpoint of this edge
         * @throws IllegalArgumentException if the vertex is not one of the
         *         endpoints of this edge
         */
        public int other(int vertex) {
            if      (vertex == v) return w;
            else if (vertex == w) return v;
            else throw new IllegalArgumentException("Illegal endpoint");
        }
        
        @Override
        public int hashCode() {
            return 31 * Math.min(v, w) + Math.max(v, w);
        }

        @Override
        public boolean equals(Object o) {
            if (o == this) return true;
            if (!(o instanceof Edge)) return false;
            Edge e = (Edge) o;
            return (v == e.v && w == e.w) || (v == e.w && w == e.v);
        }
    }
}
