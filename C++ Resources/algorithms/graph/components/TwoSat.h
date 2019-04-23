#pragma once
#include <bits/stdc++.h>
#include "TarjanScc.h"
using namespace std;

// Solves the two satisfiability problem:
// Given a conjuntive normal form (x0 | x1) & (x2 | !x1) & ...
// determine whether a consistent assignment exists
// Time Complexity: O(N + M) for N variables, M equations
// Memory Complexity: O(N + M) for N varaibles, M equations
template <const int MAXN> struct TwoSat {
    TarjanSCC<MAXN * 2> scc; bool vis[MAXN * 2], x[MAXN * 2]; int post[MAXN], cur;
    // adds the disjunction xi | xj, with possible negations
    void addEquation(bool affI, int i, bool affJ, int j) { 
        i = i * 2 + affI; j = j * 2 + affJ;
        scc.addEdge(i ^ 1, j); scc.addEdge(j ^ 1, i);
    }
    void dfs(int v) {
        vis[v] = true;
        for (int w : scc.DAG[v]) if (!vis[w]) dfs(w);
        post[v] = cur++;
    }
    bool solve(int N) {
        scc.run(N * 2);
        for (int i = 0; i < N; i++) if (scc.id[i * 2] == scc.id[i * 2 + 1]) return false;
        scc.genDAG(N * 2);
        fill(vis, vis + N * 2, false);
        for (int i = 0; i < int(scc.components.size()); i++) if (!vis[i]) dfs(i);
        for (int i = 0; i < N; i++) x[i] = post[scc.id[i * 2]] > post[scc.id[i * 2 + 1]];
        return true;
    }
    void clear(int N = MAXN) { scc.clear(); }
};
