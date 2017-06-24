package datastructures.math;

public class DynamicLazySegmentTree {
    private Node root;
    private int N;
    private int[] array;
    
    private static class Node {
        public Node left, right;
        public int val, lazy;

        public Node(int val) {
            this.val = val;
        }
        
        public Node(Node l, Node r) {
            this.left = l;
            this.right = r;
            this.val = Math.max(l.val, r.val);
        }
        
        public void propogate() {
            if (lazy != 0) {
                if (left != null) {
                    left.val += lazy;
                    left.lazy += lazy;
                }
                if (right != null) {
                    right.val += lazy;
                    right.lazy += lazy;
                }
                lazy = 0;
            }
        }
    }
    
    public DynamicLazySegmentTree(int size) {
        array = new int[size + 1];
        root = build(1, size);
        N = size;
    }
    
    public DynamicLazySegmentTree(int size, int[] arr, boolean oneIndexed) {
        array = new int[size + 1];
        for (int i = 1; i <= size; i++) {
            array[i] = arr[i - (oneIndexed ? 0 : 1)];
        }
        root = build(1, size);
        N = size;
    }
    
    public int size() {
        return N;
    }
    
    private Node build(int cL, int cR) {
        if (cL == cR) return new Node(array[cL]);
        int m = cL + (cR - cL) / 2;
        return new Node(build(cL , m), build(m + 1, cR));
    }
    
    public void update(int l, int r, int val) {
        root = update(root, 1, N, l, r, val);
    }
    
    private Node update(Node cur, int cL, int cR, int l, int r, int val) {
        cur.propogate();
        if (cL > r || cR < l) return cur;
        if (cL >= l && cR <= r) {
            cur.val += val;
            cur.lazy += val;
            return cur;
        }
        int m = cL + (cR - cL) / 2;
        return new Node(update(cur.left, cL, m, l, r, val), update(cur.right, m + 1, cR, l, r, val));
    }
    
    public int rMaxQ(int l, int r) {
        return rMaxQ(root, 1, N, l, r);
    }
    
    private int rMaxQ(Node cur, int cL, int cR, int l, int r) {
        cur.propogate();
        if (cL > r || cR < l) return Integer.MIN_VALUE;
        if (cL >= l && cR <= r) return cur.val;
        int m = cL + (cR - cL) / 2;
        int left = rMaxQ(cur.left, cL, m, l, r);
        int right = rMaxQ(cur.right, m + 1, cR, l, r);
        return Math.max(left, right);
    }
}