/*
 * ReversionSegmentTree.h
 *
 *  Created on: Jun 3, 2017
 *      Author: Wesley Leung
 */

#ifndef DATASTRUCTURES_MATH_REVERSIONSEGMENTTREE_H_
#define DATASTRUCTURES_MATH_REVERSIONSEGMENTTREE_H_

#include <bits/stdc++.h>

using namespace std;

struct Node {
public:
	Node* left;
	Node* right;
	int pre, suf, sum;
	Node(int val) {
		this->pre = val;
		this->suf = val;
		this->sum = val;
		this->left = this->right = nullptr;
	}

	Node(Node* l, Node* r) {
		this->left = l;
		this->right = r;
		this->pre = max(l->pre, r->pre + l->sum);
		this->suf = max(l->suf + r->sum, r->suf);
		this->sum = l->sum + r->sum;
	}
};

struct Query {
public:
	int pre, suf, sum;
	bool isNull;
	Query() {
		this->pre = 0;
		this->suf = 0;
		this->sum = 0;
		this->isNull = true;
	}
	Query(int pre, int suf, int sum) {
		this->pre = pre;
		this->suf = suf;
		this->sum = sum;
		this->isNull = false;
	}

	Query(Query l, Query r) {
		this->pre = max(l.pre, r.pre + l.sum);
		this->suf = max(l.suf + r.sum, r.suf);
		this->sum = l.sum + r.sum;
		this->isNull = false;
	}
};

struct SegmentTree {
private:
	Node** rev;
	int* arr;
	int revInd = 0;
	int N;

	Node* build(int l, int r) {
		if (l == r) return new Node(arr[l]);
		int m = (l + r) >> 1;
		return new Node(build(l , m), build(m + 1, r));
	}

	Node* update(Node* cur, int l, int r, int ind) {
		if (l <= ind && ind <= r) {
			if (l == r) return new Node(arr[l]);
			int m = (l + r) >> 1;
			return new Node(update(cur->left, l, m, ind), update(cur->right, m + 1, r, ind));
		}
		return cur;
	}

	Query query(Node* cur, int l, int r, int ql, int qr) {
		if (l > qr || r < ql) return Query();
		if (l >= ql && r <= qr) return Query(cur->pre, cur->suf, cur->sum);
		int m = (l + r) >> 1;
		Query left = query(cur->left, l, m, ql, qr);
		Query right = query(cur->right, m + 1, r, ql, qr);
		if (left.isNull) return right;
		if (right.isNull) return left;
		return Query(left, right);
	}

public:
	void init(int size, int* array, int rev) {
		rev = new Node[rev];
		arr = new int[size + 1];
		for (int i = 1; i <= size; i++) {
			arr[i] = array[i - 1];
		}
		rev[0] = build(1, size);
		N = size;
	}

	void update(int ind, int val) {
		arr[ind] = val;
		rev[revInd + 1] = update(rev[revInd], 1, N, ind);
		revInd++;
	}

	int query(int type, int ql, int qr) {
		if (type == 1) return query(rev[revInd], 1, N, ql, qr).pre;
		return query(rev[revInd], 1, N, ql, qr).suf;
	}
	void revert(int x) {
		rev[++revInd] = rev[x];
	}
};

#endif /* DATASTRUCTURES_MATH_REVERSIONSEGMENTTREE_H_ */
