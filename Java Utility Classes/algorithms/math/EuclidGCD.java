package algorithms.math;


/******************************************************************************
 *  Compilation:  javac Euclid.java
 *  Execution:    java Euclid p q
 *  
 *  Reads two command-line arguments p and q and computes the greatest
 *  common divisor of p and q using Euclid's algorithm.
 *
 *  Remarks
 *  -----------
 *    - may return the negative of the gcd if p is negative
 *
 ******************************************************************************/

public class EuclidGCD {
    
    /**
     * This class should not be instantiated.
     */
    private EuclidGCD() { }

    // non-recursive implementation
    /**
     * Returns the greatest common divisor of 2 integers.
     *
     * @param  p the first integer
     * @param  q the second integer
     * @return greatest common divisor of {@code p} and {@code q}
     */
    public static int gcd(int p, int q) {
        while (q != 0) {
            int temp = q;
            q = p % q;
            p = temp;
        }
        return p;
    }
    
    // recursive implementation
    /**
     * Returns the greatest common divisor of 2 integers.
     *
     * @param  p the first integer
     * @param  q the second integer
     * @return greatest common divisor of {@code p} and {@code q}
     */
    public static int gcdRecursive(int p, int q) {
        return q == 0 ? p : gcd(q, p % q);
    }
}
