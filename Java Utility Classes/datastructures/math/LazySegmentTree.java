package datastructures.math;

public class LazySegmentTree {
    private Node[] tree;
    private int[] array;
    private int N;
    
    private static class Node {
        public int maxVal, lazy;
    }
    
    public LazySegmentTree(int size, int[] arr) {
        array = new int[size + 1];
        for (int i = 1; i <= size; i++) {
            array[i] = arr[i - 1];
        }
        tree = new Node[4 * size];
        build(1, 1, size);
        N = size;
    }

    public LazySegmentTree(int size) {
        array = new int[size + 1];
        tree = new Node[4 * size];
        build(1, 1, size);
        N = size;
    }
    
    private void propogate(int cur) {
        if (tree[cur].lazy != 0) {
            tree[cur * 2].maxVal += tree[cur].lazy;
            tree[cur * 2].lazy += tree[cur].lazy;
            tree[cur * 2 + 1].maxVal += tree[cur].lazy;
            tree[cur * 2 + 1].lazy += tree[cur].lazy;
            tree[cur].lazy = 0;
        }
    }

    private void build(int cur, int cL, int cR) {
        if (cL == cR) {
            tree[cur].maxVal = array[cL];
            return;
        }
        int m = cL + (cR - cL) / 2;
        build(cur * 2, cL , m);
        build(cur * 2 + 1, m + 1, cR);
    }

    private void update(int cur, int cL, int cR, int l, int r, int val) {
        if (cL != cR) propogate(cur);
        if (cL > r || cR < l) return;
        if (cL >= l && cR <= r) {
            tree[cur].maxVal += val;
            tree[cur].lazy += val;
            return;
        }
        int m = cL + (cR - cL) / 2;
        update(cur * 2, cL, m, l, r, val);
        update(cur * 2 + 1, m + 1, cR, l, r, val);
        tree[cur].maxVal = Math.max(tree[cur * 2].maxVal, tree[cur * 2 + 1].maxVal);
    }
    
    public void update(int l, int r, int val) {
        update(1, 1, N, l, r, val);
    }

    private int rMaxQ(int cur, int cL, int cR, int l, int r) {
        if (cL != cR) propogate(cur);
        if (cL > r || cR < l) return Integer.MIN_VALUE;
        if (cL >= l && cR <= r) return tree[cur].maxVal;
        int m = cL + (cR - cL) / 2;
        int left = rMaxQ(cur * 2, cL, m, l, r);
        int right = rMaxQ(cur * 2 + 1, m + 1, cR, l, r);
        return Math.max(left, right);
    }

    public int rMaxQ(int l, int r) {
        return rMaxQ(1, 1, N, l, r);
    }
    
    public int size() {
        return N;
    }
}
