package datastructures.trees;

/**
 * FenwickTree supporting point updates and range queries
 * Memory usage:  O(n)
 *
 * @author Wesley Leung
 */
public class FenwickTree {

    private int[] array; // 1-indexed array, In this array We save cumulative information to perform efficient range queries and updates

    public FenwickTree(int size) {
        array = new int[size + 1];
    }

    /**
     * Range Sum query from 1 to ind
     * ind is 1-indexed
     * <p>
     * Time-Complexity:    O(log(n))
     *
     * @param  ind index
     * @return sum
     */
    public int rsq(int ind) {
        int sum = 0;
        for (int x = ind; x > 0; x -= (x & -x)) {
            sum += array[x];
        }
        return sum;
    }

    /**
     * Range Sum Query from a to b.
     * Search for the sum from array index from a to b
     * a and b are 1-indexed
     * <p>
     * Time-Complexity:    O(log(n))
     *
     * @param  a left index
     * @param  b right index
     * @return sum
     */
    public int rsq(int a, int b) {
        return rsq(b) - rsq(a - 1);
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

    public int size() {
        return array.length - 1;
    }
}
