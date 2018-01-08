#ifndef ALGORITHMS_GRAPH_QUERIES_MOTREE_H_
#define ALGORITHMS_GRAPH_QUERIES_MOTREE_H_

#include <bits/stdc++.h>
#include <datastructures/graph/Graph.h>

using namespace std;

/**
 * Mo's Algorithm for a tree. Computes the number of distinct integers on the path between
 * two nodes.
 */
class MoTree {
private:
    bool *vis;
    int *a, *depth, *parent, *chain, *size, *head, *pre, *post, *vertex, *cnt, *ans;
    int V, Q, chainNum, cur, res;

    struct Query {
        int l, r, lca, ind, block;

        bool operator < (const Query &q) const {
            if (block != q.block) return block < q.block;
            return r < q.r;
        }
    } *q;

    void dfs(Graph *G, int v, int d, int prev) {
        vertex[cur] = v;
        pre[v] = cur++;
        depth[v] = d;
        parent[v] = prev;
        size[v] = 1;
        for (int w: G->adj(v)) {
            if (w != prev) {
                dfs(G, w, d + 1, v);
                size[v] += size[w];
            }
        }
        vertex[cur] = v;
        post[v] = cur++;
    }

    void hld(Graph *G, int v, int prev) {
        if (head[chainNum] == -1) head[chainNum] = v;
        chain[v] = chainNum;
        int maxIndex = -1;
        for (int w: G->adj(v)) {
            if (w != prev && (maxIndex == -1 || size[maxIndex] < size[w])) maxIndex = w;
        }
        if (maxIndex != -1) hld(G, maxIndex, v);
        for (int w: G->adj(v)) {
            if (w != prev && w != maxIndex) {
                chainNum++;
                hld(G, w, v);
            }
        }
    }

    int lca(int v, int w) {
        while (chain[v] != chain[w]) {
            if (depth[head[chain[v]]] < depth[head[chain[w]]]) w = parent[head[chain[w]]];
            else v = parent[head[chain[v]]];
        }
        if (depth[v] < depth[w]) return v;
        return w;
    }

    void update(int v) {
        if (vis[v] && --cnt[a[v]] == 0) res--;
        else if (!vis[v] && cnt[a[v]]++ == 0) res++;
        vis[v] = !vis[v];
    }

    void mo() {
        int l = q[0].l;
        int r = l - 1;
        res = 0;
        for (int i = 0; i < Q; i++) {
            while (l < q[i].l) {
                update(vertex[l]);
                l++;
            }
            while (l > q[i].l) {
                l--;
                update(vertex[l]);
            }
            while (r < q[i].r) {
                r++;
                update(vertex[r]);
            }
            while (r > q[i].r) {
                update(vertex[r]);
                r--;
            }
            if (q[i].lca != vertex[l] && q[i].lca != vertex[r]) update(q[i].lca);
            ans[q[i].ind] = res;
            if (q[i].lca != vertex[l] && q[i].lca != vertex[r]) update(q[i].lca);
        }
    }

public:
    /**
     * Computes the number of distinct integers on the path between
     * two nodes.
     * @param G the Graph
     * @param arr the array with the weights of each node
     * @param queries the array of pairs containing the queries
     * @param Q the number of queries
     */
    MoTree(Graph *G, int *arr, pair<int, int> *queries, int Q) {
        this->V = G->getV();
        this->Q = Q;
        a = new int[V];
        vis = new bool[V];
        depth = new int[V];
        parent = new int[V];
        chain = new int[V];
        size = new int[V];
        head = new int[V];
        pre = new int[V];
        post = new int[V];
        vertex = new int[V * 2];
        cnt = new int[V];
        q = new Query[Q];
        ans = new int[Q];
        chainNum = 0;
        cur = 0;
        for (int i = 0; i < V; i++) {
            head[i] = -1;
            vis[i] = false;
        }
        dfs(G, 0, 0, -1);
        hld(G, 0, -1);
        int sz = (int) sqrt(cur);
        int temp[V];
        for (int i = 0; i < V; i++) {
            temp[i] = arr[i];
            cnt[i] = 0;
        }
        sort(temp, temp + V);
        int k = unique(temp, temp + V) - temp;
        for (int i = 0; i < V; i++) {
            a[i] = lower_bound(temp, temp + k, arr[i]) - temp;
        }
        for (int i = 0; i < Q; i++) {
            int v = queries[i].first;
            int w = queries[i].second;
            q[i].lca = lca(v, w);
            if (pre[v] > pre[w]) swap(v, w);
            if (q[i].lca == v) {
                q[i].l = pre[v];
                q[i].r = pre[w];
            } else {
                q[i].l = post[v];
                q[i].r = pre[w];
            }
            q[i].ind = i;
            q[i].block = q[i].l / sz;
        }
        sort(q, q + Q);
        mo();
    }

    /**
     * Returns the answer to the ith query.
     *
     * @param i the index of the query
     * @return the answer the query
     */
    int query(int i) {
        return ans[i];
    }
};

#endif /* ALGORITHMS_GRAPH_QUERIES_MOTREE_H_ */
