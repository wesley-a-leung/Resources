#ifndef ALGORITHMS_GRAPH_LCA_LCA_FARAHCOLTONBENDER_H_
#define ALGORITHMS_GRAPH_LCA_LCA_FARAHCOLTONBENDER_H_

#include <bits/stdc++.h>
using namespace std;

/**
 * The {@code LCA} class represents a data structure for finding the
 * lowest common ancestor of 2 nodes in a tree using the Farah-Colton and Bender algorithm.
 * The constructor takes time proportional to V + E and afterwards,
 * the {@code lca()} method takes time constant time to find the lowest common ancestor.
 */
class LCA {
private:
    int V, M;
    vector<vector<int>> adj;
    vector<int> head, euler, depth, log_2;
    vector<vector<int>> st;
    vector<vector<vector<int>>> blocks;
    vector<int> blockMask;
    int blockSize, blockCnt;

    void dfs(int v, int prev) {
        head[v] = (int) euler.size();
        euler.push_back(v);
        for (int w : adj[v]) {
            if (w == prev) continue;
            depth[w] = depth[v] + 1;
            dfs(w, v);
            euler.push_back(v);
        }
    }

    inline int minDepth(int i, int j) {
        return depth[euler[i]] < depth[euler[j]] ? i : j;
    }

    inline int lcaBlock(int b, int l, int r) {
        return blocks[blockMask[b]][l][r] + b * blockSize;
    }

public:
    /**
     * Initializes the structure.
     *
     * @param V the number of vertices
     */
    LCA(int V) : V(V), adj(V) {
        head.assign(V, -1);
        depth.assign(V, 0);
        M = 2 * V;
        euler.reserve(M);
        log_2.reserve(M + 1);
        log_2.push_back(-1);
        for (int i = 1; i <= M; i++) {
            log_2.push_back(log_2[i / 2] + 1);
        }
        blockSize = max(1, log_2[M] / 2);
        blockCnt = (M + blockSize - 1) / blockSize;
        st.assign(blockCnt, vector<int>(log_2[blockCnt] + 1));
        blockMask.assign(blockCnt, 0);
        blocks.resize(1 << (blockSize - 1));
    }

    /**
     * Adds a bidirectional edge from v to w.
     *
     * @param v one vertex of the edge
     * @param w the other vertex of the edge
     */
    void addEdge(int v, int w) {
        adj[v].push_back(w);
        adj[w].push_back(v);
    }

    /**
     * Begins the LCA process.
     *
     * @param root the root of the tree
     */
    void compute(int root = 0) {
        depth[root] = 0;
        dfs(root, -1);
        for (int i = 0, j = 0, b = 0; i < M; i++, j++) {
            if (j == blockSize) {
                j = 0;
                b++;
            }
            if (j == 0 || minDepth(i, st[b][0]) == i) {
                st[b][0] = i;
            }
        }
        for (int j = 1; j <= log_2[blockCnt]; j++) {
            for (int i = 0; i < blockCnt; i++) {
                int k = i + (1 << (j - 1));
                if (k >= blockCnt) st[i][j] = st[i][j - 1];
                else st[i][j] = minDepth(st[i][j - 1], st[k][j - 1]);
            }
        }
        for (int i = 0, j = 0, b = 0; i < M; i++, j++) {
            if (j == blockSize) {
                j = 0;
                b++;
            }
            if (j > 0 && (i >= M || minDepth(i - 1, i) == i - 1)) {
                blockMask[b] += 1 << (j - 1);
            }
        }
        for (int b = 0; b < blockCnt; b++) {
            int mask = blockMask[b];
            if (!blocks[mask].empty()) continue;
            blocks[mask].assign(blockSize, vector<int>(blockSize));
            for (int l = 0; l < blockSize; l++) {
                blocks[mask][l][l] = l;
                for (int r = l + 1; r < blockSize; r++) {
                    blocks[mask][l][r] = blocks[mask][l][r - 1];
                    if (b * blockSize + r < M) {
                        blocks[mask][l][r] = minDepth(b * blockSize + blocks[mask][l][r],
                                b * blockSize + r) - b * blockSize;
                    }
                }
            }
        }
    }

    /**
     * Returns the lowest common ancestor of vertex {@code v} and {@code w}.
     *
     * @param v the first vertex
     * @param w the first vertex
     * @return the lowest common ancestor of vertex {@code v} and {@code w}
     */
    int lca(int v, int w) {
        int l = head[v];
        int r = head[w];
        if (l > r) swap(l, r);
        int bl = l / blockSize;
        int br = r / blockSize;
        if (bl == br) return euler[lcaBlock(bl, l % blockSize, r % blockSize)];
        int ans1 = lcaBlock(bl, l % blockSize, blockSize - 1);
        int ans2 = lcaBlock(br, 0, r % blockSize);
        int ans = minDepth(ans1, ans2);
        if (bl + 1 < br) {
            int l = log_2[br - bl - 1];
            int ans3 = st[bl + 1][l];
            int ans4 = st[br - (1 << l)][l];
            ans = minDepth(ans, minDepth(ans3, ans4));
        }
        return euler[ans];
    }
};

#endif /* ALGORITHMS_GRAPH_LCA_LCA_FARAHCOLTONBENDER_H_ */
