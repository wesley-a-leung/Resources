package datastructures.math;

public class SegmentTree {
    private Node[] tree;
    private int[] array;
    private int N;
    
    private static class Node {
        public int val;
        public Node(int val) {
            this.val = val;
        }
        
        public Node(Node left, Node right) {
            this.val = Math.max(left.val, right.val);
        }
    }
    
    public SegmentTree(int size, int[] arr, boolean oneIndexed) {
        array = new int[size + 1];
        for (int i = 1; i <= size; i++) {
            array[i] = arr[i - (oneIndexed ? 0 : 1)];
        }
        tree = new Node[4 * size];
        build(1, 1, size);
        N = size;
    }

    public SegmentTree(int size) {
        array = new int[size + 1];
        tree = new Node[4 * size];
        build(1, 1, size);
        N = size;
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

    private void update(int cur, int cL, int cR, int ind, int val) {
        if (cL > ind || cR < ind) return;
        if (cL >= ind && cR <= ind) {
            tree[cur].val = val;
            return;
        }
        int m = cL + (cR - cL) / 2;
        update(cur * 2, cL, m, ind, val);
        update(cur * 2 + 1, m + 1, cR, ind, val);
        tree[cur].val = Math.max(tree[cur * 2].val, tree[cur * 2 + 1].val);
    }
    
    public void update(int ind, int val) {
        update(1, 1, N, ind, val);
    }

    private int rMaxQ(int cur, int cL, int cR, int l, int r) {
        if (cL > r || cR < l) return 0;
        if (cL >= l && cR <= r) return tree[cur].val;
        int m = cL + (cR - cL) / 2;
        int left = rMaxQ(cur * 2, cL, m, l, r);
        int right = rMaxQ(cur * 2 + 1, m + 1, cR, l, r);
        return Math.max(left, right);
    }

    public int rMaxQ(int l, int r) {
        return rMaxQ(1, 1, N, l, r);
    }
}
