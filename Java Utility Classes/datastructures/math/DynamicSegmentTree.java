package datastructures.math;

public class DynamicSegmentTree {
    private Node root;
    private int N;
    
    private static class Node {
        public Node left = null, right = null;
        public int val = 0;
    }
    
    public DynamicSegmentTree(int size) {
        root = new Node();
        N = size;
    }
    
    public int size() {
        return N;
    }
      
    public void update(int ind, int val) {
        update(root, 1, N, ind, val);
    }
    
    private void update(Node cur, int cL, int cR, int ind, int val) {
        if (cL > ind || cR < ind) return;
        if (cL >= ind && cR <= ind) {
            cur.val += val;
            return;
        }
        int m = cL + (cR - cL) / 2;
        if (cur.left == null) cur.left = new Node();
        update(cur.left, cL, m, ind, val);
        if (cur.right == null) cur.right = new Node();
        update(cur.right, m + 1, cR, ind, val);
        cur.val = Math.max(cur.left.val, cur.right.val);
     }
    
    public int query(int l, int r) {
        return query(root, 1, N, l, r);
    }
    
    private int query(Node cur, int cL, int cR, int l, int r) {
        if (cur == null || cL > r || cR < l) return 0;
        if (cL >= l && cR <= r) return cur.val;
        int m = cL + (cR - cL) / 2;
        int left = query(cur.left, cL, m, l, r);
        int right = query(cur.right, m + 1, cR, l, r);
        return Math.max(left, right);
    }
}
