package algorithms.math;

import java.util.Arrays;

/**
 * Offline algorithm that reorders queries to solve them
 * 
 * Take time proportional to <em>M</em> sqrt <em>N</em> * <em>cost</em>
 * where M is the number of queries, N is the length of the array, and
 * cost is the cost of each operation.
 * 
 * @author Wesley Leung
 *
 */
public class Mo {
	private int n, m, sz, res;
	private int[] cnt, a, ans;

	public Mo(int[] arr, int[][] queries) {
		n = arr.length;
		for (int i = 1; i <= n; i++) {
			a[i] = arr[i - 1];
		}
		m = queries.length;
		Query[] q = new Query[m];
		for (int i = 0; i < m; i++) {
			q[i] = new Query(queries[i][0], queries[i][1], i);
		}
		Arrays.sort(q);
		cnt = new int[1000001];
		ans = new int[m];
		int l = 1, r = 0;
		for (Query query : q) {
			while (r > query.r) {
				remove(a[r]);
				r--;
			}
			while (r < query.r) {
				r++;
				update(a[r]);
			}
			while (l < query.l) {
				remove(a[l]);
				l++;
			}
			while (l > query.l) {
				l--;
				update(a[l]);
			}
			ans[query.index] = res;
		}
	}
	
	public int query(int i) {
		return ans[i];
	}
	
	private void update(int i) {
		cnt[i]++;
		if (cnt[i] == 1)
			res++;
	}
	
	private void remove(int i) {
		cnt[i]--;
		if (cnt[i] == 0)
			res--;
	}
	
	public class Query implements Comparable<Query> {
		int l, r, index;

		Query(int l, int r, int index) {
			this.l = l;
			this.r = r;
			this.index = index;
		}
		
		@Override
		public int compareTo(Query o) {
			if ((l - 1) / sz != (o.l - 1) / sz)
				return (l - 1) / sz - (o.l - 1) / sz;
			return r - o.r;
		}
	}
}
