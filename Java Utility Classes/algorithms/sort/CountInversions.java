package algorithms.sort;

/**
 * Counts the number of inversions given an array. An inversion is two elements at index i and j,
 * such that i < j and A[i] > B[i].
 * 
 * @author Wesley Leung
 *
 */
public class CountInversions {
	
	private static long count; // number of inversions
	
	// merges and counts inversions
	private static void merge(int[] a, int[] aux, int lo, int mid, int hi) {
		for (int k = lo; k <= hi; k++) {
			aux[k] = a[k];
		}
		int i = lo, j = mid + 1;
		for (int k = lo; k <= hi; k++) {
			if (i > mid) { // no inversions if the lower half is completed
				a[k] = aux[j++];
			} else if (j > hi) { // if the upper half is completed, the number of inversions
			                     // is equal to the size of the upper half
				a[k] = aux[i++];
				count += j - (mid + 1);
			} else if (aux[i] <= aux[j]) { // if the lower half element is smaller, the number of inversions
			                               // is equal to the number of elements taken in the upper half
				a[k] = aux[i++];
				count += j - (mid + 1);
			} else {                       // if the upper half element is smaller, the number of inversions
			                               // is equal to the number of elements not taken in the lower half
				a[k] = aux[j++];
				count += mid + 1 - i;
			}
		}
	}
	
	private static void countInversions(int[] a, int[] aux, int lo, int hi) {
		if (lo >= hi) return; // base case is interval of 1; if lo is greater than or equal to hi,
		                      // then the current interval is less than 1
		int mid = lo + (hi - lo) / 2; // to prevent integer overflow
		// recursively divides into 2 subarrays
		countInversions(a, aux, lo, mid);
		countInversions(a, aux, mid + 1, hi);
		// merges subarrays and counts inversions
		merge(a, aux, lo, mid, hi);
	}
	
	/**
	 * Counts the number of inversions in an array.
	 * 
	 * @param a the array
	 * @return the number of inversions in {@code a}
	 */
	public static long countInversions(int[] a) {
		int[] aux = new int[a.length];
		count = 0;
		countInversions(a, aux, 0, a.length - 1);
		return count / 2; // since each inversion is counted twice
	}
}