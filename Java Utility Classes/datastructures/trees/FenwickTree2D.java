package datastructures.trees;

/**
 * Memory usage:  O(nm)
 *
 * @author Wesley Leung
 */
public class FenwickTree2D {

    private int[][] array;

    public FenwickTree2D(int sizeX, int sizeY) {
        array = new int[sizeX + 1][sizeY + 1];
    }

    /**
     * Range Sum Query
     * <p>
     * Time-Complexity:    O(log(nm))
     *
     * @param  indX x index
     * @param  indY y index
     * @return sum
     */
    public int rsq(int indX, int indY) {
        int sum = 0;
        for (int x = indX; x > 0; x -= (x & -x)) {
            for (int y = indY; y > 0; y -= (y & -y)) {
                sum += array[x][y];
            }
        }
        return sum;
    }

    /**
     * Range Sum Query
     * <p>
     * Time-Complexity:    O(log(nm))
     *
     * @param  x1 top index
     * @param  y1 left index
     * @param  x2 bottom index
     * @param  y2 right index
     * @return sum
     */
    public int rsq(int x1, int y1, int x2, int y2) {
        return rsq(x2, y2) + rsq(x1 - 1, y1 - 1) - rsq(x1 - 1, y2) - rsq(x2, y1 - 1);
    }

    /**
     * Update the array at [indX][indY] and all the affected regions above ind.
     * ind is 1-indexed
     * <p>
     * Time-Complexity:    O(log(nm))
     *
     * @param  indX   x index
     * @param  indY   y index
     * @param  value value
     */
    public void update(int indX, int indY, int value) {
        for (int x = indX; x < array.length; x += (x & -x)) {
            for (int y = indY; y < array[x].length; y += (y & -y)) {
                array[x][y] += value;
            }
        }
    }

    public int sizeX() {
        return array.length - 1;
    }
    
    public int sizeY() {
        return array[0].length - 1;
    }
}
