#ifndef DATASTRUCTURES_ARRAYINDEXPQ_H_
#define DATASTRUCTURES_ARRAYINDEXPQ_H_

#include <bits/stdc++.h>
using namespace std;

class no_such_element_exception: public runtime_error {
public:
    no_such_element_exception(): runtime_error("No such element exists"){}
    no_such_element_exception(string message): runtime_error(message){}
};

/**
 *  The {@code ArrayIndexPQ} class represents an indexed priority queue of
 *  generic keys.
 *  It supports the usual <em>insert</em> and <em>pop</em>
 *  operations, along with <em>remove</em> and <em>change-the-key</em>
 *  methods. In order to let the client refer to items on the priority queue,
 *  an integer between {@code 0} and {@code maxN - 1} is associated with each key.
 *  The client uses this integer to specify which key to delete or change.
 *  It also supports methods for peeking at the top key, testing if the
 *  priority queue is empty, and iterating through the keys.
 *  <p>
 *  This implementation uses a binary heap along with an array to associate
 *  keys with integers in the given range.
 *  The <em>insert</em>, <em>pop</em>, <em>remove</em>,
 *  <em>change-key</em>, <em>decrease-key</em>, and <em>increase-key</em>
 *  operations take logarithmic time.
 *  The <em>is-empty</em>, <em>size</em>, <em>top</em>,
 *  and <em>key-of</em> operations take constant time.
 *  Construction takes time proportional to the specified capacity.
 */
// convention is same as priority_queue in STL
template <typename Key, typename Comparator = less<Key>>
struct ArrayIndexPQ {
private:
    Comparator cmp;

    int maxN;       // the maximum number of elements in the priority queue
    int n;          // number of elements on PQ
    int *pq;        // binary heap using 1-based indexing
    int *qp;        // inverse of pq; qp[i] = heap location of the key at index i; qp[pq[i]] = pq[qp[i]] = i
    Key *keys;      // keys[i] = priority of index i

    void exch(int i, int j) {
        swap(pq[i], pq[j]);
        qp[pq[i]] = i;
        qp[pq[j]] = j;
    }

    void swim(int k) {
        while (k > 1 && cmp(keys[pq[k/2]], keys[pq[k]])) {
            exch(k, k/2);
            k = k/2;
        }
    }

    void sink(int k) {
        while (2*k <= n) {
            int j = 2*k;
            if (j < n && cmp(keys[pq[j]], keys[pq[j+1]])) j++;
            if (!cmp(keys[pq[k]], keys[pq[j]])) break;
            exch(k, j);
            k = j;
        }
    }

public:
    /**
     * Initializes an empty indexed priority queue with indices between {@code 0}
     * and {@code maxN - 1}. {@code maxN} must be a non negative number.
     *
     * @param  maxN the keys on this priority queue are index from {@code 0} to {@code maxN - 1}
     */
    ArrayIndexPQ(int maxN) {
        assert(maxN >= 0);
        this->maxN = maxN;
        n = 0;
        keys = new Key[maxN];
        pq = new int[maxN + 1];
        qp = new int[maxN];
        for (int i = 0; i < maxN; i++) qp[i] = -1;
    }

    virtual ~ArrayIndexPQ() {
        delete[](pq);
        delete[](qp);
        delete[](keys);
    }

    /**
     * Returns true if this priority queue is empty.
     *
     * @return {@code true} if this priority queue is empty;
     *         {@code false} otherwise
     */
    bool isEmpty() {
        return n == 0;
    }

    /**
     * Is {@code i} an index on this priority queue?
     *
     * @param  i an index
     * @return {@code true} if {@code i} is an index on this priority queue;
     *         {@code false} otherwise
     */
    bool contains(int i) {
        assert(0 <= i && i < maxN);
        return qp[i] != -1;
    }

    /**
     * Returns the number of keys on this priority queue.
     *
     * @return the number of keys on this priority queue
     */
    int size() {
        return n;
    }

    /**
      * Associate key with index i.
      *
      * @param  i an index
      * @param  key the key to associate with index {@code i}
      * @throws invalid_argument if there already is an item
      *         associated with index {@code i}
      */
     void insert(int i, Key key) {
         assert(0 <= i && i < maxN);
         if (contains(i)) throw invalid_argument("index is already in the priority queue");
         n++;
         qp[i] = n;
         pq[n] = i;
         keys[i] = key;
         swim(n);
     }

     /**
      * Returns a pair containing the index and key at the top of the heap.
      *
      * @return a pair containing the index and key at the top of the heap
      * @throws no_such_element_exception if this priority queue is empty
      */
     pair<int, Key> top() {
         if (n == 0) throw no_such_element_exception("Priority queue underflow");
         return make_pair(pq[1], keys[pq[1]]);
     }

     /**
      * Removes the top key and returns a pair containing its associated index
      * and the key removed.
      *
      * @return a pair containing the index and key at the top of the heap
      * @throws no_such_element_exception if this priority queue is empty
      */
     pair<int, Key> pop() {
         if (n == 0) throw no_such_element_exception("Priority queue underflow");
         int minInd = pq[1];
         Key minKey = keys[minInd];
         exch(1, n--);
         sink(1);
         // assert pq[n+1] == min;
         qp[minInd] = -1;        // delete
         pq[n+1] = -1;        // not needed
         return make_pair(minInd, minKey);
     }

     /**
      * Returns the key associated with index {@code i}.
      *
      * @param  i the index of the key to return
      * @return the key associated with index {@code i}
      * @throws no_such_element_exception no key is associated with index {@code i}
      */
     Key keyOf(int i) {
         assert(0 <= i && i < maxN);
         if (!contains(i)) throw no_such_element_exception("index is not in the priority queue");
         else return keys[i];
     }

     /**
      * Change the key associated with index {@code i} to the specified value.
      *
      * @param  i the index of the key to change
      * @param  key change the key associated with index {@code i} to this key
      * @throws no_such_element_exception no key is associated with index {@code i}
      */
     void changeKey(int i, Key key) {
         assert(0 <= i && i < maxN);
         if (!contains(i)) throw no_such_element_exception("index is not in the priority queue");
         Key old = keys[i];
         keys[i] = key;
         if (cmp(old, key)) swim(qp[i]);
         else if (cmp(key, old)) sink(qp[i]);
     }

     /**
      * Removes the key on the priority queue associated with index {@code i}.
      *
      * @param  i the index of the key to remove
      * @throws no_such_element_exception no key is associated with index {@code i}
      */
     void remove(int i) {
         assert(0 <= i && i < maxN);
         if (!contains(i)) throw no_such_element_exception("index is not in the priority queue");
         int index = qp[i];
         exch(index, n--);
         swim(index);
         sink(index);
         qp[i] = -1;
     }
};

#endif /* DATASTRUCTURES_ARRAYINDEXPQ_H_ */
