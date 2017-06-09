package datastructures.math;

public class DynamicSegmentTree {
	private int[] arr;
	private Node root;
	private int size;
	
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
	
	public DynamicSegmentTree(int size, int[] array) {
		this.arr = new int[size + 1];
		for (int i = 1; i <= size; i++) {
			arr[i] = array[i - 1];
		}
		root = build(1, size);
		this.size = size;
	}
	
	public DynamicSegmentTree(int size) {
		this.arr = new int[size + 1];
		root = build(1, size);
		this.size = size;
	}
	
	public int size() {
		return this.size;
	}
	
	private Node build(int cL, int cR) {
		if (cL == cR) return new Node(arr[cL]);
		int m = (cL + cR) >> 1;
		return new Node(build(cL , m), build(m + 1, cR));
	}
	
	public void update(int ind, int val) {
		arr[ind] = val;
		root = update(root, 1, arr.length - 1, ind);
	}
	
	private Node update(Node cur, int cL, int cR, int ind) {
		if (cL <= ind && ind <= cR) {
			if (cL == cR) return new Node(arr[cL]);
			int m = (cL + cR) >> 1;
			return new Node(update(cur.left, cL, m, ind), update(cur.right, m + 1, cR, ind));
		}
		return cur;
	}
	
	public int maxPSA(int l, int r) {
		return query(root, 1, arr.length - 1, l, r).pre;
	}
	
	public int maxSSA(int l, int r) {
		return query(root, 1, arr.length - 1, l, r).suf;
	}
	
	private Query query(Node cur, int cL, int cR, int l, int r) {
		if (cL > r || cR < l) return null;
		if (cL >= l && cR <= r) return new Query(cur.pre, cur.suf, cur.sum);
		int m = (cL + cR) >> 1;
		Query left = query(cur.left, cL, m, l, r);
		Query right = query(cur.right, m + 1, cR, l, r);
		if (left == null) return right;
		if (right == null) return left;
		return new Query(left, right);
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
