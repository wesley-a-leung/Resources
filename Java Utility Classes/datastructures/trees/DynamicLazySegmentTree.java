package datastructures.trees;

public class DynamicLazySegmentTree {
    private Node root;
    private int N;
    
    private static class Node {
        public Node left = null, right = null;
        public int val = 0, lazy = 0;
        
        public void propogate() {
            if (lazy != 0) {
                if (left == null) left = new Node();
                left.val += lazy;
                left.lazy += lazy;
                if (right == null) right = new Node();
                right.val += lazy;
                right.lazy += lazy;
                lazy = 0;
            }
        }
    }
    
    public DynamicLazySegmentTree(int size) {
        root = new Node();
        N = size;
    }
    
    public int size() {
        return N;
    }
    
    public void update(int l, int r, int val) {
        update(root, 1, N, l, r, val);
    }
    
    private void update(Node cur, int cL, int cR, int l, int r, int val) {
        if (cL != cR) cur.propogate();
        if (cL > r || cR < l) return;
        if (cL >= l && cR <= r) {
            cur.val += val;
            cur.lazy += val;
            return;
        }
        int m = cL + (cR - cL) / 2;
        if (cur.left == null) cur.left = new Node();
        update(cur.left, cL, m, l, r, val);
        if (cur.right == null) cur.right = new Node();
        update(cur.right, m + 1, cR, l, r, val);
        cur.val = Math.max(cur.left.val, cur.right.val);
    }
    
    public int query(int l, int r) {
        return query(root, 1, N, l, r);
    }
    
    private int query(Node cur, int cL, int cR, int l, int r) {
        if (cur == null || cL > r || cR < l) return 0;
        if (cL != cR) cur.propogate();
        if (cL >= l && cR <= r) return cur.val;
        int m = cL + (cR - cL) / 2;
        int left = query(cur.left, cL, m, l, r);
        int right = query(cur.right, m + 1, cR, l, r);
        return Math.max(left, right);
    }
}