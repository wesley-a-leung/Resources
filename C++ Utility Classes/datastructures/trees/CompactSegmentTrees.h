#ifndef DATASTRUCTURES_TREES_COMPACTSEGMENTTREES_H_
#define DATASTRUCTURES_TREES_COMPACTSEGMENTTREES_H_

#include <bits/stdc++.h>

using namespace std;

// single assignment and modifications, range query
struct SegmentTree_SAM_RQ {
private:
    int N;
    int *T;
    const int def = INT_MIN; // default value

    // merges 2 values together in the correct order
    // operation must be associative (but not necessarily commutative)
    int merge(int l, int r) {
        return max(l, r);
    }

public:
    SegmentTree_SAM_RQ(int size, int *A, bool oneIndexed) {
        N = size;
        T = new int[2 * N];
        for (int i = 0; i < N; i++) T[N + i] = A[i + oneIndexed];
        for (int i = N - 1; i > 0; i--) T[i] = merge(T[i << 1], T[i << 1 | 1]);
    }

    SegmentTree_SAM_RQ(int size) {
        N = size;
        T = new int[2 * N];
        for (int i = 1; i < 2 * N; i++) T[i] = def;
    }

    // assigns v to index i (one-indexed)
    void update(int i, int v) {
        for (T[i += (N - 1)] = v; i >>= 1;) T[i] = merge(T[i << 1], T[i << 1 | 1]);
    }

    // returns the maximum element on the interval [l, r] (one-indexed)
    int query(int l, int r) {
        int ql = def, qr = def;
        for (l += (N - 1), r += (N - 1); l <= r; l >>= 1, r >>= 1) {
            if (l & 1) ql = merge(ql, T[l++]);
            if (!(r & 1)) qr = merge(T[r--], qr);
        }
        return merge(ql, qr);
    }

    int size() {
        return N;
    }
};

// range modification, single query
// works when order of modifications does not affect result
struct SegmentTree_RM_SQ {
private:
    int N;
    int *T;
    const int def = 0; // default value
    bool final = false;

public:
    SegmentTree_RM_SQ(int size, int *A, bool oneIndexed) {
        N = size;
        T = new int[2 * N];
        for (int i = 0; i < N; i++) {
            T[N + i] = A[i + oneIndexed];
            T[i] = 0;
        }
    }

    SegmentTree_RM_SQ(int size) {
        N = size;
        T = new int[2 * N];
        for (int i = 0; i < N; i++) {
            T[N + i] = def;
            T[i] = 0;
        }
    }

    // adds v to the interval [l, r] (one-indexed)
    void update(int l, int r, int v) {
        for (l += (N - 1), r += (N - 1); l <= r; l >>= 1, r >>= 1) {
            if (l & 1) T[l++] += v;
            if (!(r & 1)) T[r--] += v;
        }
    }

    // returns the value at index i (one-indexed)
    int query(int i) {
        if (final) return T[N + i - 1];
        int q = 0;
        for (i += (N - 1); i > 0; i >>= 1) q += T[i];
        return q;
    }

    void pushAll() {
        for (int i = 1; i < N; i++) {
            T[i << 1] += T[i];
            T[i << 1 | 1] += T[i];
            T[i] = 0;
        }
        final = true;
    }

    int size() {
        return N;
    }
};

// range modifications, range query
// works when order of modifications does not affect result
struct SegmentTree_RM_RQ {
private:
    int N, H;
    int *T;
    int *L; // storing lazy values
    const int def = INT_MIN; // default value
    const int ldef = 0; // lazy default value

    // applies the change to index i and updates the lazy value if the index is not a leaf
    void apply(int i, int v) {
        T[i] += v;
        if (i < N) L[i] += v;
    }

     // updates parents of index i
    void pushup(int i) {
        while (i > 1) {
            i >>= 1;
            T[i] = merge(T[i << 1], T[i << 1 | 1]) + L[i];
        }
    }

    // propagates changes down from index i
    void propogate(int i) {
        for (int h = H; h > 0; h--) {
            int ii = i >> h;
            if (L[ii] != ldef) {
                apply(ii << 1, L[ii]);
                apply(ii << 1 | 1, L[ii]);
                L[ii] = ldef;
            }
        }
    }

    // merges 2 values together in the correct order
    // operation must be associative (but not necessarily commutative)
    int merge(int l, int r) {
        return max(l, r);
    }

public:
    SegmentTree_RM_RQ(int size, int *A, bool oneIndexed) {
        N = size;
        T = new int[2 * N];
        L = new int[N];
        H = 0;
        for (int i = 1; i <= N; H++) i <<= 1;
        for (int i = 0; i < N; i++) {
            T[N + i] = A[i + oneIndexed];
            L[i] = ldef;
        }
        for (int i = N - 1; i > 0; i--) T[i] = merge(T[i << 1], T[i << 1 | 1]);
    }

    SegmentTree_RM_RQ(int size) {
        N = size;
        T = new int[2 * N];
        L = new int[N];
        H = 0;
        for (int i = 1, H = 0; i <= N; H++) i <<= 1;
        for (int i = 1; i < 2 * N; i++) T[i] = def;
        for (int i = 1; i < N; i++) L[i] = ldef;
    }

    // adds v to the interval [l, r] (one-indexed)
    void update(int l, int r, int v) {
        l += (N - 1);
        r += (N - 1);
        int l0 = l, r0 = r;
        for (; l <= r; l >>= 1, r >>= 1) {
            if (l & 1) apply(l++, v);
            if (!(r & 1)) apply(r--, v);
        }
        pushup(l0);
        pushup(r0);
    }

    // returns the maximum element on the interval [l, r] (one-indexed)
    int query(int l, int r) {
        l += (N - 1);
        r += (N - 1);
        propogate(l);
        propogate(r);
        int ql = def, qr = def;
        for (; l <= r; l >>= 1, r >>= 1) {
            if (l & 1) ql = merge(ql, T[l++]);
            if (!(r & 1)) qr = merge(T[r--], qr);
        }
        return merge(ql, qr);
    }

    int size() {
        return N;
    }
};

#endif /* DATASTRUCTURES_TREES_COMPACTSEGMENTTREES_H_ */
