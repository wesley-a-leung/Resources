package datastructures.math;

/**
 * Memory usage:  O(n^3)
 *
 * @author Wesley Leung
 */
public class FenwickTree3D {

    int[][][] array;

    public FenwickTree3D(int sizeX, int sizeY, int sizeZ) {
        array = new int[sizeX + 1][sizeY + 1][sizeZ + 1];
    }

    /**
     * Range Sum Query
     * <p>
     * Time-Complexity:    O(log(n)^3)
     *
     * @param  indX x index
     * @param  indY y index
     * @param  indZ z index
     * @return sum
     */
    public int rsq(int indX, int indY, int indZ) {
        int sum = 0;
        for (int x = indX; x > 0; x -= (x & -x)) {
        	for (int y = indY; y > 0; y -= (y & -y)) {
        		for (int z = indZ; z > 0; z -= (z & -z)) {
        			sum += array[x][y][z];
        		}
        	}
        }
        return sum;
    }

    /**
     * Range Sum Query
     * <p>
     * Time-Complexity:    O(log(n)^3)
     *
     * @param  x1 top index
     * @param  y1 left index
     * @param  z1 from index
     * @param  x2 bottom index
     * @param  y2 right index
     * @param  z2 back index
     * @return sum
     */
    public int rsq(int x1, int y1, int z1, int x2, int y2, int z2) {
    	return rsq(x2, y2, z2) 
    			- rsq(x1 - 1, y2, z2) - rsq(x2, y1 - 1, z2) - rsq(x2, y2, z1 - 1)
    			+ rsq(x1 - 1, y1 - 1, z2) + rsq(x1 - 1, y2, z1 - 1) + rsq(x2, y1 - 1, z1 - 1)
    			- rsq(x1 - 1, y1 - 1, z1 - 1);
    }

    /**
     * Update the array at [indX][indY][indZ] and all the affected regions above ind.
     * ind is 1-indexed
     * <p>
     * Time-Complexity:    O(log(n)^3)
     *
     * @param  indX   x index
     * @param  indY   y index
     * @param  indZ   z index
     * @param  value value
     */
    public void update(int indX, int indY, int indZ, int value) {
    	for (int x = indX; x < array.length; x += (x & -x)) {
    		for (int y = indY; y < array[x].length; y += (y & -y)) {
    			for (int z = indZ; z < array[x][y].length; z += (z & -z)) {
    				array[x][y][z] += value;
    			}
    		}
    	}
    }

    public int sizeX() {
        return array.length - 1;
    }
    
    public int sizeY() {
    	return array[0].length - 1;
    }
    
    public int sizeZ() {
    	return array[0][0].length - 1;
    }
}
