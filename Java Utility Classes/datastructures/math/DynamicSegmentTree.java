package datastructures.math;

public class DynamicSegmentTree {
    private int[] array;
    private Node root;
    private int N;
    
    private static class Node {
        public Node left, right;
        public int val;
        
        public Node(int val) {
            this.val = val;
        }
        
        public Node(Node l, Node r) {
            this.left = l;
            this.right = r;
            this.val = Math.max(l.val, r.val);
        }
    }
    
    public DynamicSegmentTree(int size) {
        array = new int[size + 1];
        root = build(1, size);
        N = size;
    }
    
    public DynamicSegmentTree(int size, int[] arr) {
        array = new int[size + 1];
        for (int i = 1; i <= size; i++) {
            arr[i] = arr[i - 1];
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
    
    public void update(int ind, int val) {
        array[ind] = val;
        root = update(root, 1, array.length - 1, ind);
    }
    
    private Node update(Node cur, int cL, int cR, int ind) {
        if (cL <= ind && ind <= cR) {
            if (cL == cR) return new Node(array[cL]);
            int m = cL + (cR - cL) / 2;
            return new Node(update(cur.left, cL, m, ind), update(cur.right, m + 1, cR, ind));
        }
        return cur;
    }
    
    public int rMaxQ(int ind) {
        return rMaxQ(root, 1, N, ind);
    }
    
    private int rMaxQ(Node cur, int cL, int cR, int ind) {
        if (cL > ind || cR < ind) return Integer.MIN_VALUE;
        if (cL >= ind && cR <= ind) return cur.val;
        int m = cL + (cR - cL) / 2;
        int left = rMaxQ(cur.left, cL, m, ind);
        int right = rMaxQ(cur.right, m + 1, cR, ind);
        return Math.max(left, right);
    }
}
