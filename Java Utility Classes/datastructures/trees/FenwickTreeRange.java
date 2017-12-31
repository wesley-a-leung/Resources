package datastructures.trees;

/**
 * FenwickTree supporting range updates and range queries
 * Memory usage:  O(2n)
 *
 * @author Wesley Leung
 */
public class FenwickTreeRange {

    private int[] array1;
    private int[] array2;

    public FenwickTreeRange(int size) {
        array1 = new int[size + 1];
        array2 = new int[size + 1];
    }

    private int rsq(int[] array, int ind) {
        int sum = 0;
        for (int x = ind; x > 0; x -= (x & -x)) {
            sum += array[x];
        }
        return sum;
    }
    
    /**
     * Range Sum query at index ind
     * ind is 1-indexed
     * <p>
     * Time-Complexity:    O(log(n))
     *
     * @param  ind index
     * @return sum
     */
    public int rsq(int ind) {
        return rsq(array1, ind) * ind - rsq(array2, ind);
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
    
    private void update(int[] array, int ind, int value) {
        for (int x = ind; x < array.length; x += (x & -x)) {
            array[x] += value;
        }
    }
    
    /**
     * Update the array between index a and b
     * ind is 1-indexed
     * <p>
     * Time-Complexity:    O(log(n))
     *
     * @param  ind   index
     * @param  value value
     */
    public void update(int a, int b, int value) {
        update(array1, a, value);
        update(array1, b + 1, -value);
        update(array2, a, value * (a - 1));
        update(array2, b + 1, -value * b);
    }

    public int size() {
        return array1.length - 1;
    }
}
