package algorithms.math;

public class GaussianElimination {
    private static final double EPSILON = 1e-10;

    // Gaussian elimination with partial pivoting
    public static double[] solve(double[][] A, double[] b) {
        int n = b.length;
        for (int p = 0; p < n; p++) {
            // find pivot row and swap
            int m = p;
            for (int i = p + 1; i < n; i++) {
                if (Math.abs(A[i][p]) > Math.abs(A[m][p])) m = i;
            }
            double[] tempA = A[p];
            A[p] = A[m];
            A[m] = tempA;
            double tempB = b[p];
            b[p] = b[m];
            b[m] = tempB;
            // singular or nearly singular
            if (Math.abs(A[p][p]) <= EPSILON) throw new ArithmeticException("Matrix is singular or nearly singular");
            // pivot within A and b
            for (int i = p + 1; i < n; i++) {
                double alpha = A[i][p] / A[p][p];
                b[i] -= alpha * b[p];
                for (int j = p; j < n; j++) {
                    A[i][j] -= alpha * A[p][j];
                }
            }
        }
        // back substitution
        double[] x = new double[n];
        for (int i = n - 1; i >= 0; i--) {
            double sum = 0.0;
            for (int j = i + 1; j < n; j++) {
                sum += A[i][j] * x[j];
            }
            x[i] = (b[i] - sum) / A[i][i];
        }
        return x;
    }
}
