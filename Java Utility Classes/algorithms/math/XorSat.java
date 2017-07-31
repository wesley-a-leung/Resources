package algorithms.math;

public class XorSat {
    // Gaussian elimination with partial pivoting
    public static boolean[] solve(boolean[][] A, boolean[] b) {
        int n = b.length;
        for (int p = 0; p < n; p++) {
            // find pivot row and swap
            int m = p;
            for (int i = 0; i < p; i++) {
                int j = i;
                while (j < n && !A[i][j]) {
                    j++;
                }
                if (j == p) m = i;
            }
            for (int i = p + 1; i < n; i++) {
                if (A[i][p]) m = i;
            }
            if (!A[m][p]) continue;
            boolean[] tempA = A[p];
            A[p] = A[m];
            A[m] = tempA;
            boolean tempB = b[p];
            b[p] = b[m];
            b[m] = tempB;
            // pivot within A and b
            for (int i = 0; i < n; i++) {
                if (p == i || !A[i][p]) continue;
                b[i] ^= b[p];
                for (int j = p; j < n; j++) {
                    A[i][j] ^= A[p][j];
                }
            }
        }
        // back substitution
        for (int i = n - 1; i >= 0; i--) {
            if (!A[i][i] && b[i]) throw new ArithmeticException("Matrix has no solution");
            if (!b[i]) continue;
            for (int j = i - 1; j >= 0; j--) {
                b[j] ^= A[j][i];
                A[j][i] = false;
            }
        }
        return b;
    }
}
