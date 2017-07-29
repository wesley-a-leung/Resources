package algorithms.graph.search;

import datastructures.graph.Graph;

public class CentroidDecomposition {
    private final Graph G;
    private boolean[] exclude;
    
    public CentroidDecomposition(Graph G) {
        this.G = G;
        exclude = new boolean[G.V()];
    }
    
    private int getSize(int v, int prev) {
        int size = 1;
        for (int w : G.adj(v)) {
            if (w != prev && !exclude[w]) size += getSize(w, v);
        }
        return size;
    }
    
    /**
     * Returns the centroid of the current subtree containing v, then divides the tree.
     * 
     * @param v the vertex
     * @return the centroid of the current subtree containing v
     */
    public int getCentroid(int v) {
        int c = getCentroid(v, -1, getSize(v, -1));
        exclude[c] = true;
        return c;
    }
    
    private int getCentroid(int v, int prev, int treeSize) {
        int n = treeSize;
        int size = 1;
        boolean hasCentroid = true;
        for (int w : G.adj(v)) {
            if (w == prev || exclude[w]) continue;
            int ret = getCentroid(w, v, treeSize);
            if (ret >= 0) return ret;
            hasCentroid &= -ret <= n / 2;
            size += -ret;
        }
        hasCentroid &= n - size <= n / 2;
        return hasCentroid ? v : -size;
    }
}
