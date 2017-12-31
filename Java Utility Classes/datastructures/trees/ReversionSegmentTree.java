package datastructures.trees;

import java.util.ArrayList;

public class ReversionSegmentTree {
    private int[] array;
    private int N;
    private ArrayList<Node> rev;
    private int revInd = 0;
    
    private static class Node {
        public Node left, right;
        public int pre, suf, sum;
        public Node(int val) {
            this.pre = val;
            this.suf = val;
            this.sum = val;
        }
        
        public Node(Node l, Node r) {
            this.left = l;
            this.right = r;
            this.pre = Math.max(l.pre, r.pre + l.sum);
            this.suf = Math.max(l.suf + r.sum, r.suf);
            this.sum = l.sum + r.sum;
        }
    }
    
    public ReversionSegmentTree(int size, int[] arr, boolean oneIndexed) {
        array = new int[size + 1];
        for (int i = 1; i <= size; i++) {
            array[i] = arr[i - (oneIndexed ? 0 : 1)];
        }
        rev.add(build(1, size));
        N = size;
    }
    
    public ReversionSegmentTree(int size) {
        array = new int[size + 1];
        rev.add(build(1, size));
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
        rev.add(update(rev.get(revInd++), 1, array.length - 1, ind));
    }
    
    private Node update(Node cur, int cL, int cR, int ind) {
        if (cL <= ind && ind <= cR) {
            if (cL == cR) return new Node(array[cL]);
            int m = cL + (cR - cL) / 2;
            return new Node(update(cur.left, cL, m, ind), update(cur.right, m + 1, cR, ind));
        }
        return cur;
    }
    
    public int maxPSA(int l, int r) {
        return query(rev.get(revInd), 1, array.length - 1, l, r).pre;
    }
    
    public int maxSSA(int l, int r) {
        return query(rev.get(revInd), 1, array.length - 1, l, r).suf;
    }
    
    private Query query(Node cur, int cL, int cR, int l, int r) {
        if (cL > r || cR < l) return null;
        if (cL >= l && cR <= r) return new Query(cur.pre, cur.suf, cur.sum);
        int m = cL + (cR - cL) / 2;
        Query left = query(cur.left, cL, m, l, r);
        Query right = query(cur.right, m + 1, cR, l, r);
        if (left == null) return right;
        if (right == null) return left;
        return new Query(left, right);
    }
    
    public void revert(int x) {
        rev.add(rev.get(x));
        revInd++;
    }
    
    private static class Query {
        public int pre, suf, sum;
        public Query(int pre, int suf, int sum) {
            this.pre = pre;
            this.suf = suf;
            this.sum = sum;
        }
        
        public Query(Query l, Query r) {
            this.pre = Math.max(l.pre, r.pre + l.sum);
            this.suf = Math.max(l.suf + r.sum, r.suf);
            this.sum = l.sum + r.sum;
        }
    }
}