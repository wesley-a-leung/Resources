package datastructures.math;

/**
 * Memory usage:  O(n^2)
 *
 * @author Wesley Leung
 */
public class FenwickTree2D {

    int[][] array;

    public FenwickTree2D(int size) {
        array = new int[size + 1][size + 1];
    }

    /**
     * Range Sum Query
     * <p>
     * Time-Complexity:    O(log(n)^2)
     *
     * @param  indX x index
     * @param  indY y index
     * @return sum
     */
    public int rsq(int indX, int indY) {
        int sum = 0;
        int y = indY;
        while (indX > 0) {
           indY = y;
           while (indY > 0) {
        	   sum += array[indX][indY];
        	   indY -= (indY & -indY);
           }
           indX -= (indX & -indX);
        }

        return sum;
    }

    /**
     * Range Sum Query
     * <p>
     * Time-Complexity:    O(log(n)^2)
     *
     * @param  x1 top index
     * @param  y1 left index
     * @param  x2 bottom index
     * @param  y2 right index
     * @return sum
     */
    public int rsq(int x1, int y1, int x2, int y2) {
        return rsq(x2, y2) + rsq(x1, y1) - rsq(x1, y2) - rsq(x2, y1);
    }

    /**
     * Update the array at [indX][indY] and all the affected regions above ind.
     * ind is 1-indexed
     * <p>
     * Time-Complexity:    O(log(n)^2)
     *
     * @param  indX   x index
     * @param  indY   y index
     * @param  value value
     */
    public void update(int indX, int indY, int value) {
    	int y = indY;
        while (indX < array.length) {
        	indY = y;
        	while (indY <= array[indX].length) {
        		array[indX][indY] += value;
        		indY += (indY & -indY);
        	}
            indX += (indX & -indX);
        }
    }

    public int size() {
        return array.length - 1;
    }
}
