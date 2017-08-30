package datastructures.geometry;

/**
 *  The {@code Vector} class represents a <em>d</em>-dimensional Euclidean vector.
 *  Vectors are immutable: their values cannot be changed after they are created.
 *  It includes methods for addition, subtraction,
 *  dot product, scalar product, unit vector, Euclidean norm, and the Euclidean
 *  distance between two vectors.
 *  <p>
 *  For additional documentation, 
 *  see <a href="http://algs4.cs.princeton.edu/12oop">Section 1.2</a> of 
 *  <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne. 
 *
 *  @author Robert Sedgewick
 *  @author Kevin Wayne
 */
public class Vector { 

    private int d;               // dimension of the vector
    private double[] data;       // array of vector's components


    /**
     * Initializes a d-dimensional zero vector.
     *
     * @param d the dimension of the vector
     */
    public Vector(int d) {
        this.d = d;
        data = new double[d];
    }

    /**
     * Initializes a vector from either an array or a vararg list.
     * The vararg syntax supports a constructor that takes a variable number of
     * arugments such as Vector x = new Vector(1.0, 2.0, 3.0, 4.0).
     *
     * @param a  the array or vararg list
     */
    public Vector(double... a) {
        d = a.length;

        // defensive copy so that client can't alter our copy of data[]
        data = new double[d];
        for (int i = 0; i < d; i++)
            data[i] = a[i];
    }

    /**
     * Returns the dimension of this vector.
     *
     * @return the dimension of this vector
     */
    public int dimension() {
        return d;
    }

    /**
     * Returns the dot product of this vector with the specified vector.
     *
     * @param  that the other vector
     * @return the dot product of this vector and that vector
     * @throws IllegalArgumentException if the dimensions of the two vectors are not equal
     */
    public double dot(Vector that) {
        if (this.d != that.d) throw new IllegalArgumentException("Dimensions don't agree");
        double sum = 0.0;
        for (int i = 0; i < d; i++)
            sum = sum + (this.data[i] * that.data[i]);
        return sum;
    }
    
    /**
     * Returns the 2-D cross product of this vector with the specified vector.
     *
     * @param  that the other vector
     * @return the 2-D cross product of this vector and that vector
     * @throws IllegalArgumentException if the two vectors are not 2-dimensional
     */
    public double cross2D(Vector that) {
        if (this.d != 2 || that.d != 2) throw new IllegalArgumentException("Vectors must be 2-dimensional");
        return this.data[0] * that.data[1] - this.data[1] * that.data[0];
    }
    
    /**
     * Returns the 3-D cross product of this vector with the specified vector.
     *
     * @param  that the other vector
     * @return the 3-D cross product of this vector and that vector
     * @throws IllegalArgumentException if the two vectors are not 3-dimensional
     */
    public Vector cross3D(Vector that) {
        if (this.d != 3 || that.d != 3) throw new IllegalArgumentException("Vectors must be 3-dimensional");
        return new Vector(this.data[1] * that.data[2] - this.data[2] * that.data[1],
                          this.data[2] * that.data[0] - this.data[0] * that.data[2],
                          this.data[0] * that.data[1] - this.data[1] * that.data[0]);
    }

    /**
     * Returns the magnitude of this vector.
     * This is also known as the L2 norm or the Euclidean norm.
     *
     * @return the magnitude of this vector
     */
    public double magnitude() {
        return Math.sqrt(this.dot(this));
    }

    /**
     * Returns the Euclidean distance between this vector and the specified vector.
     *
     * @param  that the other vector 
     * @return the Euclidean distance between this vector and that vector
     * @throws IllegalArgumentException if the dimensions of the two vectors are not equal
     */
    public double distanceTo(Vector that) {
        if (this.d != that.d) throw new IllegalArgumentException("Dimensions don't agree");
        return this.minus(that).magnitude();
    }

    /**
     * Returns the sum of this vector and the specified vector.
     *
     * @param  that the vector to add to this vector
     * @return the vector whose value is {@code (this + that)}
     * @throws IllegalArgumentException if the dimensions of the two vectors are not equal
     */
    public Vector plus(Vector that) {
        if (this.d != that.d) throw new IllegalArgumentException("Dimensions don't agree");
        Vector c = new Vector(d);
        for (int i = 0; i < d; i++)
            c.data[i] = this.data[i] + that.data[i];
        return c;
    }

    /**
     * Returns the difference between this vector and the specified vector.
     *
     * @param  that the vector to subtract from this vector
     * @return the vector whose value is {@code (this - that)}
     * @throws IllegalArgumentException if the dimensions of the two vectors are not equal
     */
    public Vector minus(Vector that) {
        if (this.d != that.d) throw new IllegalArgumentException("Dimensions don't agree");
        Vector c = new Vector(d);
        for (int i = 0; i < d; i++)
            c.data[i] = this.data[i] - that.data[i];
        return c;
    }

    /**
     * Returns the ith cartesian coordinate.
     *
     * @param  i the coordinate index
     * @return the ith cartesian coordinate
     */
    public double cartesian(int i) {
        return data[i];
    }

    /**
     * Returns the scalar-vector product of this vector and the specified scalar
     *
     * @param  alpha the scalar
     * @return the vector whose value is {@code (alpha * this)}
     */
    public Vector scale(double alpha) {
        Vector c = new Vector(d);
        for (int i = 0; i < d; i++)
            c.data[i] = alpha * data[i];
        return c;
    }

    /**
     * Returns a unit vector in the direction of this vector.
     *
     * @return a unit vector in the direction of this vector
     * @throws ArithmeticException if this vector is the zero vector
     */
    public Vector direction() {
        if (this.magnitude() == 0.0) throw new ArithmeticException("Zero-vector has no direction");
        return this.scale(1.0 / this.magnitude());
    }
    
    /**
     * Returns the angle of this 2-D vector to horizontal.
     * 
     * 
     * @return the angle of this 2-D vector to horizontal
     * @throws IllegalArgumentException if this vector is not 2-dimensional
     */
    public double angle() {
        if (this.d != 2) throw new IllegalArgumentException("Vector must be 2-dimensional");
        return Math.atan2(this.data[1], this.data[0]);
    }
    
    /**
     * Returns a projection of this vector on that vector.
     *
     * @param that the vector that this vector is to be projected on
     * @return a projection of this vector on that vector
     * @throws IllegalArgumentException if the dimensions of the two vectors are not equal
     */
    public Vector projectionOn(Vector that) {
        if (this.d != that.d) throw new IllegalArgumentException("Dimensions don't agree");
        return that.scale(dot(that) / (that.magnitude() * that.magnitude()));
    }
    
    /**
     * Returns a vector that is this vector rotated theta radians around that vector.
     *
     * @param that vector representing the axis of rotation
     * @param theta the angle in radians
     * @return a vector that is this vector rotated theta radians around that vector
     * @throws IllegalArgumentException if the dimensions of the two vectors are not equal
     * @throws IllegalArgumentException if the two vectors are not 2-dimensional or 3-dimensional
     */
    public Vector rotate(Vector that, double theta) {
        if (d == 2 && that.d == 2) {
            Vector r = new Vector(2);
            r.data[0] = that.data[0] + (data[0] - that.data[0]) * Math.cos(theta) - (data[1] - that.data[1]) * Math.sin(theta);
            r.data[1] = that.data[1] + (data[0] - that.data[0]) * Math.sin(theta) + (data[1] - that.data[1]) * Math.cos(theta);
            return r;
        } else if (d == 3 && that.d == 3) {
            Vector r = new Vector(3);
            r = this.scale(Math.cos(theta)).plus(that.direction().cross3D(this).scale(Math.sin(theta))).plus(that.direction().scale(that.direction().dot(this)).scale(1.0 - Math.cos(theta)));
            return r;
        } else if (d == that.d) {
            throw new IllegalArgumentException("Vectors must be 2-dimensional or 3-dimensional");
        } else {
            throw new IllegalArgumentException("Dimensions don't agree");
        }
    }

    /**
     * Returns a string representation of this vector.
     *
     * @return a string representation of this vector, which consists of the 
     *         the vector entries, separates by single spaces
     */
    public String toString() {
        StringBuilder s = new StringBuilder();
        for (int i = 0; i < d; i++)
            s.append(data[i] + " ");
        return s.toString();
    }
}
