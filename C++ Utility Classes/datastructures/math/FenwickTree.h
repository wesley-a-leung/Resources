/*
 * FenwickTree.h
 *
 *  Created on: May 26, 2017
 *      Author: Wesley Leung
 */

#ifndef DATASTRUCTURES_MATH_FENWICKTREE_H_
#define DATASTRUCTURES_MATH_FENWICKTREE_H_

#include <bits/stdc++.h>

using namespace std;

struct FenwickTree {
private:
	int* array;
	int length;

public:
	FenwickTree(int size) {
		this->length = size + 1;
		array = new int[length];
	}

	int rsq(int ind) {
		int sum = 0;
		for (int x = ind; x > 0; x -= (x & -x)) {
			sum += array[x];
		}
		return sum;
	}

	int rsq(int a, int b) {
		return rsq(b) - rsq(a - 1);
	}

	void update(int ind, int value) {
		for (int x = ind; x < length; x += (x & -x)) {
			array[x] += value;
		}
	}

	int size() {
		return length - 1;
	}
};

#endif /* DATASTRUCTURES_MATH_FENWICKTREE_H_ */
