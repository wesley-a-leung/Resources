package datastructures.math;

public class DynamicLazySegmentTree {
	private Node root;
	private int size;
	
	private static class Node {
		public Node left, right;
		public int max, lazy;
		
		public Node() {
		}
		
		public Node(Node l, Node r) {
			this.left = l;
			this.right = r;
			this.max = Math.max(l.max, r.max);
		}
		
		public void propogate() {
			if (lazy != 0) {
				max += lazy;
				if (left != null) {
					left.lazy += lazy;
				}
				if (right != null) {
					right.lazy += lazy;
				}
				lazy = 0;
			}
		}
	}
	
	public DynamicLazySegmentTree(int size) {
		root = build(1, size);
		this.size = size;
	}
	
	public int size() {
		return this.size;
	}
	
	private Node build(int cL, int cR) {
		if (cL == cR) return new Node();
		int m = (cL + cR) >> 1;
		return new Node(build(cL , m), build(m + 1, cR));
	}
	
	public void update(int l, int r, int val) {
		root = update(root, 1, size, l, r, val);
	}
	
	private Node update(Node cur, int cL, int cR, int l, int r, int val) {
		cur.propogate();
		if (cL > r || cR < l) return cur;
		if (cL >= l && cR <= r) {
			cur.lazy += val;
			cur.propogate();
			return cur;
		}
		int m = (cL + cR) >> 1;
		return new Node(update(cur.left, cL, m, l, r, val), update(cur.right, m + 1, cR, l, r, val));
	}
}