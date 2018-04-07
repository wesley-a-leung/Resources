#ifndef DATASTRUCTURES_GEOMETRY_VECTOR_H_
#define DATASTRUCTURES_GEOMETRY_VECTOR_H_

#include <bits/stdc++.h>
using namespace std;

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
struct Vector {
private:
    int d;               // dimension of the vector
    double *data;        // array of vector's components

public:
    Vector() {}

    /**
     * Initializes a d-dimensional zero vector.
     *
     * @param d the dimension of the vector
     */
    Vector(int d) {
        this->d = d;
        this->data = new double[d];
        for (int i = 0; i < d; i++) data[i] = 0.0;
    }

    /**
     * Initializes a vector from an array.
     *
     * @param d the dimension of the vector
     * @param a the array
     */
    Vector(int d, double *a) {
        this->d = d;
        // defensive copy so that client can't alter our copy of data[]
        this->data = new double[d];
        for (int i = 0; i < d; i++) this->data[i] = a[i];
    }

    Vector(const Vector &v) {
        this->d = v.d;
        this->data = new double[this->d];
        for (int i = 0; i < d; i++) {
            this->data[i] = v.data[i];
        }
    }

    Vector &operator = (const Vector &v) {
        if (this != &v) {
            delete this->data;
            this->d = v.d;
            this->data = new double[this->d];
            for (int i = 0; i < d; i++) {
                this->data[i] = v.data[i];
            }
        }
        return *this;
    }

    Vector(Vector &&v) {
        this->d = v.d;
        this->data = v.data;
        v.data = nullptr;
    }

    Vector &operator = (Vector &&v) {
        if (this != &v) {
            delete this->data;
            this->d = v.d;
            this->data = v.data;
            v.data = nullptr;
        }
        return *this;
    }

    ~Vector() {
        delete data;
    }

    /**
     * Returns the dimension of this vector.
     *
     * @return the dimension of this vector
     */
    int dimension() {
        return d;
    }

    /**
     * Returns the dot product of this vector with the specified vector.
     *
     * @param  that the other vector
     * @return the dot product of this vector and that vector
     * @throws invalid_argument if the dimensions of the two vectors are not equal
     */
    double dot(Vector &that) {
        if (d != that.d) throw invalid_argument("Dimensions don't agree");
        double sum = 0.0;
        for (int i = 0; i < d; i++)
            sum = sum + (data[i] * that.data[i]);
        return sum;
    }

    /**
     *
     * Returns the dot product of this vector with the specified vector.
     *
     * @param  that the other vector
     * @return the dot product of this vector and that vector
     * @throws invalid_argument if the dimensions of the two vectors are not equal
     */
    double operator * (Vector &that) {
        return dot(that);
    }

    /**
     * Returns the 2-D cross product of this vector with the specified vector.
     *
     * @param  that the other vector
     * @return the 2-D cross product of this vector and that vector
     * @throws invalid_argument if the two vectors are not 2-dimensional
     */
    double cross2D(Vector &that) {
        if (d != 2 || that.d != 2) throw invalid_argument("Vectors must be 2-dimensional");
        return data[0] * that.data[1] - data[1] * that.data[0];
    }

    /**
     * Returns the 2-D cross product of this vector with the specified vector.
     *
     * @param  that the other vector
     * @return the 2-D cross product of this vector and that vector
     * @throws invalid_argument if the two vectors are not 2-dimensional
     */
    double operator | (Vector &that) {
        return cross2D(that);
    }

    /**
     * Returns the 3-D cross product of this vector with the specified vector.
     *
     * @param  that the other vector
     * @return the 3-D cross product of this vector and that vector
     * @throws invalid_argument if the two vectors are not 3-dimensional
     */
    Vector &cross3D(Vector &that) {
        if (d != 3 || that.d != 3) throw invalid_argument("Vectors must be 3-dimensional");
        Vector *c = new Vector(d);
        c->data[0] = data[1] * that.data[2] - data[2] * that.data[1];
        c->data[1] = data[2] * that.data[0] - data[0] * that.data[2];
        c->data[2] = data[0] * that.data[1] - data[1] * that.data[0];
        return *c;
    }

    /**
     * Returns the 3-D cross product of this vector with the specified vector.
     *
     * @param  that the other vector
     * @return the 3-D cross product of this vector and that vector
     * @throws invalid_argument if the two vectors are not 3-dimensional
     */
    Vector &operator ^ (Vector &that) {
        return cross3D(that);
    }

    /**
     * Returns the magnitude of this vector.
     * This is also known as the L2 norm or the Euclidean norm.
     *
     * @return the magnitude of this vector
     */
    double magnitude() {
        return sqrt(this->dot(*this));
    }

    /**
     * Returns the Euclidean distance between this vector and the specified vector.
     *
     * @param  that the other vector
     * @return the Euclidean distance between this vector and that vector
     * @throws invalid_argument if the dimensions of the two vectors are not equal
     */
    double distanceTo(Vector &that) {
        if (d != that.d) throw invalid_argument("Dimensions don't agree");
        return minus(that).magnitude();
    }

    /**
     * Returns the sum of this vector and the specified vector.
     *
     * @param  that the vector to add to this vector
     * @return the vector whose value is {@code (this + that)}
     * @throws invalid_argument if the dimensions of the two vectors are not equal
     */
    Vector &plus(Vector &that) {
        if (d != that.d) throw invalid_argument("Dimensions don't agree");
        Vector *c = new Vector(d);
        for (int i = 0; i < d; i++)
            c->data[i] = data[i] + that.data[i];
        return *c;
    }

    /**
     * Returns the sum of this vector and the specified vector.
     *
     * @param  that the vector to add to this vector
     * @return the vector whose value is {@code (this + that)}
     * @throws invalid_argument if the dimensions of the two vectors are not equal
     */
    Vector &operator + (Vector &that) {
        return plus(that);
    }

    /**
     * Returns the difference between this vector and the specified vector.
     *
     * @param  that the vector to subtract from this vector
     * @return the vector whose value is {@code (this - that)}
     * @throws invalid_argument if the dimensions of the two vectors are not equal
     */
    Vector &minus(Vector &that) {
        if (d != that.d) throw invalid_argument("Dimensions don't agree");
        Vector *c = new Vector(d);
        for (int i = 0; i < d; i++)
            c->data[i] = data[i] - that.data[i];
        return *c;
    }

    /**
     * Returns the difference between this vector and the specified vector.
     *
     * @param  that the vector to subtract from this vector
     * @return the vector whose value is {@code (this - that)}
     * @throws invalid_argument if the dimensions of the two vectors are not equal
     */
    Vector &operator - (Vector &that) {
        return minus(that);
    }

    /**
     * Returns the ith cartesian coordinate.
     *
     * @param  i the coordinate index
     * @return the ith cartesian coordinate
     */
    double cartesian(int i) {
        return data[i];
    }

    // mutator operator
    double &operator [](int i) {
        return data[i];
    }

    // accessor operator
    double operator [](int i) const {
        return data[i];
    }

    /**
     * Returns the scalar-vector product of this vector and the specified scalar
     *
     * @param  alpha the scalar
     * @return the vector whose value is {@code (alpha * this)}
     */
    Vector &scale(double alpha) {
        Vector *c = new Vector(d);
        for (int i = 0; i < d; i++)
            c->data[i] = alpha * data[i];
        return *c;
    }

    /**
     * Returns the scalar-vector product of this vector and the specified scalar
     *
     * @param  alpha the scalar
     * @return the vector whose value is {@code (alpha * this)}
     */
    Vector &operator * (double alpha) {
        return scale(alpha);
    }

    /**
     * Returns the scalar-vector product of this vector and the specified the reciprocal of the scalar
     *
     * @param  alpha the scalar
     * @return the vector whose value is {@code (alpha * this)}
     */
    Vector &operator / (double alpha) {
        return scale(1.0 / alpha);
    }

    /**
     * Returns a unit vector in the direction of this vector.
     *
     * @return a unit vector in the direction of this vector
     * @throws runtime_error if this vector is the zero vector
     */
    Vector &direction() {
        if (magnitude() == 0.0) throw runtime_error("Zero-vector has no direction");
        return scale(1.0 / magnitude());
    }

    /**
     * Returns the direction cosine of this angle in the ith dimension.
     *
     * @param i the direction number (0-indexed)
     * @return the direction cosine of this angle in the ith dimension
     */
    double directionCosine(int i) {
        return data[i] / magnitude();
    }

    /**
     * Returns a projection of this vector on that vector.
     *
     * @param that the vector that this vector is to be projected on
     * @return a projection of this vector on that vector
     * @throws invalid_argument if the dimensions of the two vectors are not equal
     */
    Vector &projectionOn(Vector &that) {
        if (d != that.d) throw invalid_argument("Dimensions don't agree");
        return that.scale(dot(that) / that.dot(that));
    }

    /**
     * Returns a vector that is this vector rotated theta radians around that vector.
     *
     * @param that vector representing the axis of rotation
     * @param theta the angle in radians
     * @return a vector that is this vector rotated theta radians around that vector
     * @throws invalid_argument if the dimensions of the two vectors are not equal
     * @throws invalid_argument if the two vectors are not 2-dimensional or 3-dimensional
     */
    Vector &rotate(Vector &that, double theta) {
        if (d == 2 && that.d == 2) {
            Vector *r = new Vector(2);
            r->data[0] = that.data[0] + (data[0] - that.data[0]) * cos(theta) - (data[1] - that.data[1]) * sin(theta);
            r->data[1] = that.data[1] + (data[0] - that.data[0]) * sin(theta) + (data[1] - that.data[1]) * cos(theta);
            return *r;
        } else if (d == 3 && that.d == 3) {
            Vector *r = new Vector(3);
            r = &this->scale(cos(theta)).plus(that.direction().cross3D(*this).scale(sin(theta))).plus(that.direction().scale(that.direction().dot(*this)).scale(1.0 - cos(theta)));
            return *r;
        } else if (d == that.d) {
            throw invalid_argument("Vectors must be 2-dimensional or 3-dimensional");
        } else {
            throw invalid_argument("Dimensions don't agree");
        }
    }
};

#endif /* DATASTRUCTURES_GEOMETRY_VECTOR_H_ */
