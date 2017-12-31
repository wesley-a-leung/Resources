package datastructures.trees;

public class LazySegmentTree {
    private Node[] tree;
    private int[] array;
    private int N;
    
    private static class Node {
        public int val, lazy;
        public Node(int val) {
            this.val = val;
        }
        
        public Node(Node left, Node right) {
            this.val = Math.max(left.val, right.val);
        }
    }
    
    public LazySegmentTree(int size, int[] arr, boolean oneIndexed) {
        array = new int[size + 1];
        for (int i = 1; i <= size; i++) {
            array[i] = arr[i - (oneIndexed ? 0 : 1)];
        }
        tree = new Node[((int) (2 * Math.pow(2.0, Math.ceil(Math.log((double) size) / Math.log(2.0)))))];
        build(1, 1, size);
        N = size;
    }

    public LazySegmentTree(int size) {
        array = new int[size + 1];
        tree = new Node[((int) (2 * Math.pow(2.0, Math.ceil(Math.log((double) size) / Math.log(2.0)))))];
        build(1, 1, size);
        N = size;
    }
    
    private void propogate(int cur) {
        if (tree[cur].lazy != 0) {
            tree[cur * 2].val += tree[cur].lazy;
            tree[cur * 2].lazy += tree[cur].lazy;
            tree[cur * 2 + 1].val += tree[cur].lazy;
            tree[cur * 2 + 1].lazy += tree[cur].lazy;
            tree[cur].lazy = 0;
        }
    }

    private void build(int cur, int cL, int cR) {
        if (cL == cR) {
            tree[cur] = new Node(array[cL]);
            return;
        }
        int m = cL + (cR - cL) / 2;
        build(cur * 2, cL , m);
        build(cur * 2 + 1, m + 1, cR);
        tree[cur] = new Node(tree[cur * 2], tree[cur * 2 + 1]);
    }

    private void update(int cur, int cL, int cR, int l, int r, int val) {
        if (cL != cR) propogate(cur);
        if (cL > r || cR < l) return;
        if (cL >= l && cR <= r) {
            tree[cur].val += val;
            tree[cur].lazy += val;
            return;
        }
        int m = cL + (cR - cL) / 2;
        update(cur * 2, cL, m, l, r, val);
        update(cur * 2 + 1, m + 1, cR, l, r, val);
        tree[cur].val = Math.max(tree[cur * 2].val, tree[cur * 2 + 1].val);
    }
    
    public void update(int l, int r, int val) {
        update(1, 1, N, l, r, val);
    }

    private int query(int cur, int cL, int cR, int l, int r) {
        if (cL != cR) propogate(cur);
        if (cL > r || cR < l) return 0;
        if (cL >= l && cR <= r) return tree[cur].val;
        int m = cL + (cR - cL) / 2;
        int left = query(cur * 2, cL, m, l, r);
        int right = query(cur * 2 + 1, m + 1, cR, l, r);
        return Math.max(left, right);
    }

    public int query(int l, int r) {
        return query(1, 1, N, l, r);
    }
    
    public int size() {
        return N;
    }
}
