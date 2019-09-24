#pragma once
#include <bits/stdc++.h>
using namespace std;

// Mo's algorithm on a tree, used to count the number of distinct integers on a path between two nodes
// Time Complexity: O(V + Q log Q + Q * max(B, Q / B) * (update complexity))
// Memory Complexity: O(V log V + Q)
template <const int MAXV, const int MAXQ, const int BLOCKSZ, const int MAXLGV, const bool COMPRESS_VALUES> struct MoTree {
    struct Query {
        int l, r, lca, ind, block;
        bool operator < (const Query &q) const { return block == q.block ? r < q.r : block < q.block; }
    };
    int LG[MAXV * 2], head[MAXV], dep[MAXV], rmq[MAXLGV][MAXV * 2];
    int pre[MAXV], post[MAXV], vert[MAXV * 2], cnt[MAXV], ans[MAXQ], val[MAXV], temp[MAXV];
    int Q = 0, ind1, ind2, curAns; vector<int> adj[MAXV]; Query q[MAXQ]; bool vis[MAXV];
    void addEdge(int v, int w) { adj[v].push_back(w); adj[w].push_back(v); }
    void query(int v, int w) { q[Q++] = {v, w, 0, 0, 0}; }
    void dfs(int v, int prev, int d) {
        dep[v] = d; rmq[0][head[v] = ind1++] = v; vert[pre[v] = ind2++] = v;
        for (int w : adj[v]) if (w != prev) { dfs(w, v, d + 1); rmq[0][ind1++] = v; }
        vert[post[v] = ind2++] = v;
    }
    int minDep(int v, int w) { return dep[v] < dep[w] ? v : w; }
    int RMQ(int l, int r) { int i = LG[r - l + 1]; return minDep(rmq[i][l], rmq[i][r - (1 << i) + 1]); }
    int lca(int v, int w) { if (head[v] > head[w]) swap(v, w); return RMQ(head[v], head[w]); }
    void add(int x) { if (cnt[x]++ == 0) curAns++; }
    void rem(int x) { if (--cnt[x] == 0) curAns--; }
    void update(int v) {
        if (vis[v]) rem(val[v]);
        else add(val[v]);
        vis[v] = !vis[v];
    }
    void run(int V) {
        LG[0] = LG[1] = 0;
        for (int i = 2; i <= V * 2 - 1; i++) LG[i] = LG[i / 2] + 1;
        ind1 = ind2 = 0; int lg = LG[V * 2 - 1] + 1; fill(cnt, cnt + V, 0); fill(vis, vis + V, false); dfs(0, -1, 0);
        for (int i = 0; i < lg - 1; i++) for (int j = 0; j < ind1; j++) rmq[i + 1][j] = minDep(rmq[i][j], rmq[i][min(j + (1 << i), ind1 - 1)]);
        if (COMPRESS_VALUES) {
            copy(val, val + V, temp); sort(temp, temp + V); int k = unique(temp, temp + V) - temp;
            for (int v = 0; v < V; v++) val[v] = lower_bound(temp, temp + k, val[v]) - temp;
        }
        for (int i = 0; i < Q; i++) {
            int v = q[i].l, w = q[i].r; q[i].lca = lca(v, w);
            if (pre[v] > pre[w]) swap(v, w);
            if (q[i].lca == v) { q[i].l = pre[v]; q[i].r = pre[w]; }
            else { q[i].l = post[v]; q[i].r = pre[w]; }
            q[i].ind = i; q[i].block = q[i].l / BLOCKSZ;
        }
        sort(q, q + Q); int l = q[0].l, r = l - 1; curAns = 0;
        for (int i = 0; i < Q; i++) {
            while (l < q[i].l) update(vert[l++]);
            while (l > q[i].l) update(vert[--l]);
            while (r < q[i].r) update(vert[++r]);
            while (r > q[i].r) update(vert[r--]);
            if (q[i].lca != vert[l] && q[i].lca != vert[r]) update(q[i].lca);
            ans[q[i].ind] = curAns;
            if (q[i].lca != vert[l] && q[i].lca != vert[r]) update(q[i].lca);
        }
    }
    void clear(int V = MAXV) { Q = 0; for (int i = 0; i < V; i++) adj[i].clear(); }
};
