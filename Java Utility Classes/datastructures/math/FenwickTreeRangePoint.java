package datastructures.math;

/**
 * FenwickTree supporting range updates and point queries
 * Memory usage:  O(n)
 *
 * @author Wesley Leung
 */
public class FenwickTreeRangePoint {

    private int[] array; // 1-indexed array, In this array We save cumulative information to perform efficient range queries and updates

    public FenwickTreeRangePoint(int size) {
        array = new int[size + 1];
    }

    /**
     * Value at index {@code ind}.
     * <p>
     * Time-Complexity:    O(log(n))
     *
     * @param  ind index
     * @return sum
     */
    public int getValue(int ind) {
        int sum = 0;
        for (int x = ind; x > 0; x -= (x & -x)) {
            sum += array[x];
        }
        return sum;
    }

    /**
     * Update the array at ind and all the affected regions above ind.
     * ind is 1-indexed
     * <p>
     * Time-Complexity:    O(log(n))
     *
     * @param  ind   index
     * @param  value value
     */
    public void update(int ind, int value) {
        for (int x = ind; x < array.length; x += (x & -x)) {
            array[x] += value;
        }
    }
    
    /**
     * Update the array between a and b.
     * ind is 1-indexed
     * <p>
     * Time-Complexity:    O(log(n))
     *
     * @param  ind   index
     * @param  value value
     */
    public void update(int a, int b, int value) {
        update(a, value);
        update(b + 1, -value);
    }

    public int size() {
        return array.length - 1;
    }
}
