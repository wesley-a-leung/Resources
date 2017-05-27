package algorithms.search;

import java.util.Arrays;

/**
 *  The {@code QuickSelect} class provides static methods for selecting the
 *  Kth smallest element in an array
 *  <p>
 *  Best Case: <em>N</em>
 *  <p>
 *  Average Case: <em>N</em>
 *  <p>
 *  Worse Case: <em>N</em>
 *  <p>
 *  Space: 1
 *
 *  @author Wesley Leung
 */
public class QuickSelect {

    // This class should not be instantiated.
    private QuickSelect() { }

    /**
     * Returns the {@code K}th smallest element 
     * @param a the array to be sorted
     */
    public Comparable find (int k, int beg, int end, Comparable[] a) {
		int i = median(a, beg, end, k);
		if (i == k)
			return a[i];
		else if (i < k)
			return find(k, i + 1, end, a);
		return find(k - (a.length - i), beg, i, a);
	}

	private int median (Comparable[] a, int beg, int end, int k) {
		if (end - beg + 1 <= 5) {
			Arrays.sort(a, beg, end + 1);
			return beg + k - 1;
		}

		for (int i = 0; i < (end + 1) / 5; i++) {
			int left = 5 * i;
			int right = left + 4;
			if (right > end)
				right = end;
			int median = median(a, left, right, 3);
			swap(a, median, i);
		}
		return median(a, 0, (end + 1) / 5, (end + 1) / 10);
	}
	
	private static void swap (Comparable[] a, int i, int j) {
		Comparable temp = a[i];
		a[i] = a[j];
		a[j] = temp;
	}

}