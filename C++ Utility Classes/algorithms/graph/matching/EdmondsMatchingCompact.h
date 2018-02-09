#ifndef ALGORITHMS_GRAPH_MATCHING_EDMONDSMATCHINGCOMPACT_H_
#define ALGORITHMS_GRAPH_MATCHING_EDMONDSMATCHINGCOMPACT_H_

#include <bits/stdc++.h>
using namespace std;

/**
 * Edmonds Blossom Algorithm.
 *
 * @author Jeffrey Xiao
 */
class EdmondsMatching {
private:
    int N;
    vector<vector<int>> adj;
    vector<bool> mark, used;
    vector<int> match, par, id;

    void markPath(vector<bool> &blossom, int i, int b, int j) {
        for (; id[i] != b; i = par[match[i]]) {
            blossom[id[i]] = blossom[id[match[i]]] = true;
            par[i] = j;
            j = match[i];
        }
    }

    int lca(int i, int j) {
        vector<bool> v(N);
        while (true) {
            i = id[i];
            used[i] = true;
            if (match[i] == -1) break;
            i = par[match[i]];
        }
        while (true) {
            j = id[j];
            if (v[j]) return j;
            j = par[match[j]];
        }
    }

    int getAugmentingPath(int src) {
        fill(par.begin(), par.end(), -1);
        fill(used.begin(), used.end(), 0);
        for (int i = 0; i < N; i++) id[i] = i;
        used[src] = true;
        queue<int> q;
        q.push(src);

        while (!q.empty()) {
            int curr = q.front();
            q.pop();
            for (int next : adj[curr]) {
                if (id[curr] == id[next] || match[curr] == next) continue;
                if (next == src || (match[next] != -1 && par[match[next]] != -1)) {
                    int newBase = lca(curr, next);
                    vector<bool> blossom(N);
                    markPath(blossom, curr, newBase, next);
                    markPath(blossom, next, newBase, curr);
                    for (int i = 0; i < N; i++) {
                        if (blossom[id[i]]) {
                            id[i] = newBase;
                            if (!used[i]) {
                                used[i] = true;
                                q.push(i);
                            }
                        }
                    }
                } else if (par[next] == -1) {
                    par[next] = curr;
                    if (match[next] == -1) return next;
                    next = match[next];
                    used[next] = true;
                    q.push(next);
                }
            }
        }
        return -1; // should never be here
    }

public:
    EdmondsMatching(int N) : N(N), adj(N), mark(N), used(N), match(N), par(N), id(N) {}

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    int getMaxMatching() {
        fill(match.begin(), match.end(), -1);
        fill(par.begin(), par.end(), 0);
        fill(id.begin(), id.end(), 0);
        fill(used.begin(), used.end(), 0);

        for (int i = 0; i < N; i++) {
            if (match[i] == -1) {
                int v = getAugmentingPath(i);
                while (v != -1) {
                    int pv = par[v];
                    int ppv = match[pv];
                    match[v] = pv;
                    match[pv] = v;
                    v = ppv;
                }
            }
        }

        int res = 0;
        for (int i = 0; i < N; i++) if (match[i] != -1) res++;
        return res / 2;
    }
};

#endif /* ALGORITHMS_GRAPH_MATCHING_EDMONDSMATCHINGCOMPACT_H_ */
