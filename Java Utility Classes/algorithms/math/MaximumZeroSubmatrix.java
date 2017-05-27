package algorithms.math;

import datastructures.Stack;

public class MaximumZeroSubmatrix {
	
	// This class should not be instantiated
	private MaximumZeroSubmatrix() {}
	
	public static int getMaxZeroSubmatrix(int[][] a) {
		int rows = a.length;
		int cols = a[0].length;
		int[][] height = new int[rows][cols];
		Stack<Integer> s = new Stack<Integer>();
		int ret = 0;
		for (int j = 0; j < cols; j++) {
			for (int i = rows - 1; i >= 0; i--) {
				if (a[i][j] == 1)
					height[i][j] = 0;
				else
					height[i][j] = 1 + (i == rows - 1 ? 0 : height[i + 1][j]);
			}
		}
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				int minIndex = j;
				while (!s.isEmpty() && height[i][s.peek()] >= height[i][j]) {
					ret = Math.max(ret, (j - s.peek()) * (height[i][s.peek()]));
					minIndex = s.peek();
					height[i][minIndex] = height[i][j];
					s.pop();
				}
				s.push(minIndex);
			}
			while (!s.isEmpty()) {
				ret = Math.max(ret, (cols - s.peek()) * height[i][s.peek()]);
				s.pop();
			}
		}
		return ret;
	}
}
