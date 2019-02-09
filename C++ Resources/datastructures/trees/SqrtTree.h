#pragma once
#include <bits/stdc++.h>
using namespace std;

// Supports point updates and range queries
// Time Complexity:
//   init: O(N log log N)
//   update: O(sqrt N)
//   query: O(1)
// Memory Complexity: O(N log log N)
template <const int MAXN, const int MAXLGN, const int MAXLGLGN, const bool ONE_INDEXED> struct SqrtTree {
    using Data = int; using Lazy = int; const Data def = 0;
    int N, LG, LGLG, INDSZ, clz[MAXN * 2], layers[MAXLGLGN], onLayer[MAXLGN];
    Data A[MAXN * 2], pre[MAXLGLGN][MAXN * 2], suf[MAXLGLGN][MAXN * 2], bet[MAXLGLGN][MAXN * 2];
    Data merge(const Data &l, const Data &r); // to be implemented
    Data applyLazy(const Data &l, const Lazy &r); // to be implemented
    int log2(int n) { int res = 0; while ((1 << res) < n) res++; return res; }
    void buildBlock(int layer, int l, int r) {
        pre[layer][l] = A[l]; suf[layer][r - 1] = A[r - 1];
        for (int i = l + 1; i < r; i++) pre[layer][i] = merge(pre[layer][i - 1], A[i]);
        for (int i = r - 2; i >= l; i--) suf[layer][i] = merge(A[i], suf[layer][i + 1]);
    }
    void buildBet(int layer, int lb, int rb, int offset) {
        int bszlg = (layers[layer] + 1) >> 1, cntlg = layers[layer] >> 1, bsz = 1 << bszlg, cnt = (rb - lb + bsz - 1) >> bszlg; Data cur = def;
        for (int i = 0; i < cnt; i++) for (int j = i; j < cnt; j++) {
            cur = i == j ? suf[layer][lb + (j << bszlg)] : merge(cur, suf[layer][lb + (j << bszlg)]);
            bet[layer - 1][offset + lb + (i << cntlg) + j] = cur;
        }
    }
    void buildBetZero() {
        int bszlg = (LG + 1) >> 1;
        for (int i = 0; i < INDSZ; i++) A[N + i] = suf[0][i << bszlg];
        build(1, N, N + INDSZ, (1 << LG) - N);
    }
    void updateBetZero(int ind) {
        int bszlg = (LG + 1) >> 1; A[N + ind] = suf[0][ind << bszlg]; update(1, N, N + INDSZ, (1 << LG) - N, N + ind); 
    }
    void build(int layer, int lb, int rb, int offset) {
        if (layer >= LGLG) return;
        int bsz = 1 << ((layers[layer] + 1) >> 1);
        for (int l = lb; l < rb; l += bsz) { buildBlock(layer, l, min(l + bsz, rb)); build(layer + 1, l, min(l + bsz, rb), offset); }
        if (layer == 0) buildBetZero();
        else buildBet(layer, lb, rb, offset);
    }
    void update(int layer, int lb, int rb, int offset, int ind) {
        if (layer >= LGLG) return;
        int bszlg = (layers[layer] + 1) >> 1, bsz = 1 << bszlg, bind = (ind - lb) >> bszlg, l = lb + (bind << bszlg), r = min(l + bsz, rb);
        buildBlock(layer, l, r);
        if (layer == 0) updateBetZero(bind);
        else buildBet(layer, lb, rb, offset);
        update(layer + 1, l, r, offset, ind);
    }
    Data query(int l, int r, int offset, int base) {
        if (l == r) return A[l];
        if (l + 1 == r) return merge(A[l], A[r]);
        int layer = onLayer[clz[(l - base) ^ (r - base)]], bszlg = (layers[layer] + 1) >> 1, cntlg = layers[layer] >> 1;
        base += ((l - base) >> layers[layer]) << layers[layer]; int lb = ((l - base) >> bszlg) + 1, rb = ((r - base) >> bszlg) - 1;
        Data ret = suf[layer][l];
        if (lb <= rb) ret = merge(ret, layer == 0 ? query(N + lb, N + rb, (1 << LG) - N, N) : bet[layer - 1][offset + base + (lb << cntlg) + rb]);
        return merge(ret, pre[layer][r]);
    }
    template <class It> void init(It st, It en) {
        copy(st, en, A); N = en - st; LG = log2(N); LGLG = 0; clz[0] = 0; assert(LG < MAXLGN); assert((1 << LG) <= 2 * MAXN);
        for (int i = 1; i < (1 << LG); i++) clz[i] = clz[i >> 1] + 1;
        for (int i = LG; i > 1; i = (i + 1) >> 1) { assert(LGLG < MAXLGLGN); layers[onLayer[i] = LGLG++] = i; }
        for (int i = LG - 1; i >= 0; i--) onLayer[i] = max(onLayer[i], onLayer[i + 1]);
        int bszlg = (LG + 1) >> 1, bsz = 1 << bszlg; INDSZ = (N + bsz - 1) >> bszlg;
        assert(N + INDSZ <= MAXN * 2 && (1 << LG) + bsz <= MAXN * 2); fill(A + N, A + N + INDSZ, def); build(0, 0, N, 0);
    }
    void init(int size) { fill(A, A + size, def); init(A, A + size); }
    void update(int ind, const Lazy &v) { ind -= ONE_INDEXED; A[ind] = applyLazy(A[ind], v); update(0, 0, N, 0, ind); }
    Data query(int l, int r) { return query(l - ONE_INDEXED, r - ONE_INDEXED, 0, 0); }
};
