package algorithms.sort;

/**
 *  The {@code Counting Sort} class provides static methods for sorting an
 *  array using counting sort. This sorting algorithm only works with integer values.
 *  <p>
 *  Best Case: <em>N</em> + <em>K</em>
 *  <p>
 *  Average Case: <em>N</em> + <em>K</em>
 *  <p>
 *  Worse Case: <em>N</em> + <em>K</em>
 *  <p>
 *  Space: <em>N</em> + <em>K</em>
 *
 *  @author Wesley Leung
 */
public class CountingSort {

    // This class should not be instantiated.
    private CountingSort() { }

    /**
     * Rearranges the array in ascending order, using the natural order.
     * @param a the array to be sorted
     */
    public static void sort(int[] a) {
    	int max = 0;
		for (int x : a)
			max = Math.max(max, x);
		int[] count = new int[max + 1];
		for (int x : a)
			count[x]++;
		for (int i = 1; i < count.length; i++)
			count[i] += count[i - 1];
		int[] b = new int[a.length];
		for (int i = 0; i < a.length; i++)
			b[--count[a[i]]] = a[i];
		for (int i = 0; i < a.length; i++)
			a[i] = b[i];
    }
}